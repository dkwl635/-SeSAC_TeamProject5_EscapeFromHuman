// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/UI/EFHBaseUI.h"
#include "GameResultUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UGameResultUI : public UEFHBaseUI
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_BResult;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_AResult;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_TeamBScore;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_TeamAScore;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Move;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* WinEffect;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Retry;
	UPROPERTY ( Transient , meta = (BindWidgetAnim) , BlueprintReadOnly )
	class UWidgetAnimation* Widgetanim_Game;
protected:

	//1 = A , 2 = B, 3 = 무승부
	int32 GameResult = -1;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UTexture2D*> GameResult_Sprites;

	UPROPERTY()
	class UCanvasPanelSlot* EffectSlot;

protected:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;
	virtual void BindToGameState ( class AEFHGameState* newGS ) override;
	void SetResult ();

	void RotEffect (float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void SetEffect ();
	
	UFUNCTION()
	void RetryGame ();
	UFUNCTION()
	void ShowMoveText ();
};
