// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/AnimalMainUI.h"

#include "KJW/UI/EFHBaseUI.h"
#include "KJW/Game/EFHGameState.h"
#include "KJW/Game/EFHPlayerState.h"
#include "KJW/UIGamePlayTag.h"

#include "Components/CanvasPanel.h"


#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"


void UAnimalMainUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	PlayLevelSequence ();
	SetUI ();
}

void UAnimalMainUI::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );

}

void UAnimalMainUI::SetUI ()
{
	int32 childCount = MainCanvas->GetChildrenCount ();

	for (int32 i = 0; i < childCount; i++)
	{
		UWidget* child = MainCanvas->GetChildAt ( i );
		UEFHBaseUI* EFHUI = Cast<UEFHBaseUI> ( child );
		if (EFHUI)
		{
			EFHUI->SetMainUI ( this );
			if (EFHUI->WidgetTag.IsValid ())
			{
				if (!UIs.Contains ( EFHUI->WidgetTag ))
				{
					UIs.Add ( EFHUI->WidgetTag , EFHUI );
				}
			}
			EFHUI->SetVisibility ( ESlateVisibility::Collapsed );
			//
		}
	}

	ShowUI ( UIGameplayTags::UI_AnimalSelect );
}

void UAnimalMainUI::BindToPlayerState ( AEFHPlayerState* newPS )
{
	PS = newPS;

	for (TPair<FGameplayTag , UEFHBaseUI*>& Elem : UIs)
	{
		if (Elem.Value)
		{
			Elem.Value->BindToPlayerState ( newPS );
		}
	}
	
	PS->OnSelectAnimal.AddUObject ( this , &UAnimalMainUI::GameReadyEvent );
}

void UAnimalMainUI::BindToGameState ( AEFHGameState* newGS )
{
	GS = newGS;

	for (TPair<FGameplayTag , UEFHBaseUI*>& Elem : UIs)
	{
		if (Elem.Value)
		{
			Elem.Value->BindToGameState ( newGS );
		}
	}
	GS->OnStartStateEvent.AddUObject ( this , &UAnimalMainUI::GameStartEvent );
	GS->OnEndStateEvent.AddUObject ( this , &UAnimalMainUI::GameEndEvent );
}

void UAnimalMainUI::ShowUI ( FGameplayTag UITag )
{
	if(UIs.Contains ( UITag ))
	{
		UIs[UITag]->SetVisibility ( ESlateVisibility::Visible );
	}
}

void UAnimalMainUI::HideUI ( FGameplayTag UITag )
{
	if (UIs.Contains ( UITag ))
	{
		UIs[UITag]->SetVisibility ( ESlateVisibility::Collapsed );
	}
}


void UAnimalMainUI::GameReadyEvent ()
{
	

}

void UAnimalMainUI::GameStartEvent ()
{
	HideUI ( UIGameplayTags::UI_GameReady );
	ShowUI ( UIGameplayTags::UI_GameState );
}

void UAnimalMainUI::GameEndEvent ()
{
	ShowUI ( UIGameplayTags::UI_GameEnd );
}

void UAnimalMainUI::ShowSettingUI ()
{
	SettingUI->SetVisibility ( ESlateVisibility::Visible );
}

void UAnimalMainUI::ShowGameInfoUI ()
{
}

void UAnimalMainUI::PlayLevelSequence ()
{
	if (IntroSequence)
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.LoopCount.Value = -1;


		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer ( GetWorld () , IntroSequence , Settings , SequenceActor );
		if (SequencePlayer)
		{
			SequencePlayer->Play ();
		}


	}
}

void UAnimalMainUI::StopLevelSequence ()
{
	if (SequenceActor)
	{
		SequenceActor->Destroy ();
		SequenceActor = nullptr;
	}
}
