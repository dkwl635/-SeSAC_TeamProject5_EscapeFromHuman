// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowItemSpawner.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API AThrowItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick ( float DeltaSeconds ) override;
protected:

	UPROPERTY ( EditDefaultsOnly )
	class USphereComponent* SphereComp;

	UPROPERTY ( EditDefaultsOnly )
	class UStaticMeshComponent* MeshComp;

	UPROPERTY ( EditDefaultsOnly )
	class USceneComponent* ItemSpawnPoint;

	UPROPERTY ( EditDefaultsOnly )
	class UNiagaraComponent* NS_Spawner;

	UPROPERTY ( EditDefaultsOnly )
	TArray<TSubclassOf<class AThrowItem>> Items;

	UPROPERTY ( EditDefaultsOnly )
	class AThrowItem* SpawnedItem;

	UPROPERTY ( EditDefaultsOnly )
	float SpawnerDelay = 15.0f;

	UPROPERTY ()
	class AEFHGameState* GS;

	float ElapsedTime = 0.0f;

	UPROPERTY ( EditAnywhere )
	bool bTutorial = false;
protected:
	void SpawnItem ();
	void UsingItem ();
	void ClearItem ();


	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ShowEffect ( bool bShow );


	FTimerHandle SpawnerTimerHandle;

};
