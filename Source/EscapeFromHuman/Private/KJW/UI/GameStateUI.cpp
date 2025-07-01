// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/GameStateUI.h"
#include "KJW/Game/EFHGameState.h"
#include "KJW/Game/EFHPlayerState.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "Animation/UMGSequencePlayer.h"

void UGameStateUI::BindToPlayerState(AEFHPlayerState* newPS)
{
	Super::BindToPlayerState(newPS);

	if (PS)
	{
		PS->OnChangeTeamScore.AddUObject(this, &UGameStateUI::SetTeamScoreText);
		
	}
}

void UGameStateUI::BindToGameState(AEFHGameState* newGS)
{
	Super::BindToGameState(newGS);

	UEFHBaseUI::BindToGameState(newGS);
	if (GS)
	{
		GS->OnStartStateEvent.AddUObject(this, &UGameStateUI::SetGameStart);
		GS->OnStartStateEvent.AddUObject(this, &UGameStateUI::SetTeamImg);
	}
}

void UGameStateUI::NativeConstruct()
{
	Super::NativeConstruct();

	AnimTimePlayer = PlayAnimation(Anim_Time, 0.0f, 0);

}

void UGameStateUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetRemainingTimeText();
}

void UGameStateUI::SetTeamImg()
{
	if (!PS) return;

	if (PS->GameTeam == EGameTeam::A)	
		Img_TeamA->SetBrushFromTexture(Flame_Sprite);
	else
		Img_TeamB->SetBrushFromTexture(Flame_Sprite);
	
}

void UGameStateUI::SetTeamScoreText()
{
	if (!PS || !GS) return;

	int32 teamA = GS->TeamAScore;
	int32 teamB = GS->TeamBScore;

	if (teamA != teamOrginScoreA)
		PlayAnimation(Anim_TeamAScore);
	
	if (teamB != teamOrginScoreB)	
		PlayAnimation(Anim_TeamBScore);
	
	teamOrginScoreA = teamA;
	teamOrginScoreB = teamB;
	Text_TeamAScore->SetText(FText::AsNumber(teamA));
	Text_TeamBScore->SetText(FText::AsNumber(teamB));
}


void UGameStateUI::SetRemainingTimeText()
{
	if (!GS) return;
	if (!bGame) return;
	float gameTime = GS->GetTimerDuration();
	float remainingTime = GS->GetRemainingTime();


	FString str = FString::Printf(TEXT("%.0f"), remainingTime);

	if (remainingTime < 10.0f)
		str = FString::Printf(TEXT("%.2f"), remainingTime);

	Text_RemainingTime->SetText(FText::FromString(str));

	float percent = remainingTime / gameTime;
	Progress_GameTime->SetPercent(percent);


	if (percent < 0.2f)
	{
		Progress_GameTime->SetFillColorAndOpacity(FColor::Red);
		if (!bStartTimerAnim)
		{
			bStartTimerAnim = true;

			if (AnimTimePlayer)
			{
				AnimTimePlayer->Stop();
			}

			PlayAnimation(Anim_TimeRed, 0.0f, 0);
		}
	}
	else if (percent < 0.6f)
	{
		Progress_GameTime->SetFillColorAndOpacity(FColor::Orange);
	}
	else
	{
		Progress_GameTime->SetFillColorAndOpacity(FColor::Green);
	}


	if (bGame && percent <= 0.0f)
	{
		bGame = false;
		PlayAnimation( Anim_GameEnd );
	}
}

void UGameStateUI::SetGameStart()
{
	if (!GS) return;

	if (!bGame)
		bGame = true;
}

