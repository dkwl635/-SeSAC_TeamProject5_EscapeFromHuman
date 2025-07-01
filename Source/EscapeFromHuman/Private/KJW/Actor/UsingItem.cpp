// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Actor/UsingItem.h"
#include "Components/SphereComponent.h"
#include "EscapeFromHuman.h"
#include "LJW/IPlayerInterAction.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AUsingItem::AUsingItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement ( true );

	SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	SetRootComponent ( SphereComp );

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	MeshComp->SetupAttachment ( GetRootComponent () );

	SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );

	MeshComp->CastShadow = false;
	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
}

void AUsingItem::BeginPlay ()
{
	Super::BeginPlay ();

	SphereComp->OnComponentBeginOverlap.AddDynamic ( this , &AUsingItem::OnOverlapBeginComponent );

}

void AUsingItem::OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{

	//PRINTLOG ( TEXT ( "OnOverlapBeginComponent , %s" ) , *OtherActor->GetActorLabel () );


	IIPlayerInterAction* IPlayerInterAction = Cast<IIPlayerInterAction> ( OtherActor );
	if (IPlayerInterAction)
	{
		if (HasAuthority ())
		{
			IPlayerInterAction->BuffItem ( UsingType );
			if (OnUsingItem.IsBound ())OnUsingItem.Execute ();
			Destroy ();
		}
		//SetOwner(OtherActor);
		//ServerRPC_RequestDropEffect ();
		SpawnDropEffect ();
	}

}

void AUsingItem::SpawnDropEffect ()
{
	if (NS_DropEffect)
	{
		FVector pos = GetActorLocation ();
		pos.Z = 0.0f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation (
			GetWorld () ,
			NS_DropEffect ,
			pos
		);
	}
}


void AUsingItem::ServerRPC_RequestDropEffect_Implementation()
{
	MultiRPC_SpawnDropEffect();
}

void AUsingItem::MultiRPC_SpawnDropEffect_Implementation()
{
	SpawnDropEffect ();
}




