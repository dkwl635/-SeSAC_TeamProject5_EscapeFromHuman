// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Actor/TrapItem.h"
#include "KJW/Game/EFHGameState.h"
#include "EscapeFromHuman.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "LJW/AnimalBase.h"
#include "LJW/IPlayerInterAction.h"


ATrapItem::ATrapItem ()
{

	TrapMeshComp = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("TrapMeshComp"));
	TrapMeshComp->SetupAttachment ( GetRootComponent () );

	
	//TrapMeshComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	TrapMeshComp->SetVisibility ( false );
	TrapMeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	
	ThrowType = EThrowType::Trap;
	
}

void ATrapItem::Tick ( float DeltaSeconds )
{
	Super::Tick ( DeltaSeconds );
}

void ATrapItem::ItemUse ()
{
	Super::ItemUse ();

	AActor* ownerActor =  GetOwner ();
	if (ownerActor)
	{
		FVector pos = ownerActor->GetActorLocation ();

		SetActorLocationAndRotation ( FVector ( pos.X , pos.Y , 25.0f ) , FQuat::Identity );
		ServerRPC_ShowTrap ( true );
	
		TrapMeshComp->OnComponentBeginOverlap.AddDynamic ( this , &ThisClass::OnOverlapBeginComponent );
		
		GetWorldTimerManager ().SetTimer ( DetroyTimer , FTimerDelegate::CreateLambda ( [this] {
			HitDestoryEvent ();
			} ) , LifeSpan , false );
	}

}

void ATrapItem::OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if (!GetOwner ())
	{
		AAnimalBase* animal = Cast<AAnimalBase> ( OtherActor );
		if (animal)
		{
			if (HasAuthority ())
			{
				AddItem ( animal );
			}

		}
		return;
	}

	if (GetOwner () == OtherActor) return;

	if (HasAuthority ())
	{
		IIPlayerInterAction* IPlayerInterAction = Cast<IIPlayerInterAction> ( OtherActor );
		if (IPlayerInterAction)
		{
			IPlayerInterAction->ThrowItem ( ThrowType );

			if (DetroyTimer.IsValid ())
			{
				GetWorldTimerManager ().ClearTimer ( DetroyTimer );
			}


			GetWorldTimerManager ().SetTimer ( DetroyTimer , FTimerDelegate::CreateLambda ( [&]
				{
					HitDestoryEvent ();
				} ) , 2.5f , false );
			
			MulticastRPC_HitAnimal ();
		}
	}

}



void ATrapItem::ShowTrap_Internal ( bool bShow )
{
	MeshComp->SetVisibility ( false );

	APawn* animal = Cast<APawn> ( GetOwner() );

	if (GetWorld ()->GetFirstPlayerController () == animal->GetController())
	{
		TrapMeshComp->SetVisibility ( true );
	}
	else
	{
		TrapMeshComp->SetVisibility ( false );
	}
}

void ATrapItem::HitDestoryEvent ()
{
	if (GS)
		GS->
		DestroyThrowItem ( this );
	else
		Destroy ();
}

void ATrapItem::MulticastRPC_HitAnimal_Implementation ()
{
	TrapMeshComp->SetVisibility ( true );
}

void ATrapItem::ServerRPC_ShowTrap_Implementation ( bool bShow )
{
	TrapMeshComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	
	MulticastRPC_ShowTrap ( bShow );
}

void ATrapItem::MulticastRPC_ShowTrap_Implementation ( bool bShow )
{
	ShowTrap_Internal ( bShow );
}
