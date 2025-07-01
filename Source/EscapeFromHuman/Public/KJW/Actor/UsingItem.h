// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KJW/ItemType.h"
#include "UsingItem.generated.h"



UCLASS()
class ESCAPEFROMHUMAN_API AUsingItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsingItem();


protected :
	UPROPERTY ( EditDefaultsOnly )
	UStaticMeshComponent* MeshComp;

	UPROPERTY ( EditDefaultsOnly )
	class USphereComponent* SphereComp;

	UPROPERTY ( EditDefaultsOnly )
	class UNiagaraSystem* NS_DropEffect;



	UPROPERTY ( EditDefaultsOnly )
	EUSINGType UsingType = EUSINGType::MAX;

public:
	virtual void BeginPlay () override;

	FSimpleDelegate OnUsingItem;

private:
	UFUNCTION ()
	void OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );

	UFUNCTION ( Server , Reliable )
	void ServerRPC_RequestDropEffect ();

	UFUNCTION ( NetMulticast , Reliable )
	void MultiRPC_SpawnDropEffect ();

	void SpawnDropEffect();
	
};
