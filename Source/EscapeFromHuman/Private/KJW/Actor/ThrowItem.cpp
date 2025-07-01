// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/ThrowItem.h"
#include "EscapeFromHuman.h"
#include "Components/SphereComponent.h"

#include "KJW/ActorComponent/ItemComponent.h"
#include "KJW/Game/EFHGameState.h"
#include "LJW/IPlayerInterAction.h"
#include "LJW/AnimalBase.h"

// Sets default values
AThrowItem::AThrowItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	SetRootComponent ( SphereComp );
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	MeshComp->SetupAttachment ( GetRootComponent() );

	SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	bReplicates = true;
	SetReplicateMovement ( true );

}


// Called when the game starts or when spawned
void AThrowItem::BeginPlay()
{
	Super::BeginPlay();

	
	GS = GetWorld ()->GetGameState<AEFHGameState>();


	SphereComp->OnComponentBeginOverlap.AddDynamic ( this , &ThisClass::OnOverlapBeginComponent );
}

void AThrowItem::Tick ( float DeltaSeconds )
{
	Super::Tick ( DeltaSeconds );	
}

void AThrowItem::AddItem ( AAnimalBase* Target )
{
	if (GetOwner())
	{
		return;
	}

	//아이템 등록 성공시
	if (Target->ItemComp->PickupThrowItem ( this ) != -1)
	{
		SphereComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
		
		if (OnAddItem.IsBound ())OnAddItem.Execute ();
	}
}

void AThrowItem::OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	

}


void AThrowItem::ShowItem ( bool bShow )
{
	ServerRPC_ShowItem ( bShow );
}

void AThrowItem::ShowItem_Internal ( bool bShow )
{
	SphereComp->SetVisibility ( bShow );
	MeshComp->SetVisibility ( bShow );

	if (bShow)
	{
		SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	}
	else
	{
		SphereComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	}

}


void AThrowItem::ItemTrigger ( float DeltaTime )
{

}


#pragma region Network

void AThrowItem::ServerRPC_ShowItem_Implementation ( bool bShow )
{
	MulticastRPC_ShowItem ( bShow );
}

void AThrowItem::MulticastRPC_ShowItem_Implementation ( bool bShow )
{
	ShowItem_Internal ( bShow );
}

#pragma endregion



