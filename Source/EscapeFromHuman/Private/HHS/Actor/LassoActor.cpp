// Fill out your copyright notice in the Description page of Project Settings.
#include "HHS/Actor/LassoActor.h"

#include "CableComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/Engine.h"
#include "LJW/PlayerInputComponent.h"

// Sets default values
ALassoActor::ALassoActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	StartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartPoint"));
	StartPoint->SetupAttachment(Root);

	EndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	EndPoint->SetupAttachment(Root);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Root);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(EndPoint);
	CollisionSphere->InitSphereRadius(20.f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSphere->SetGenerateOverlapEvents(true);
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ALassoActor::OnOverlapBegin);


	bIsSpinning = false;
	bIsThrown = false;
	bIsPullingTarget = false;
	SpinAngle = 0.f;
	CurrentThrowDistance = 0.f;
}

void ALassoActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnLassoHit(OtherActor);
}

void ALassoActor::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerAI = GetOwner();

	if (ACharacter* CharacterOwner = Cast<ACharacter>(OwnerAI))
	{
		if (CharacterOwner->GetMesh())
		{
			StartPoint->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_R"));
		}
	}

	// Spline 초기화
	Spline->ClearSplinePoints();
	for (int32 i = 0; i <= SplinePointCount; i++)
	{
		Spline->AddSplinePoint(FVector::ZeroVector, ESplineCoordinateSpace::Local);
	}

	// SplineMesh 생성
	for (int32 i = 0; i < SplinePointCount; i++)
	{
		USplineMeshComponent* Mesh = NewObject<USplineMeshComponent>(this);
		Mesh->SetMobility(EComponentMobility::Movable); 

		Mesh->RegisterComponentWithWorld(GetWorld());
		Mesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		Mesh->SetStaticMesh(RopeSegmentMesh);
		Mesh->SetMaterial(0, RopeMaterial);
		Mesh->SetForwardAxis(ESplineMeshAxis::X);
		SplineMeshes.Add(Mesh);
	}
	UE_LOG(LogTemp, Warning, TEXT("RopeSegmentMesh is %s"), RopeSegmentMesh ? TEXT("Valid") : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("SplineMesh Created: %d"), SplineMeshes.Num());

}

void ALassoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ACharacter* CharacterOwner = Cast<ACharacter>(OwnerAI))
	{
		StartPoint->SetWorldLocation(CharacterOwner->GetMesh()->GetSocketLocation(FName("Hand_R")));
	}

	if (bIsSpinning)
	{
		SpinAngle += SpinSpeed * DeltaTime;
		if (SpinAngle > 360.f) SpinAngle -= 360.f;

		float Radian = FMath::DegreesToRadians(SpinAngle);
		FVector Offset = FVector(FMath::Cos(Radian) * SpinRadius, FMath::Sin(Radian) * SpinRadius, 50.f);
		EndPoint->SetWorldLocation(StartPoint->GetComponentLocation() + Offset);
	}

	if (bIsThrown && !bIsPullingTarget)
	{
		CurrentThrowDistance += ThrowSpeed * DeltaTime;
		FVector NewLocation = StartPoint->GetComponentLocation() + ThrowDirection * CurrentThrowDistance;
		EndPoint->SetWorldLocation(NewLocation);

		// 충돌 체크
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(OwnerAI);

		FVector Start = StartPoint->GetComponentLocation();
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, NewLocation, ECC_GameTraceChannel1, Params))
		{
			OnLassoHit(Hit.GetActor());
			bIsThrown = false;
		}
	}

	if (bIsPullingTarget && TargetActor && OwnerAI)
	{
		AAnimalBase* TargetAnimal  = Cast<AAnimalBase>(TargetActor);
		if (TargetAnimal)
		{
			if (TargetAnimal->bShouldEndPull)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player escaped from lasso!"));

				TargetAnimal->bShouldEndPull = false;

				TargetAnimal->EscapeLasso();

				StopLasso();
				return;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetActor is not an AnimalBase"));
		}
		
		FVector ToAI = OwnerAI->GetActorLocation();
		FVector FromTarget = TargetActor->GetActorLocation();
		FVector Direction = (ToAI - FromTarget).GetSafeNormal();

		if (FVector::Dist(FromTarget, ToAI) > 100.f)
		{
			TargetActor->SetActorLocation(FromTarget + Direction * PullSpeed * DeltaTime);
		}
		else
		{
			StopLasso();
		}
	}
	UpdateSpline();
}

void ALassoActor::StartSpinning()
{
	bIsSpinning = true;
	bIsThrown = false;
	bIsPullingTarget = false;
	SpinAngle = 0.f;
	
	CollisionSphere->SetGenerateOverlapEvents(false);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALassoActor::ThrowLasso(AActor* Target)
{
	if (!Target) return;

	bIsSpinning = false;
	bIsThrown = true;
	bIsPullingTarget = false;
	CurrentThrowDistance = 0.f;

	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	TargetActor = Cast<AAnimalBase>(Target);
	if (TargetActor)
	{
		Target = TargetActor;
	}

	FVector Start = StartPoint->GetComponentLocation();
	FVector End = Target->GetActorLocation();
	ThrowDirection = (End - Start).GetSafeNormal();
}

void ALassoActor::StopLasso()
{
	bIsSpinning = false;
	bIsThrown = false;
	bIsPullingTarget = false;

	CollisionSphere->SetGenerateOverlapEvents(false);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (EndPoint->IsAttachedTo(TargetActor ? TargetActor->GetRootComponent() : nullptr))
	{
		EndPoint->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
	TargetActor = nullptr;
}

void ALassoActor::UpdateSpline()
{
	FVector Start = StartPoint->GetComponentLocation();
	FVector Center = Start + FVector(0, 0, 30.f); // 머리 위 높이
	float LoopRadius = SpinRadius * 0.6f;         // 원 크기 줄임

	int32 HalfPoint = SplinePointCount / 2;

	for (int32 i = 0; i <= SplinePointCount; i++)
	{
		FVector Point;

		if (bIsSpinning)
		{
			if (i <= HalfPoint)
			{
				float T = i / (float)HalfPoint;
				Point = FMath::Lerp(Start, Center, T);
			}
			else
			{
				float LoopT = (i - HalfPoint) / (float)(SplinePointCount - HalfPoint);
				float Angle = LoopT * 2.f * PI + FMath::DegreesToRadians(SpinAngle);

				// 추가된: 위아래로 출렁이는 효과
				float VerticalWobble = FMath::Sin(LoopT * 4.f * PI + GetWorld()->TimeSeconds * 2.f) * 3.f;

				FVector CircularOffset = FVector(
					FMath::Cos(Angle) * LoopRadius,
					FMath::Sin(Angle) * LoopRadius * 0.6f,
					VerticalWobble
				);

				Point = Center + CircularOffset;
			}
		}
		else if (bIsThrown)
		{
			FVector End = EndPoint->GetComponentLocation();
			float T = i / (float)SplinePointCount;
			FVector PointBase = FMath::Lerp(Start, End, T);
			Point = PointBase + FVector(0, 0, FMath::Sin(T * PI) * 150.f);
		}
		else
		{
			FVector End = EndPoint->GetComponentLocation();
			float T = i / (float)SplinePointCount;
			Point = FMath::Lerp(Start, End, T);
		}

		Spline->SetLocationAtSplinePoint(i, Point, ESplineCoordinateSpace::World);
	}

	// SplineMesh 적용
	for (int32 i = 0; i < SplinePointCount; i++)
	{
		FVector StartLoc, StartTangent, EndLoc, EndTangent;

		Spline->GetLocationAndTangentAtSplinePoint(i, StartLoc, StartTangent, ESplineCoordinateSpace::Local);
		Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndLoc, EndTangent, ESplineCoordinateSpace::Local);

		// 탄젠트 조정으로 로프의 딱딱함 완화
		StartTangent *= 0.7f;
		EndTangent *= 0.7f;
		
		if (SplineMeshes.IsValidIndex(i) && SplineMeshes[i])
		{
			SplineMeshes[i]->SetStartAndEnd(StartLoc, StartTangent, EndLoc, EndTangent);
		}
	}
}

void ALassoActor::OnLassoHit(AActor* HitActor)
{
	if (!HitActor) return;

	UE_LOG(LogTemp, Warning, TEXT("OnLassoHit: HitActor = %s"), *HitActor->GetName());
	
	AAnimalBase* AnimalTarget = Cast<AAnimalBase>(HitActor);
	if (AnimalTarget)
	{
		TargetActor = AnimalTarget;
		UE_LOG(LogTemp, Warning, TEXT("HitLasso() should be called now"));
		AnimalTarget->HitLasso(); 
		bIsPullingTarget = true;

		// Head 소켓에 EndPoint 붙이기
		if (USkeletalMeshComponent* Mesh = AnimalTarget->GetMesh())
		{
			if (Mesh->DoesSocketExist(FName("Head")))
			{
				EndPoint->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Head"));
				UE_LOG(LogTemp, Warning, TEXT("EndPoint attached to Head socket"));
			}
		}
	}
}

