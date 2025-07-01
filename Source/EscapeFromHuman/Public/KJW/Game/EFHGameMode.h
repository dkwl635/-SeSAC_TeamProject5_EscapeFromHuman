// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EFHGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API AEFHGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay () override;

public:
	UFUNCTION(BlueprintCallable)
	void GameEFHStart ();

	TSubclassOf<class AAnimalBase> GetAnimalClass(int32 AnimalIndex) ;

	int32 Player = 0;

private:
	void SetGameState ();

private:
	UPROPERTY ()
	class AEFHGameState* GS;

	UPROPERTY ( EditDefaultsOnly )
	TArray<TSubclassOf<class AAnimalBase>> Animals;

};
