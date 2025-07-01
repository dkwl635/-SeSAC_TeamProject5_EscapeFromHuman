// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/ActorComponent/ItemComponent.h"
#include "EscapeFromHuman.h"
#include "KJW/ItemType.h"

#include "LJW/AnimalBase.h"
#include "KJW/Actor/ThrowItem.h"
#include "KJW/Actor/UsingItem.h"
#include "KJW/Game/EFHGameState.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstance.h"



// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ItemList.Empty ();
	ItemList.Init ( nullptr , ItemListMax );
	Lines.Empty ();
	ThrowPowerRate = 0.0f;
	
	Animal = GetOwner<AAnimalBase> ();

	Animal->OnInteractionEvent.AddUObject ( this , &UItemComponent::PickupItem );

	ProjectileNiagaraComp = Animal->ProjectileNiagaraComp;
	ProjectileNiagaraComp->Activate ();

	//FTimerHandle handle;
	//GetWorld ()->GetTimerManager ().SetTimer ( handle ,  FTimerDelegate::CreateLambda ( [&] {
	//	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector ( ProjectileNiagaraComp , TEXT ( "PointArray" ) , Lines );
	//	} ) , 0.2f , false );

	


	RingNiagaraComp = Animal->RingNiagaraComp;
	RingNiagaraComp->Activate ();
	RingNiagaraComp->SetVisibility ( false );

	GS = GetWorld ()->GetGameState<AEFHGameState> ();
	if (GS)
	{
		GS->OnReadyStateEvent.AddUObject ( this , &ThisClass::ClearItem );
	}
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	if (!Animal->IsLocallyControlled ()) return;
	/*if (bThrow)
	{
		ThrowPowerRate += DeltaTime;
		ThrowPowerRate = FMath::Clamp ( ThrowPowerRate , 0.0f , 1.0f );
	
	
	}*/

}

void UItemComponent::ItemStarted ()
{
	if (!ItemList[0]) return;
	bThrow = true;
}

void UItemComponent::ItemTrigger ( float DeltaTime )
{
	if (!ItemList[0]) return;
	if (!bThrow) return;

	//ItemList[0]->ItemTrigger ( DeltaTime );
	//ItemList[CurrentItemIndex]->SetActorLocation ( Animal->ArrowComp->GetComponentLocation () );
}

void UItemComponent::ItemCompleted ()
{
	if (!ItemList[0]) return;
	if (!bThrow) return;
	// animal->CamComp->GetForwardVector ()
	bThrow = false;
	//// animal->CamComp
	FRotator StartRot = Animal->GetActorForwardVector().Rotation ();
	ServerRPC_ItemCompleted ( 0 , ThrowPowerRate , StartRot );


	//if (!ItemList[CurrentItemIndex]) return;
	//if (!bThrow) return;

	//bThrow = false;
	//// animal->CamComp->GetForwardVector ()
	//ProjectileNiagaraComp->SetVisibility(false);
	//RingNiagaraComp->SetVisibility ( false );
	//FRotator StartRot = Animal->ArrowComp->GetForwardVector ().Rotation ();

	//ServerRPC_ItemCompleted ( CurrentItemIndex ,ThrowPowerRate , StartRot );

	//
	//if (DrawThrowTimer.IsValid ())
	//{
	//	GetWorld ()->GetTimerManager ().ClearTimer ( DrawThrowTimer );
	//}
}

void UItemComponent::ItemCancel ()
{
	if (!ItemList[0]) return;
	bThrow = false;
	ThrowPowerRate = 0.0f;

	ProjectileNiagaraComp->SetVisibility ( false );
	RingNiagaraComp->SetVisibility ( false );
	//ProjectileNiagaraComp->Deactivate ();

	if (DrawThrowTimer.IsValid ())
	{
		GetWorld ()->GetTimerManager ().ClearTimer ( DrawThrowTimer );
	}
}

void UItemComponent::DrawThrowLine (  )
{

	if (!ProjectileNiagaraComp->IsRegistered ())
	{
		return;
	}

	if (!ProjectileNiagaraComp->IsActive ())
	{
		return;
	}

	// 비워줘야한다.
	Lines.Empty ();
	// 선이 진행될 힘(방향)
	
	//FVector Velocity = GetOwner ()->GetActorForwardVector () * (ThrowDefaultPower * ThrowPowerRate);
	//FVector Velocity = Animal->CamComp->GetForwardVector () * (ThrowDefaultPower * ThrowPowerRate);
	FVector Velocity = Animal->ArrowComp->GetForwardVector () * (ThrowDefaultPower * ThrowPowerRate);
	
	// P0
	FVector Pos = Animal->ArrowComp->GetComponentLocation ();
	Lines.Add ( Pos );
	
	for (int i = 0;i < LineSmooth;i++)
	{
		FVector LastPos = Pos;
		// v = v0 + at
		Velocity += FVector::UpVector * Gravity * DrawTimedelta;
		// P = P0 + vt
		Pos += Velocity * DrawTimedelta;


		if (Pos.Z <= 0.0f)
		{
			Pos.Z = 0.0f;

			if (RingNiagaraComp->IsActive ())
				RingNiagaraComp->SetWorldLocation ( Pos );

			Lines.Add ( Pos );
			//PRINTLOG ( TEXT ( "DrawThrowLine , %f , %f" ), Pos.X , Pos.Y );

			break;
		}
		Lines.Add	( Pos );


		//DrawDebugLine ( GetWorld () , LastPos , Pos , FColor::Red , false , -1 , 0 , 1 );
	}

	//ProjectileNiagaraComp->SetVariableInt ( TEXT ( "ArcPositionCount" ) , LineSmooth );	
	//ProjectileNiagaraComp->SetEmitterEnable ( TEXT ( "PointArray" ) , false);
	
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector ( ProjectileNiagaraComp , TEXT ( "PointArray" ) , Lines );

}

void UItemComponent::ChangeItemSlot ()
{

	/*
	CurrentItemIndex = CurrentItemIndex + 1;
	if (CurrentItemIndex == ItemListMax) CurrentItemIndex = 0;
	 
	OnItemEvent.Broadcast ();
		*/
}

void UItemComponent::ClearItem ()
{
	ItemList[0] = nullptr;
	ItemList[1] = nullptr;


	OnItemEvent.Broadcast ();
}

void UItemComponent::PickupItem ( AAnimalBase* OwnerAnimal )
{

	//ServerRPC_PickupItem ();
}

int32 UItemComponent::PickupThrowItem ( AThrowItem* NewItem )
{
	int32 ItemIndex = AddItem ( NewItem );
	if (ItemIndex != -1)
	{
		if (NewItem->OnAddItem.IsBound ()) NewItem->OnAddItem.Execute ();

		FVector StartPos = Animal->ArrowComp->GetComponentLocation ();
		FRotator StartRot = Animal->ArrowComp->GetForwardVector ().Rotation ();

		NewItem->AttachToActor ( Animal , FAttachmentTransformRules::KeepWorldTransform );
		NewItem->SetActorLocationAndRotation ( StartPos , StartRot );

		ClientRPC_PickupItem ( ItemIndex , NewItem );
	}

	return ItemIndex;
}

void UItemComponent::DropItem ()
{
	if (bThrow) return;
	if (!ItemList[0]) return;

	ServerRPC_DropItem ( 0 );
}

int32 UItemComponent::AddItem ( AThrowItem* NewItem )
{
	if (!NewItem) return -1;

	int32 ItemIndex = -1;
	for (int32 i = 0; i < ItemList.Num (); ++i)
	{
		if (ItemList[i] == nullptr)
		{
			ItemList[i] = NewItem;
			ItemList[i]->ShowItem ( false );
			ItemList[i]->SetOwner ( Animal );

			PRINTLOG ( TEXT ( "AddThrowItem" ) );
			ItemIndex = i;

			break;
		}
	}

	
	return ItemIndex;
}


AThrowItem* UItemComponent::GetCurrentItem ()
{
	return ItemList[0];
}

AThrowItem* UItemComponent::GetItem ( int32 SlotIndex )
{
	if (ItemList.Num () == 0 || SlotIndex < 0 || ItemList.Num () < SlotIndex) return nullptr;

	return ItemList[SlotIndex];
}



#pragma region  네트워크 동기화
void UItemComponent::ServerRPC_PickupItem_Implementation ()
{
	if (!GS) return;

	auto items = GS->GetThrowItems ();
	if (items.Num () <= 0) return;

	FVector2D animal2DPos = (FVector2D)Animal->GetActorLocation ();

	AThrowItem* pickup = nullptr;
	for (AThrowItem* item : items)
	{
		PRINTLOG ( TEXT ( "Check , Item " ) );
		if (!item || item->GetOwner ())
			continue;
		PRINTLOG ( TEXT ( "Check , Item Name -> %s" ) , *item->GetActorLabel() );
		FVector2D item2DPos = (FVector2D)item->GetActorLocation ();

		const float RangeSquared = FMath::Square ( ItemPickupLen );

		float distSquared = FVector2D::DistSquared ( animal2DPos , item2DPos );

		if (distSquared <= RangeSquared)
		{
			//PRINTLOG ( TEXT ( "Item Dist : %f , %f" ) , distSquared , RangeSquared );
			pickup = item;
			break;
		}
	}

	if (pickup)
	{
		int32 ItemIndex = AddItem ( pickup );
		if (ItemIndex != -1)
		{
			if(pickup->OnAddItem.IsBound()) pickup->OnAddItem.Execute ();
			ClientRPC_PickupItem ( ItemIndex , pickup );
		}
	}
}
void UItemComponent::ClientRPC_PickupItem_Implementation (int32 ItemIndex, AThrowItem* NewItem )
{
	ItemList[ItemIndex] = NewItem;

	if (Animal->IsLocallyControlled ())
		OnItemEvent.Broadcast ();
}

void UItemComponent::ServerRPC_ItemStarted_Implementation ( int32 ItemIndex )
{
	bThrow = true;


	FVector StartPos = Animal->ArrowComp->GetComponentLocation ();
	//FRotator StartRot = Animal->CamComp->GetForwardVector ().Rotation ();
	FRotator StartRot = Animal->ArrowComp->GetForwardVector ().Rotation ();

	//ItemList[CurrentItemIndex]->ShowItem ( true );
	
	//ItemList[CurrentItemIndex]->SetActorLocationAndRotation ( StartPos , StartRot );
	ItemList[0]->AttachToActor ( Animal , FAttachmentTransformRules::KeepWorldTransform );
	ItemList[0]->SetActorLocationAndRotation ( StartPos , StartRot );

	MulticastRPC_ItemStarted ( ItemList[0] );

}

void UItemComponent::MulticastRPC_ItemStarted_Implementation ( AThrowItem* Item )
{
	FVector StartPos = Animal->ArrowComp->GetComponentLocation ();
	//FRotator StartRot = Animal->CamComp->GetForwardVector ().Rotation ();
	FRotator StartRot = Animal->ArrowComp->GetForwardVector ().Rotation ();

	Item->AttachToActor ( Animal , FAttachmentTransformRules::KeepWorldTransform );
	Item->SetActorLocationAndRotation ( StartPos , StartRot );
	bThrow = true;
}


void UItemComponent::ServerRPC_ItemCompleted_Implementation ( int32 ItemIndex , float ThrowRate , FRotator StartRot )
{
	AThrowItem* throwItem = ItemList[0];
	throwItem->DetachFromActor ( FDetachmentTransformRules::KeepWorldTransform);
	throwItem->SetActorRotation ( StartRot );

	throwItem->ItemUse (); // (ThrowDefaultPower * ThrowRate);
	
	ItemList[0] = nullptr;
	bThrow = false;

	if (ItemList[1])
	{
		ItemList[0] = ItemList[1];
		ItemList[1] = nullptr;
	}
	

	MulticastRPC_ItemCompleted ( 0 );
	
}

void UItemComponent::MulticastRPC_ItemCompleted_Implementation (int32 ItemIndex )
{	
	bThrow = false;

	if (!Animal->HasAuthority ())
	{
		ItemList[0] = nullptr;		
		if (ItemList[1])
		{
			ItemList[0] = ItemList[1];
			ItemList[1] = nullptr;
		}
	}
	OnItemEvent.Broadcast ();
}


void UItemComponent::ServerRPC_DropItem_Implementation ( int32 ItemIndex )
{

	if (!ItemList[0]) return;

	AThrowItem* DropItem = ItemList[0];
	///DropItem->SetActorLocationAndRotation (  , FQuat::Identity );
	DropItem->ShowItem ( true );
	DropItem->SetActorLocation ( GetOwner ()->GetActorLocation () );
	DropItem->SetOwner ( nullptr );

	ItemList[0] = nullptr;
	ClientRPC_DropItem ( 0 );
}

void UItemComponent::ClientRPC_DropItem_Implementation ( int32 ItemIndex )
{
	ItemList[ItemIndex] = nullptr;

	if (Animal->IsLocallyControlled ())
		OnItemEvent.Broadcast ();
}


#pragma endregion   네트워크 동기화
