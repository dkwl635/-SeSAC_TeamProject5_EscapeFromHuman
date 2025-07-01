// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Actor/Tractor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/AnimalBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATractor::ATractor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(300.f, 150.f, 130.f));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATractor::HitPlayer);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Body
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(BoxComp);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Wheels
	FrontLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontLeftWheel"));
	FrontLeftWheel->SetupAttachment(Body);

	FrontRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontRightWheel"));
	FrontRightWheel->SetupAttachment(Body);

	RearLeftWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearLeftWheel"));
	RearLeftWheel->SetupAttachment(Body);

	RearRightWheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearRightWheel"));
	RearRightWheel->SetupAttachment(Body);

	
	FrontLeftWheel->SetRelativeRotation(FRotator::ZeroRotator);
	FrontRightWheel->SetRelativeRotation(FRotator::ZeroRotator);
	RearLeftWheel->SetRelativeRotation(FRotator::ZeroRotator);
	RearRightWheel->SetRelativeRotation(FRotator::ZeroRotator);

}

// Called when the game starts or when spawned
void ATractor::BeginPlay()
{
	Super::BeginPlay();

	PreviousLocation = GetActorLocation();
	currentTime = 0.f;
	startPos = GetActorLocation();

	SetReplicates(true);
	SetReplicateMovement(true);

	if (HasAuthority())
	{
		Multicast_PlayDriveSound();
	}
}

// Called every frame
void ATractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
	{
		currentTime += DeltaTime;

		if (currentTime < MovingTime)
		{
			SetActorLocation(startPos + GetActorForwardVector() * MovingSpeed * currentTime);
		}
		else
		{
			Destroy();
		}
	}

	// 바퀴 회전 계산
	FVector CurrentLocation = GetActorLocation();
	FVector DeltaLocation = CurrentLocation - PreviousLocation;
	float ForwardDistance = FVector::DotProduct(DeltaLocation, GetActorForwardVector());

	float WheelCircumference = 2 * PI * WheelRadius;
	float RotationAngle = (ForwardDistance / WheelCircumference) * 360.f;

	// X축 회전
	FQuat WheelQuatRotation = FQuat(FRotator(0.F, 0.F, RotationAngle)); // X축 기준 회전

	FrontLeftWheel->AddRelativeRotation(WheelQuatRotation);
	FrontRightWheel->AddRelativeRotation(WheelQuatRotation);
	RearLeftWheel->AddRelativeRotation(WheelQuatRotation);
	RearRightWheel->AddRelativeRotation(WheelQuatRotation);

	PreviousLocation = CurrentLocation;
}

void ATractor::HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AAnimalBase* HitPlayer = Cast<AAnimalBase>(OtherActor);
		if (HitPlayer)
		{
			HitPlayer->Die();
		}
	}
}

void ATractor::Multicast_PlayDriveSound_Implementation()
{
	if (DrivingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DrivingSound, GetActorLocation());
	}
}

void ATractor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATractor, currentTime);
}