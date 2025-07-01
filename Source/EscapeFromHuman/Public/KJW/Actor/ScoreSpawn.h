// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreSpawn.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API AScoreSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	void StartSpawnScore ();
	void SpawnScore ();
	void StopSpawnScore ();
	void ReturnScore ( class AScoreActor* ScoreActor );
	FVector GetSpawnPointRandeom ();
private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AScoreActor> ScoreActorClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> Points;

	UPROPERTY ( EditDefaultsOnly )
	float ScoreSpawnTimer = 30.f;
	UPROPERTY ( EditDefaultsOnly )
	int32 ScoreSpawnCount = 1;

	UPROPERTY()
	class AEFHGameState* GS;

	UPROPERTY()
	TArray<class AScoreActor*> SpawnedScore;


	FTimerHandle SpawnTimerHandle;
};
