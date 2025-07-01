// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/AnimalStartUI.h"

#include "EscapeFromHuman.h"
#include "KJW/Game/EFHPlayerController.h"
#include "KJW/Game/EFHGameState.h"
#include "KJW/Game/EFHPlayerState.h"
#include "KJW/UI/AnimalPortraitBox.h"
#include "LJW/AnimalDataAsset.h"
#include "KJW/UI/GameInfoUI.h"

#include "KJW/UIGamePlayTag.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"


void UAnimalStartUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	//Btn_GameStart->SetVisibility ( ESlateVisibility::Hidden );
	//SetSelectionUI ();
	
	Btn_Setting->OnClicked.AddDynamic ( this , &ThisClass::ShowSettingUI );
	Btn_GameInfo->OnClicked.AddDynamic ( this , &ThisClass::ShowGameInfo );
	
	//GameInfoUI->SetVisibility ( ESlateVisibility::Collapsed );
	PlayerFlag->SetVisibility ( ESlateVisibility::Collapsed );
}

void UAnimalStartUI::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry, InDeltaTime );

}


void UAnimalStartUI::SetSelectionUI ()
{

	SizeBoxes.Add ( SizeBox_AnimalPortrait1 );
	SizeBoxes.Add ( SizeBox_AnimalPortrait2 );
	SizeBoxes.Add ( SizeBox_AnimalPortrait3 );
	
	int32 AnimalClassCount = 0;
	for (int i = 0; i < SizeBoxes.Num(); ++i)
	{
		UAnimalPortraitBox* box = Cast<UAnimalPortraitBox> ( SizeBoxes[i]->GetChildAt(0));
		if (box)
		{
			box->StartUI = this;
			box->SetAnimalData ( GS->AnimalsClass[AnimalClassCount] );
			box->AnimalIndex = AnimalClassCount;
			AnimalClassCount++;
			Boxes.Add ( box );
		}
	}

	
	
	Boxes[0]->SelectAnimal ();

}

void UAnimalStartUI::ShowSettingUI ()
{
	if (MainUI)
	{
		MainUI->ShowUI ( UIGameplayTags::UI_GameSetting );
	}
}

void UAnimalStartUI::ShowGameInfo ()
{
	if (MainUI)
	{
		MainUI->ShowUI ( UIGameplayTags::UI_GameInfo );
	}
}


void UAnimalStartUI::BindToPlayerState ( AEFHPlayerState* newPS )
{
	UEFHBaseUI::BindToPlayerState ( newPS );
	

	FTimerHandle setflag;
	GetWorld ()->GetTimerManager ().SetTimer ( setflag , FTimerDelegate::CreateLambda ( [this] {

		PlayerFlag->SetVisibility ( ESlateVisibility::Visible );
		if (PS->GameTeam == EGameTeam::A)
		{
			Img_Flag->SetBrushFromTexture ( Flag_Sprites[0] );
			TextBlock_Team->SetText ( FText::FromString ( TEXT ( "A" ) ) );

		}
		else
		{
			Img_Flag->SetBrushFromTexture ( Flag_Sprites[1] );
			TextBlock_Team->SetText ( FText::FromString ( TEXT ( "B" ) ) );
		}
		} ) , 0.5f , false );

	
}

void UAnimalStartUI::BindToGameState ( AEFHGameState* newGS )
{
	UEFHBaseUI::BindToGameState ( newGS );
	if (!GS) return;

	

	SetSelectionUI ();
}


void UAnimalStartUI::SelectAnimal ( int32 BoxIndex )
{
	for (int32 i = 0; i < Boxes.Num (); ++i)
	{
		if (i == BoxIndex) continue;

		Boxes[i]->UnselectAnimal ();
	}

	for (int32 i = 0; i < Boxes.Num (); ++i)
	{
		if (i == BoxIndex)
		{
			SizeBoxes[i]->SetWidthOverride ( SelectBoxSize.X );
			SizeBoxes[i]->SetHeightOverride ( SelectBoxSize.Y );
		}
		else
		{
			SizeBoxes[i]->SetWidthOverride ( OrginBoxSize.X );
			SizeBoxes[i]->SetHeightOverride ( OrginBoxSize.Y );
		}

		
	}

	SelectBox = Boxes[BoxIndex];

	UAnimalDataAsset* data = SelectBox->GetAnimalData ();
}

void UAnimalStartUI::StartAnimal ()
{
	if (SelectBox)
	{
		MainUI->StopLevelSequence ();

		AEFHPlayerController* pc = GetOwningPlayer <AEFHPlayerController> ();
		if (pc)
		{
			pc->ServerRPC_SpawnActorAndPossess ( SelectBox->AnimalIndex );
		}
		pc->SetInputMode ( FInputModeGameOnly() );
		pc->SetShowMouseCursor ( false );
		//	this->SetVisibility ( ESlateVisibility::Collapsed );

		MainUI->HideUI ( WidgetTag );
		MainUI->ShowUI ( UIGameplayTags::UI_GameReady );
		MainUI->ShowUI ( UIGameplayTags::UI_AnimalState );

		if (PS)
		{
			PS->SetAnimal ();
		}
	}
}
