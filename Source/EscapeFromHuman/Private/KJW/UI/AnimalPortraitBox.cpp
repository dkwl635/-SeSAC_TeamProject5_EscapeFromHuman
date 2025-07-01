// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/AnimalPortraitBox.h"

#include "LJW/AnimalBase.h"
#include "LJW/AnimalDataAsset.h"
#include "KJW/UI/AnimalStartUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Slate/SlateBrushAsset.h"
#include "Engine/Texture2D.h"

void UAnimalPortraitBox::NativeConstruct ()
{
	Super::NativeConstruct ();

	Btn_Select->OnClicked.AddDynamic ( this , &ThisClass::SelectAnimal );
	Btn_Start->OnClicked.AddDynamic ( this , &ThisClass::StartAnimal );
}



void UAnimalPortraitBox::SetAnimalData ( TSubclassOf<AAnimalBase> AnimalClass )
{
	SlotAnimal = AnimalClass;

	FAnimalInfo& data = AnimalClass.GetDefaultObject ()->AnimalData->AnimalStats;
	Img_Animal->SetBrushFromTexture ( data.AnimalTexture );
	Img_Animal->SetVisibility ( ESlateVisibility::SelfHitTestInvisible );

	TextBlock_Name->SetText ( data.AnimalName );
	Img_Skill->SetBrushFromTexture ( data.Skillicon );
	TextBlock_SkillName->SetText ( data.SkillName );
	TextBlock_SkillInfo->SetText ( data.Skillinfo );

}

void UAnimalPortraitBox::SelectAnimal ()
{
	if (bOpen) return;

	bOpen = true;
	//ToggleButton ( true );

	// 기존 스타일 복사
	FButtonStyle NewStyle = Btn_Select->WidgetStyle;

	// 브러시 생성
	FSlateBrush Brush;
	Brush.SetResourceObject ( Frame_Sprites[1]);
	//Brush.ImageSize = FVector2D ( 300.f , 700.f ); // 원하는 크기
	Brush.DrawAs = ESlateBrushDrawType::Box;
	Brush.Margin = FMargin ( 0.2f , 0.4f );

	// 버튼 상태별로 이미지 설정
	NewStyle.SetNormal ( Brush );
	NewStyle.SetHovered ( Brush );
	NewStyle.SetPressed ( Brush );

	// 스타일 적용
	Btn_Select->SetStyle ( NewStyle );
	



	if (StartUI)
	{
		StartUI->SelectAnimal ( AnimalIndex );

	}

	Btn_Start->SetVisibility ( ESlateVisibility::Visible );
}

void UAnimalPortraitBox::StartAnimal ()
{
	if (StartUI)
	{
		StartUI->StartAnimal ();
	}

	Btn_Start->SetVisibility ( ESlateVisibility::Collapsed );
}

void UAnimalPortraitBox::UnselectAnimal ()
{

	FButtonStyle NewStyle = Btn_Select->WidgetStyle;

	// 브러시 생성
	FSlateBrush Brush;
	Brush.SetResourceObject ( Frame_Sprites[0] );
	Brush.ImageSize = FVector2D ( 300.f , 700.f ); // 원하는 크기
	Brush.DrawAs = ESlateBrushDrawType::Box;
	Brush.Margin = FMargin ( 0.2f , 0.4f );

	// 버튼 상태별로 이미지 설정
	NewStyle.SetNormal ( Brush );
	NewStyle.SetHovered ( Brush );
	NewStyle.SetPressed ( Brush );

	// 스타일 적용
	Btn_Select->SetStyle ( NewStyle );

	if (bOpen)
		ToggleButton ( false );



	bOpen = false;

	Btn_Start->SetVisibility ( ESlateVisibility::Collapsed );
}

UAnimalDataAsset* UAnimalPortraitBox::GetAnimalData ()
{
	if (!SlotAnimal) return nullptr;

	return SlotAnimal.GetDefaultObject ()->AnimalData;
}

void UAnimalPortraitBox::SetPlayerIcon (int32 TeamIndex)
{
	

}

void UAnimalPortraitBox::ToggleButton ( bool bShouldOpen )
{
	
	//float currentTime = GetAnimationCurrentTime ( OpenButtonAnim );

	//EUMGSequencePlayMode::Type PlayMode = bShouldOpen ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse;
	//PlayAnimation ( OpenButtonAnim , currentTime , 1 , PlayMode );

}
