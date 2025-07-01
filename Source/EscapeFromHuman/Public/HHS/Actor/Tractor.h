// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Tractor.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API ATractor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATractor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// === 이동/사운드/충돌 ===
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovingSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovingTime = 5.0f;

	UPROPERTY(Replicated)
	float currentTime;

	FVector startPos;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* DrivingSound;

	UFUNCTION()
	void HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDriveSound();

	// === 바퀴 회전 ===
	FVector PreviousLocation;

	UPROPERTY(EditAnywhere, Category = "Wheels")
	float WheelRadius = 30.0f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FrontLeftWheel;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FrontRightWheel;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RearLeftWheel;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RearRightWheel;

};
