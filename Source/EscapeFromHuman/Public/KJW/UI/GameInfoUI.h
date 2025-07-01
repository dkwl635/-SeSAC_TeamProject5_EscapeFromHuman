// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/UI/EFHBaseUI.h"
#include "GameInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UGameInfoUI : public UEFHBaseUI
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* Horizontal_BtnBox;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Back;
	UPROPERTY()
	TArray<class UButton*> Btns;

	//0 Select, 1 Select
	UPROPERTY ( EditDefaultsOnly )
	TArray<UTexture2D*> Frame_Sprites;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* GameInfoSwitcher;

protected:
	virtual void NativeConstruct () override;
		
	UFUNCTION()
	void ShowGameInfo ( );
	UFUNCTION()
	void ShowKeyInfo ( );
	UFUNCTION()
	void ShowAnimalInfo ( );
	UFUNCTION()
	void ShowHunterInfo ( );
	UFUNCTION()
	void ShowItemInfo ( );
	UFUNCTION()
	void Back ();

	void SelectButton ( UButton* Button );
	void AllUnselectButton ( UButton* IgnoreButton );
	void UnselectButton ( UButton* Button );
	void SetButtonStyle ( UButton* Button , UTexture2D* Frame);
};
