// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/KJWTestPawn.h"
#include "KJW/ActorComponent/ItemComponent.h"
#include "KJW/Actor/ThrowItem.h"
#include "Kismet/GameplayStatics.h"
#include "KJW/UI/AnimalUI.h"

// Sets default values
AKJWTestPawn::AKJWTestPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ItemComp = CreateDefaultSubobject<UItemComponent> ( TEXT ( "ItemComp" ) );
}

// Called when the game starts or when spawned
void AKJWTestPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (AnimalUIClass)
	{
		UAnimalUI* UI = CreateWidget<UAnimalUI> ( GetWorld() , AnimalUIClass);
		if (UI)
		{
			UI->AddToViewport ();
		}
	}

}

// Called every frame
void AKJWTestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKJWTestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKJWTestPawn::StartItem ()
{
	if (!ItemComp->GetCurrentItem ())
	{
		AThrowItem* item = GetWorld ()->SpawnActor<AThrowItem> ( ItemClass , FVector ( 200.f , 200.f , 0.0f ) , FRotator::ZeroRotator );
		ItemComp->AddItem ( item );
	}

	ItemComp->ItemStarted ();
}

void AKJWTestPawn::CompletItem ()
{
	ItemComp->ItemCompleted ();
}

void AKJWTestPawn::CancelItem ()
{
	ItemComp->ItemCancel ();
}

