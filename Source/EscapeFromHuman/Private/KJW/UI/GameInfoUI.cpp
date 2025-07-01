// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/UI/GameInfoUI.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"


void UGameInfoUI::NativeConstruct ()
{

	Super::NativeConstruct ();

	int32 btnCount = Horizontal_BtnBox->GetChildrenCount ();
	for (int32 i = 0; i < btnCount; i++)
	{
		UButton* btn = Cast<UButton> ( Horizontal_BtnBox->GetChildAt ( i ) );
		if (btn)
		{
			Btns.Add ( btn );
		}
	}

	Btns[0]->OnClicked.AddDynamic(this ,&ThisClass::ShowGameInfo );
	Btns[1]->OnClicked.AddDynamic(this ,&ThisClass::ShowKeyInfo );
	Btns[2]->OnClicked.AddDynamic(this ,&ThisClass::ShowAnimalInfo );
	Btns[3]->OnClicked.AddDynamic(this ,&ThisClass::ShowHunterInfo );
	Btns[4]->OnClicked.AddDynamic(this ,&ThisClass::ShowItemInfo );
	
	Btn_Back->OnClicked.AddDynamic ( this , &ThisClass::Back );

	SelectButton ( Btns[0] );
	GameInfoSwitcher->SetActiveWidgetIndex ( 0 );
}

void UGameInfoUI::ShowGameInfo ()
{
	SelectButton ( Btns[0] );
	GameInfoSwitcher->SetActiveWidgetIndex ( 0 );
}
	
void UGameInfoUI::ShowKeyInfo ()
{
	SelectButton ( Btns[1] );
	GameInfoSwitcher->SetActiveWidgetIndex ( 1 );
}

void UGameInfoUI::ShowAnimalInfo ()
{
	SelectButton ( Btns[2] );
	GameInfoSwitcher->SetActiveWidgetIndex ( 2 );
}

void UGameInfoUI::ShowHunterInfo ()
{
	SelectButton ( Btns[3] );
	GameInfoSwitcher->SetActiveWidgetIndex ( 3 );
}

void UGameInfoUI::ShowItemInfo ()
{
	SelectButton ( Btns[4] );
	GameInfoSwitcher->SetActiveWidgetIndex ( 4 );
}

void UGameInfoUI::Back ()
{
	this->SetVisibility ( ESlateVisibility::Collapsed );
}

void UGameInfoUI::SelectButton ( UButton* Button )
{
	SetButtonStyle ( Button , Frame_Sprites[0] );
	AllUnselectButton ( Button );
}

void UGameInfoUI::AllUnselectButton ( UButton* IgnoreButton )
{
	for (int32 i = 0; i < Btns.Num (); i++)
	{
		if (Btns[i] != IgnoreButton)
			UnselectButton ( Btns[i] );
	}
}


void UGameInfoUI::UnselectButton ( UButton* Button )
{
	SetButtonStyle ( Button , Frame_Sprites[1] );
}

void UGameInfoUI::SetButtonStyle ( UButton* Button , UTexture2D* Frame )
{
	// 기존 스타일 복사
	FButtonStyle NewStyle = Button->WidgetStyle;

	// 브러시 생성
	FSlateBrush Brush;
	Brush.SetResourceObject ( Frame );
	Brush.ImageSize = FVector2D ( 300.f , 300.f ); // 원하는 크기
	Brush.DrawAs = ESlateBrushDrawType::Box;
	Brush.Margin = FMargin ( 0.3f , 0.3f , 0.3f , 0.1f );

	// 버튼 상태별로 이미지 설정
	NewStyle.SetNormal ( Brush );
	NewStyle.SetHovered ( Brush );
	NewStyle.SetPressed ( Brush );

	// 스타일 적용
	Button->SetStyle ( NewStyle );
}
