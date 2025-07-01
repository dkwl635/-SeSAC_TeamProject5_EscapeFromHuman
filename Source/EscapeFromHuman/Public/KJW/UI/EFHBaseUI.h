// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KJW/UI/AnimalMainUI.h"
#include "GameplayTagContainer.h"
#include "EFHBaseUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UEFHBaseUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void BindToPlayerState ( class AEFHPlayerState* newPS ) { PS = newPS; }
	virtual void BindToGameState ( class AEFHGameState* newGS ) { GS = newGS;  }
	void SetMainUI ( UAnimalMainUI* newMainUI ) { MainUI = newMainUI; }

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "UI" )
	FGameplayTag WidgetTag;
protected:
	UPROPERTY ()
	class AEFHPlayerState* PS;
	UPROPERTY ()
	class AEFHGameState* GS;

	UPROPERTY ()
	UAnimalMainUI* MainUI;

};
