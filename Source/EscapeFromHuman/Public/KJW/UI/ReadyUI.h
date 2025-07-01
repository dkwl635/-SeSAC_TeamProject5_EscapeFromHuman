// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/UI/EFHBaseUI.h"
#include "ReadyUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UReadyUI : public UEFHBaseUI
{
	GENERATED_BODY()
	
protected:
	UPROPERTY( Transient , meta = (BindWidgetAnim) , BlueprintReadOnly )
	class UWidgetAnimation* Widgetanim_In;
	UPROPERTY( Transient , meta = (BindWidgetAnim) , BlueprintReadOnly )
	class UWidgetAnimation* Widgetanim_Out;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* KeyInfo;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_WaitSecend;


	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_Flag;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_Team;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_Ready;
protected:
	UPROPERTY ( EditDefaultsOnly )
	TArray<UTexture2D*> Flag_Sprites;

protected:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;

protected:
	virtual void BindToGameState ( class AEFHGameState* newGS ) override;
	virtual void BindToPlayerState ( class AEFHPlayerState* newPS ) override;
	void GameReadyEvent ();

	void SetWatingTimer ();
	void TeamFlag ();

	void OnReadyEvent ();
};
