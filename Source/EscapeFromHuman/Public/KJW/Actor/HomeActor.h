// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomeActor.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API AHomeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY ( EditAnywhere )
	int32 Index = 0;

protected:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly)
	class AAnimalBase* MyTeamAnimal = nullptr;

	UPROPERTY()
	class AEFHPlayerState* PS;
	UPROPERTY()
	TArray<class AScoreActor*> MyScores;

private:
	UFUNCTION()
	void OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );

	UFUNCTION()
	void OnOverlapEndinComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32  OtherBodyIndex );


	FVector GetHomePointRand ();


};
