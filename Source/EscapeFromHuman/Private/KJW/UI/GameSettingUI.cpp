// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/GameSettingUI.h"

#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"
#include "KJW/Game/EFHGameInstance.h"


void UGameSettingUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	Slider_Master->OnValueChanged.AddDynamic ( this , &ThisClass::ChangeValueMaster );
	Slider_BGM->OnValueChanged.AddDynamic ( this , &ThisClass::ChangeValueBGM );
	Slider_SFX->OnValueChanged.AddDynamic ( this , &ThisClass::ChangeValueSFX );

	Btn_Back->OnClicked.AddDynamic ( this , &ThisClass::Back );
	Btn_Exit->OnClicked.AddDynamic ( this , &ThisClass::Exit );

	Slider_Master->SetValue ( 50.0f );
	Slider_BGM->SetValue ( 50.0f );
	Slider_SFX->SetValue ( 50.0f );
}

void UGameSettingUI::ChangeValueMaster ( float Value )
{
	float masterValue = Value;

	float percent = Value / MaxValue;

	ProgressBar_Master->SetPercent ( percent );

	if (Value <= 0.0f)
	{
		Img_Master->SetBrushFromTexture ( Sound_Icon[0] );
	}
	else
	{
		Img_Master->SetBrushFromTexture ( Sound_Icon[1] );
	}

	SoundMix->SoundClassEffects[0].VolumeAdjuster = percent;
	UGameplayStatics::PushSoundMixModifier ( GetWorld () , SoundMix );
}

void UGameSettingUI::ChangeValueBGM ( float Value )
{
	float BGMValue = Value;

	float percent = Value / MaxValue;

	ProgressBar_BGM->SetPercent ( percent );

	if (Value <= 0.0f)
	{
		Img_BGM->SetBrushFromTexture ( Sound_Icon[0] );
	}
	else
	{
		Img_BGM->SetBrushFromTexture ( Sound_Icon[1] );
	}

	SoundMix->SoundClassEffects[1].VolumeAdjuster = percent;
	UGameplayStatics::PushSoundMixModifier ( GetWorld () , SoundMix );
}

void UGameSettingUI::ChangeValueSFX ( float Value )
{
	float SFXValue = Value;

	float percent = Value / MaxValue;

	ProgressBar_SFX->SetPercent ( percent );

	if (Value <= 0.0f)
	{
		Img_SFX->SetBrushFromTexture ( Sound_Icon[0] );
	}
	else
	{
		Img_SFX->SetBrushFromTexture ( Sound_Icon[1] );
	}
	SoundMix->SoundClassEffects[2].VolumeAdjuster = percent;
	UGameplayStatics::PushSoundMixModifier ( GetWorld () , SoundMix );
}

void UGameSettingUI::Back ()
{
	this->SetVisibility ( ESlateVisibility::Collapsed );
}

void UGameSettingUI::Exit ()
{
	UEFHGameInstance* MyGameInstance = Cast<UEFHGameInstance> ( GetWorld ()->GetGameInstance () );
	if (MyGameInstance)
	{
		MyGameInstance->ExitSession ();
	}
}

