// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/ReadyUI.h"
#include "KJW/Game/EFHGameState.h"
#include "KJW/Game/EFHPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UReadyUI::NativeConstruct ()
{
	Super::NativeConstruct ();



	PlayAnimation ( Widgetanim_In );

	FTimerHandle setflag;
	GetWorld ()->GetTimerManager ().SetTimer ( setflag , this , &ThisClass::TeamFlag , 1.0f , false );

	
}


void UReadyUI::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );

	SetWatingTimer ();
}

void UReadyUI::BindToGameState ( AEFHGameState* newGS )
{
	UEFHBaseUI::BindToGameState ( newGS );

	if (GS)
	{
		GS->OnReadyStateEvent.AddUObject ( this , &ThisClass::GameReadyEvent );
	}
}

void UReadyUI::BindToPlayerState ( AEFHPlayerState* newPS )
{
	Super::BindToPlayerState ( newPS );

	if (PS)
	{
		PS->OnReadyEvent.AddUObject ( this , &ThisClass::OnReadyEvent );

		OnReadyEvent ();
	}
}

void UReadyUI::GameReadyEvent ()
{
	FTimerHandle handle;
	GetWorld ()->GetTimerManager ().SetTimer ( handle , FTimerDelegate::CreateLambda ( [this] {
		
		PlayAnimation ( Widgetanim_Out );

		KeyInfo->SetVisibility ( ESlateVisibility::Collapsed );
		
		} ),GS->GetWaitingTime() , false);

	TextBlock_Ready->SetText ( FText::FromString ( TEXT ( "곧 게임이 시작 됩니다!" ) ) );

}

void UReadyUI::SetWatingTimer ()
{

	if (!GS) return;
	float gameTime = GS->GetWaitingTime ();
	float remainingTime = GS->GetRemainingWatingTime ();

	FString str = FString::Printf ( TEXT ( "%.1f" ) , remainingTime );
	if (remainingTime > 0.0f)
	{
		Text_WaitSecend->SetVisibility ( ESlateVisibility::Visible );
		Text_WaitSecend->SetText ( FText::FromString ( str ) );
	}
	else
	{
		Text_WaitSecend->SetVisibility ( ESlateVisibility::Hidden );
	}
	//
	//
	//Progress_GameTime->SetPercent ( 1 - (remainingTime / gameTime) );

}

void UReadyUI::TeamFlag ()
{
	if (!PS) return;

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

}

void UReadyUI::OnReadyEvent ()
{
	//레디시 이벤트
	if (PS)
	{
		if (PS->IsReady ())
		{
		TextBlock_Ready->SetText ( FText::FromString ( TEXT ( "준비완료!!" ) ) );
		}

	}

	
}

