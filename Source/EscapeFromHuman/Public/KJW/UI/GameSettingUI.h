// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/UI/EFHBaseUI.h"
#include "GameSettingUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UGameSettingUI : public UEFHBaseUI
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_Master;

	UPROPERTY ( meta = (BindWidget) )
	class UProgressBar* ProgressBar_Master;

	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_Master;

	UPROPERTY ( meta = (BindWidget) )
	class USlider* Slider_BGM;

	UPROPERTY ( meta = (BindWidget) )
	class UProgressBar* ProgressBar_BGM;

	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_BGM;

	UPROPERTY ( meta = (BindWidget) )
	class USlider* Slider_SFX;

	UPROPERTY ( meta = (BindWidget) )
	class UProgressBar* ProgressBar_SFX;

	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_SFX;

	UPROPERTY ( meta = (BindWidget) )
	class UButton* Btn_Back;

	UPROPERTY ( meta = (BindWidget) )
	class UButton* Btn_Exit;


	UPROPERTY ( EditDefaultsOnly )
	class USoundMix* SoundMix;

	// SoundClass Index == 0 Master
;
	// SoundClass Index == 1 BGN
	
	// SoundClass Index == 2 SFX


protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<UTexture2D*> Sound_Icon;

	 virtual void NativeConstruct () override;
	 float MaxValue = 100.0f;

protected:
	UFUNCTION()
	void ChangeValueMaster ( float Value );
	UFUNCTION()
	void ChangeValueBGM ( float Value );
	UFUNCTION()
	void ChangeValueSFX ( float Value );
	UFUNCTION()
	void Back ();
	
	UFUNCTION ()
	void Exit ();
};
