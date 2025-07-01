// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "AnimalMainUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UAnimalMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;

	

public:
	void BindToPlayerState ( class AEFHPlayerState* newPS );
	void BindToGameState ( class AEFHGameState* newGS );

	void ShowUI ( FGameplayTag UITag );
	void HideUI ( FGameplayTag UITag );
	void SetUI ();
	void ShowSettingUI ();
	void ShowGameInfoUI ();
	void PlayLevelSequence ();
	void StopLevelSequence ();
protected :

	UPROPERTY ( meta = (BindWidget) )
	class UCanvasPanel* MainCanvas;


	UPROPERTY()
	TMap<FGameplayTag , class UEFHBaseUI*> UIs;

	UPROPERTY ( meta = (BindWidget) )
	class UEFHBaseUI* SettingUI;

	UPROPERTY ()
	class AEFHPlayerState* PS;
	UPROPERTY ()
	class AEFHGameState* GS;

	UPROPERTY ( EditDefaultsOnly )
	class ULevelSequence* IntroSequence;

	UPROPERTY ()
	class ALevelSequenceActor* SequenceActor;

	UPROPERTY ()
	class ULevelSequencePlayer* SequencePlayer;

protected:
	void GameReadyEvent ();
	void GameStartEvent ();
	void GameEndEvent ();




}; 
