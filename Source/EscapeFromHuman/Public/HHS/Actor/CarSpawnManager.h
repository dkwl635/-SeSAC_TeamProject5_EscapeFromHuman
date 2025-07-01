// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrowDecalActor.h"
#include "GameFramework/Actor.h"
#include "HHS/Actor/CarActor.h"
#include "CarSpawnManager.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API ACarSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<ACarActor>> CarClass;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	class ACarActor* CarActor;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnInterval; // 차량 생성 주기

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float StartTime; // 게임 시작 후 언제부터 움직이기를 시작할 지
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* SpawnArrow;

	FTimerHandle SpawnTimerHandle;
	
	UFUNCTION()
	void SpawnCar();

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* EngineStartSound;

	UPROPERTY(EditAnywhere, Category="Sound")
	float EngineStartDelay = 2.0;

	FTimerHandle EngineDelayTimerHandle;

	void TrySpawnCarWithSound();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayEngineSound();

	UPROPERTY(EditDefaultsOnly, Category = "Car")
	TSubclassOf<AArrowDecalActor> ArrowDecalClass;

	UPROPERTY()
	AArrowDecalActor* SpawnArrowDecalActor;

	UPROPERTY()
	bool CarState = false;

	UFUNCTION()
	void CarStart();

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DriveSound;

	FTimerHandle DriveSoundTimerHandle;

	UFUNCTION()
	void PlayDriveSound();

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundAttenuation* SoundAttenuationSettings;

};
