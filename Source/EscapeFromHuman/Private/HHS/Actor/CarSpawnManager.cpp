// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Actor/CarSpawnManager.h"
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HHS/Actor/CarActor.h"
#include "HHS/Actor/ArrowDecalActor.h"
#include "HHS/Actor/GroundActor.h"
#include "Kismet/GameplayStatics.h"
#include "KJW/Game/EFHGameState.h"

// Sets default values
ACarSpawnManager::ACarSpawnManager()
{
	bReplicates = true;

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(RootComponent);

	bReplicates = true;
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void ACarSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	AEFHGameState* GS = GetWorld ()->GetGameState<AEFHGameState> ();
	if (GS)
	{
		GS->OnStartStateEvent.AddUObject ( this , &ThisClass::CarStart );
	}

	
	if (HasAuthority())
	{
	
		GetWorldTimerManager().SetTimer(SpawnTimerHandle,
			this,
			&ACarSpawnManager::TrySpawnCarWithSound,
			SpawnInterval, 
			true,          
			StartTime       
		);
	}	
}

void ACarSpawnManager::SpawnCar()
{
	if (CarClass.Num() == 0) return;

	// 1. 랜덤한 차량 클래스 선택
	int32 RandomIndex = FMath::RandRange(0, CarClass.Num() - 1);
	TSubclassOf<ACarActor> SelectedClass = CarClass[RandomIndex];

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 300.0f); // 아래 방향으로 긴 트레이스

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// 2. 멀티 라인트레이스
	bool bHit = GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, QueryParams);

	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("라인트레이스로 %d개 액터에 충돌함"), HitResults.Num());
		
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor) continue;
			//UE_LOG(LogTemp, Warning, TEXT("Hit된 액터 이름: %s"), *HitActor->GetName());


			if (HitActor->IsA(AGroundActor::StaticClass()))
			{
				//UE_LOG(LogTemp, Warning, TEXT("충돌한 액터는 GroundActor임"));

				FVector GroundLocation = Hit.ImpactPoint;

				// 2. 차량 기본 오브젝트 얻기
				ACarActor* DefaultCar = SelectedClass->GetDefaultObject<ACarActor>();
				if (DefaultCar)
				{
					// 3. BoxComponent 찾기
					UBoxComponent* BoxComp = DefaultCar->FindComponentByClass<UBoxComponent>();
					if (BoxComp)
					{
						FVector BoxExtent = BoxComp->GetScaledBoxExtent();
						FVector BoxLocation = BoxComp->GetRelativeLocation();

						// Z 오프셋 계산 (박스의 밑면 기준으로 차량 위치 조정)
						// BoxLocation.Z가 로컬 기준에서 박스 중심 위치라면, Z 오프셋 = BoxExtent.Z - BoxLocation.Z
						float ZOffset = BoxExtent.Z - BoxLocation.Z;

						// 최종 스폰 위치
						GroundLocation.Z += ZOffset;

						
						// 4. 차량 스폰
						FActorSpawnParameters SpawnParams;
						ACarActor* SpawnedCar = GetWorld()->SpawnActor<ACarActor>(
							SelectedClass,
							GroundLocation,
							GetActorRotation(),
							SpawnParams
						);

						if (SpawnedCar)
						{
							SpawnedCar->SetReplicates(true);
							SpawnedCar->SetReplicateMovement(true);
							CarActor = SpawnedCar;
						}

						return;
					}
				}
			}
		}
	}
}

void ACarSpawnManager::TrySpawnCarWithSound()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 300.0f);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, QueryParams);
	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (Hit.GetActor() && Hit.GetActor()->IsA(AGroundActor::StaticClass()))
			{
				FVector GroundLocation = Hit.ImpactPoint;

				// 이미 생성된 화살표가 있다면 제거
				if (SpawnArrowDecalActor)
				{
					SpawnArrowDecalActor->Destroy();
					SpawnArrowDecalActor = nullptr;
				}

				// 화살표 데칼 생성
				if (ArrowDecalClass)
				{
					FActorSpawnParameters SpawnParams;
					SpawnArrowDecalActor = GetWorld()->SpawnActor<AArrowDecalActor>(ArrowDecalClass, GroundLocation + FVector(400,0,10), FRotator::ZeroRotator, SpawnParams);
					if (SpawnArrowDecalActor)
					{
						SpawnArrowDecalActor->SetReplicates(true);
						SpawnArrowDecalActor->SetDecalLength(10); // 예시: 화살표 3개
					}
				}
				break;
			}
		}
	}

	Multicast_PlayEngineSound();
	
	// 시동 소리 재생 후 일정 시간 뒤 차량 실제 생성
	GetWorldTimerManager().ClearTimer(EngineDelayTimerHandle);
	GetWorldTimerManager().SetTimer(EngineDelayTimerHandle, this, &ACarSpawnManager::SpawnCar, EngineStartDelay, false);
}

void ACarSpawnManager::CarStart()
{
	CarState = true;
}

void ACarSpawnManager::PlayDriveSound()
{
	if (DriveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DriveSound, GetActorLocation(),
			1.0f, // VolumeMultiplier
			1.0f, // PitchMultiplier
			0.0f, // StartTime
			SoundAttenuationSettings // 거리 감쇠 적용
		);
	}
}


void ACarSpawnManager::Multicast_PlayEngineSound_Implementation()
{
	if (EngineStartSound && CarState)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EngineStartSound,	GetActorLocation(),
			1.0f,
			1.0f,
			0.0f,
			SoundAttenuationSettings
		);
		
		GetWorldTimerManager().SetTimer(DriveSoundTimerHandle, this, &ACarSpawnManager::PlayDriveSound, 2.f, false);
	}
}
