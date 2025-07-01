// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/UI/EFHBaseUI.h"
#include "AnimalStartUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UAnimalStartUI : public UEFHBaseUI
{
	GENERATED_BODY()


protected:

	UPROPERTY ( meta = (BindWidget) )
	class UCanvasPanel* MainCanvas;

	UPROPERTY ( meta = (BindWidget) )
	class UButton* Btn_GameInfo;

	UPROPERTY ( meta = (BindWidget) )
	class UButton* Btn_Setting;
	
	//UPROPERTY ( meta = (BindWidget) )
	//class UGameInfoUI* GameInfoUI;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_AnimalPortrait1;
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_AnimalPortrait2;
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_AnimalPortrait3;


	UPROPERTY(EditDefaultsOnly)
	FVector2D OrginBoxSize = FVector2D ( 300.0f , 700.0f );
	UPROPERTY(EditDefaultsOnly)
	FVector2D SelectBoxSize = FVector2D ( 400.0f , 800.0f );
	
	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_Flag;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_Team;
	UPROPERTY ( EditDefaultsOnly )
	TArray<UTexture2D*> Flag_Sprites;

	UPROPERTY ( meta = (BindWidget) )
	UWidget* PlayerFlag;
public :
	virtual void BindToPlayerState ( class AEFHPlayerState* newPS ) override;
	virtual void BindToGameState ( class AEFHGameState* newGS ) override;
	void SelectAnimal ( int32 BoxIndex );
	void StartAnimal ();
protected:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;



protected:

	UPROPERTY()
	TArray<class UAnimalPortraitBox*> Boxes;
	UPROPERTY ()
	TArray<class USizeBox*> SizeBoxes;
	UPROPERTY()
	class UAnimalPortraitBox* SelectBox;


protected:  
	void SetSelectionUI ();
	UFUNCTION()
	void ShowSettingUI ();
	UFUNCTION()
	void ShowGameInfo ();
};
