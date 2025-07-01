// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimalBase.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API APlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Called when a player successfully logs in
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	// The Animal pawn class to spawn for players
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AAnimalBase> AnimalClass;
};
