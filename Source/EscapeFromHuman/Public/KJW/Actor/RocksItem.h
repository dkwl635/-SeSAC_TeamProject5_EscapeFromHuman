// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/Actor/ThrowItem.h"
#include "RocksItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API ARocksItem : public AThrowItem
{
	GENERATED_BODY()
	
protected:
	ARocksItem ();

	virtual void BeginPlay () override;
	virtual void Tick ( float DeltaSeconds ) override;
protected:

	UPROPERTY ( EditDefaultsOnly )
	class UNiagaraComponent* NS_ThrowTail;

	UPROPERTY ( EditDefaultsOnly )
	class UNiagaraSystem* NS_HitEffect;

public:
	virtual void ItemUse () override;
protected:
	virtual	void OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult ) override;

	void HitDestoryEvent ();
private:

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 4.0f;

	UPROPERTY ( EditAnywhere )
	float DestroyTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 3000.0f;

	UPROPERTY ( EditAnywhere )
	FVector Velocity;
 
	FTimerHandle DetroyTimer;

	bool bHit = false;
private:
	
	UFUNCTION ( Server , Reliable )
	void ServerRPC_Showtail ( bool bShow );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_Showtail ( bool bShow );


	UFUNCTION ( Server , Reliable )
	void ServerRPC_SpawnHitEffect ( );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_SpawnHitEffect (  );
};
