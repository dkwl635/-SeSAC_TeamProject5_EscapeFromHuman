// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/UI/GameResultUI.h"
#include "KJW/Game/EFHGameState.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "KJW/Game/EFHGameInstance.h"
#include "KJW/Game/EFHPlayerState.h"

void UGameResultUI::NativeConstruct ()
{
	
	Super::NativeConstruct ();

	PlayAnimation ( Widgetanim_Game );

	EffectSlot = Cast<UCanvasPanelSlot> ( WinEffect->Slot );

	Btn_Retry->OnClicked.AddDynamic ( this , &ThisClass::RetryGame );
	Btn_Retry->SetVisibility ( ESlateVisibility::Collapsed );

	

}

void UGameResultUI::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );
	RotEffect ( InDeltaTime );
}

void UGameResultUI::BindToGameState ( AEFHGameState* newGS )
{
	

	UEFHBaseUI::BindToGameState ( newGS );
	if (GS)
	{
		GS->OnEndStateEvent.AddUObject ( this , &ThisClass::SetResult );
		GS->OnMoveLevel.AddUObject ( this , &ThisClass::ShowMoveText );
	}
}

void UGameResultUI::SetResult ()
{
	if (!GS) return;

	int32 AScore = GS->TeamAScore;
	int32 BScore = GS->TeamBScore;

	Text_TeamAScore->SetText ( FText::AsNumber ( AScore ) );
	Text_TeamBScore->SetText ( FText::AsNumber ( BScore ) );

	if (AScore == BScore)
		GameResult = 3;
	else if (AScore > BScore)
		GameResult = 1;
	else
		GameResult = 2;

	if (GameResult == 1)
	{
		Img_AResult->SetBrushFromTexture (GameResult_Sprites[0]);
		Img_BResult->SetBrushFromTexture (GameResult_Sprites[1]);
	}
	else if (GameResult == 2)
	{
		Img_AResult->SetBrushFromTexture ( GameResult_Sprites[1] );
		Img_BResult->SetBrushFromTexture ( GameResult_Sprites[0] );
	}
	else
	{
		Img_AResult->SetBrushFromTexture ( GameResult_Sprites[2] );
		Img_BResult->SetBrushFromTexture ( GameResult_Sprites[2] );

	}
	GetOwningPlayer ()->SetShowMouseCursor ( true );
	GetOwningPlayer()->SetInputMode (FInputModeGameAndUI ());


}

void UGameResultUI::RotEffect (float DeltaTime )
{
	if (WinEffect->GetVisibility () != ESlateVisibility::Visible) return;

	float Angle = WinEffect->GetRenderTransformAngle ();
	WinEffect->SetRenderTransformAngle ( Angle + (DeltaTime * 30.0f) );
}

void UGameResultUI::SetEffect ()
{
	if (PS->GameTeam == EGameTeam::A)
	{
		Btn_Retry->SetVisibility ( ESlateVisibility::Visible );
	}



	if (GameResult == 3) return;

	WinEffect->SetVisibility ( ESlateVisibility::Visible );
	if (GameResult == 1)
	{
		EffectSlot->SetPosition ( FVector2D ( -500.f , 0 ) );
	}
	else
	{
		EffectSlot->SetPosition ( FVector2D ( 500.f , 0 ) );

	}

	

}

void UGameResultUI::RetryGame ()
{
	if (GS)GS->OpenNewLevel ();

	Btn_Retry->SetVisibility ( ESlateVisibility::Collapsed );

	ShowMoveText ();
}

void UGameResultUI::ShowMoveText ()
{
	Text_Move->SetVisibility ( ESlateVisibility::Visible );
}
