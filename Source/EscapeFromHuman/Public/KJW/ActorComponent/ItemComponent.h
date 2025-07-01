// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE( FSimpleMulticastDelegate );
DECLARE_MULTICAST_DELEGATE( FItemDelegate );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEFROMHUMAN_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool IsThrowing () const { return bThrow; }

	//아이템 투척 준비 -> 키 입력 시작
	UFUNCTION(BlueprintCallable)
	void ItemStarted ();
	//아이템 트리거 -> 키 누르고 있을때
	void ItemTrigger ( float DeltaTime );
	//아이템 투척 -> 키 뗐을 때
	UFUNCTION(BlueprintCallable)
	void ItemCompleted ();
	//아이템 투척 캔슬
	UFUNCTION(BlueprintCallable)
	void ItemCancel ();
	
	//아이템 투사체라인 그리기
	void DrawThrowLine (  );

	//들고 있는 아이템 다음 아이템으로 바꾸기
	void ChangeItemSlot ();
	
	//들고 있는 아이템 다음 아이템으로 바꾸기
	void ClearItem ();
	//아이템 줍기
	UFUNCTION()
	void PickupItem ( class  AAnimalBase* OwnerAnimal );

	//서버에서 만 호출 하기
	UFUNCTION ()
	int32 PickupThrowItem ( class AThrowItem* NewItem );

	//들고 있는 아이템 버리기
	void DropItem ();
	//아이템 추가
	int32 AddItem ( class AThrowItem* NewItem );
	//현제 들고 있는 아이템
	class AThrowItem* GetCurrentItem ();

	class AThrowItem* GetItem ( int32 SlotIndex );

	
public:
	FItemDelegate  OnItemEvent;


	UPROPERTY ( EditAnywhere )
	int32 ItemListMax = 2;

	UPROPERTY ( EditAnywhere )
	float ThrowPowerRate = 1.f;

protected:
	UPROPERTY()
	class AAnimalBase* Animal;

	UPROPERTY ( EditAnywhere )
	float ItemPickupLen = 100.0f;



	UPROPERTY( EditAnywhere )
	TArray<class AThrowItem*> ItemList;

	UPROPERTY ( EditAnywhere )
	bool bThrow = false;

	UPROPERTY ( EditAnywhere )
	float ThrowDefaultPower = 1500;


	//투사체 라인 그리기용
	FTimerHandle DrawThrowTimer;
	float DrawTimedelta = 0.02f;
	UPROPERTY()
	TArray<FVector> Lines;
	UPROPERTY ( EditAnywhere )
	int32 LineSmooth = 20;
	UPROPERTY ( EditAnywhere )
	float Gravity = -980.f;
	
	//투사체 라인 이펙트
	UPROPERTY ( EditAnywhere )
	class UNiagaraComponent* ProjectileNiagaraComp;
	//투사체 라인 이펙트
	UPROPERTY ( EditAnywhere )
	class UNiagaraComponent* RingNiagaraComp;

	UPROPERTY ()
	class AEFHGameState* GS;




	
#pragma region  NetWork 네트워크 동기화
private:

	UFUNCTION ( Server , Reliable )
	void ServerRPC_PickupItem (  );

	UFUNCTION ( Client , Reliable )
	void ClientRPC_PickupItem (int32 ItemIndex, AThrowItem* NewItem );

	UFUNCTION ( Server , Reliable )
	void ServerRPC_ItemStarted (int32 ItemIndex);
	
	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ItemStarted ( AThrowItem* Item );


	UFUNCTION ( Server , Reliable )
	void ServerRPC_ItemCompleted (int32 ItemIndex , float ThrowRate , FRotator StartRot );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ItemCompleted (int32 ItemIndex );

	UFUNCTION ( Server , Reliable )
	void ServerRPC_DropItem ( int32 ItemIndex );

	UFUNCTION ( Client , Reliable )
	void ClientRPC_DropItem ( int32 ItemIndex );


#pragma endregion   NetWork 네트워크 동기화

};
