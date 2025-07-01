// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Game/EFHGameMode.h"

#include "KJW/Game/EFHGameState.h"
#include "LJW/AnimalBase.h"


void AEFHGameMode::BeginPlay ()
{
	Super::BeginPlay ();

	SetGameState ();
}

void AEFHGameMode::GameEFHStart ()
{
	if (GS)
	{
		GS->ChangeGameState ( EGameState::GAME );
	}
}

TSubclassOf<AAnimalBase> AEFHGameMode::GetAnimalClass ( int32 AnimalIndex )
{
	return Animals[AnimalIndex];
}

void AEFHGameMode::SetGameState ()
{
	GS = GetGameState<AEFHGameState> ();
	if(GS)
	GS->ChangeGameState ( EGameState::MAX );
}


