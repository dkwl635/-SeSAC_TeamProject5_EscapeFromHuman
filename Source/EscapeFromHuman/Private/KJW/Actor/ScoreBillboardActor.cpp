// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/ScoreBillboardActor.h"
#include "EscapeFromHuman.h"
#include "KJW/UI/PlayerScore.h"
#include "KJW/Game/EFHPlayerState.h"
#include "KJW/Game/EFHGameState.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"

#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
AScoreBillboardActor::AScoreBillboardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SceneComp = CreateDefaultSubobject<USceneComponent> ( TEXT ( "SceneComp"));
	SetRootComponent ( SceneComp );

	WidgetComp = CreateDefaultSubobject<UWidgetComponent> ( TEXT ("WidgetComp"));
	WidgetComp->SetupAttachment ( GetRootComponent () );
}

// Called when the game starts or when spawned
void AScoreBillboardActor::BeginPlay()
{
	Super::BeginPlay();

	PRINTLOG ( TEXT ( "BeginPlay" ) );
	WidgetComp->SetVisibility ( false );
	PlayerScore = Cast< UPlayerScore> ( WidgetComp->GetWidget () );

	GetWorld ()->GetTimerManager ().SetTimer ( SetPSHandle , this , &ThisClass::SetPlayerState, 1.0f ,true );	

}



void AScoreBillboardActor::SetPlayerState ()
{
	OwnerAnimal = Cast<AAnimalBase> ( GetAttachParentActor () );
	if (OwnerAnimal)
	{
		AEFHGameState* GS = GetWorld ()->GetGameState<AEFHGameState> ();
		if (GS)
		{
			GS->OnStartStateEvent.AddUObject ( this , &ThisClass::StartGame );
		}
	}
}

void AScoreBillboardActor::SetPlayerTempScore ()
{
	if (PlayerScore)
	{
		PlayerScore->SetScoreText ();
	}
}

void AScoreBillboardActor::StartGame ()
{
	AEFHPlayerState* PS = OwnerAnimal->GetPlayerState<AEFHPlayerState> ();
	if (PS)
	{
		PS->OnChangeScore.AddUObject ( this , &ThisClass::SetPlayerTempScore );
		PlayerScore->PS = PS;
		PlayerScore->Animal = OwnerAnimal;

		GetWorldTimerManager ().ClearTimer ( SetPSHandle );


		FTimerHandle CheckDistHandle;
		GetWorld ()->GetTimerManager ().SetTimer ( CheckDistHandle , this , &ThisClass::CheckDisanceAnima , 0.1f , true );
	}
}

void AScoreBillboardActor::CheckDisanceAnima ()
{
	if (!OwnerAnimal) return;
	

	//isDie 체크
	APlayerController* PC =	GetWorld ()->GetFirstPlayerController ();
	APawn* OtherPawn = PC->GetPawn ();
	if (OtherPawn == OwnerAnimal)
	{

		WidgetComp->SetVisibility ( true );
	}
	else
	{
		float Dist = GetDistanceTo ( OtherPawn );
		if (Dist < CheckDisLen)
		{
			WidgetComp->SetVisibility ( true );
		}
		else
		{
			WidgetComp->SetVisibility ( false );
		}
	}

	if (OwnerAnimal->StatusComp->IsDead )
	{
		WidgetComp->SetVisibility ( false );
	}
	

}

