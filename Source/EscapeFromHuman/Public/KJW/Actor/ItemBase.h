// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KJW/ItemType.h"
#include "ItemBase.generated.h"



UCLASS()
class ESCAPEFROMHUMAN_API AItemBase : public AActor
{
	GENERATED_BODY()
	

public:
	UPROPERTY( VisibleAnywhere )
	EItemType ItemType = EItemType::MAX;


protected:
	AItemBase ();

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
public:
	void SetCollision ( bool bShow );
	void ShowItem ( bool bShow );
	virtual void ItemTrigger ( float DeltaTime ) {};

	//들고 있는 아이템 버리기
	virtual void DropItem ( FVector  DropPos);
	bool IsDropItem () const { return bDrop; }
protected:
	UPROPERTY ( VisibleAnywhere , Replicated)
	bool bDrop = false;


	
#pragma region  NetWork
protected:

	virtual void SetCollision_Internal (bool bShow){};
	virtual void ShowItem_Internal (bool bShow){};

private:
	UFUNCTION ( Server , Reliable )
	void ServerRPC_SetCollision ( bool bShow );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_Shouting ( bool bShow );

	UFUNCTION ( Server , Reliable )
	void ServerRPC_ShowItem ( bool bShow );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ShowItem ( bool bShow );


#pragma endregion
};
