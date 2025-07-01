// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/Actor/ThrowItem.h"
#include "TrapItem.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API ATrapItem : public AThrowItem
{
	GENERATED_BODY()


protected:
	ATrapItem ();

	virtual void Tick ( float DeltaSeconds ) override;
	
protected:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* TrapMeshComp;

	FTimerHandle DetroyTimer;

	UPROPERTY ( EditDefaultsOnly )
	float LifeSpan = 10.0f;
public:
virtual void ItemUse () override;

protected:
	virtual	void OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult ) override;
protected:
	virtual void ShowTrap_Internal ( bool bShow );


	void HitDestoryEvent ();

#pragma region  NetWork
private:
	UFUNCTION ( Server , Reliable )
	void ServerRPC_ShowTrap ( bool bShow );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ShowTrap ( bool bShow );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_HitAnimal ( );
#pragma endregion
	
};
