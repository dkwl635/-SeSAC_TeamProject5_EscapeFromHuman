// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarActor.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API ACarActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* CarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	FVector startPos;
	FVector currentPos;

	UPROPERTY(Replicated)
	float currentTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovingSpeed = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovingTime = 5.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayTime;
	
	UFUNCTION()
	void HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DrivingSound;

	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_PlayDriveSound();


};
