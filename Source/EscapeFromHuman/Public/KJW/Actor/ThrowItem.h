// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KJW/ItemType.h"
#include "ThrowItem.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API AThrowItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowItem();
public:
	
	void ShowItem ( bool bShow );
	//아이템 던지기
	virtual void ItemUse () {};
	void ItemTrigger ( float DeltaTime ) ;

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay () override;
	virtual void Tick ( float DeltaSeconds ) override;

protected:	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;

	UPROPERTY ( EditAnywhere )
	EThrowType ThrowType;

	UPROPERTY()
	class AEFHGameState* GS;
public:
	UPROPERTY ( EditAnywhere )
	UTexture2D* ItemIcon;

	FSimpleDelegate OnAddItem;

protected:
	UFUNCTION ()
	void AddItem ( class AAnimalBase* Target );

	UFUNCTION()
	virtual	void OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );


protected:
	virtual void ShowItem_Internal ( bool bShow );
#pragma region  NetWork
protected:
	UFUNCTION ( Server , Reliable )
	void ServerRPC_ShowItem ( bool bShow );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ShowItem ( bool bShow );

#pragma endregion


};
