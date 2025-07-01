// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/UI/EFHBaseUI.h"
#include "GameStateUI.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UGameStateUI : public UEFHBaseUI
{
	GENERATED_BODY()
	
public:

	UPROPERTY ( meta = (BindWidget) )
	class UProgressBar* Progress_GameTime;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* Text_RemainingTime;


	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* Text_TeamAScore;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* Text_TeamBScore;

	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_TeamA;
	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_TeamB;

	UPROPERTY ( Transient , meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_TeamAScore;
	UPROPERTY ( Transient , meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_TeamBScore;
	UPROPERTY ( Transient , meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_Time;
	UPROPERTY ( Transient , meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_TimeRed;
	UPROPERTY ( Transient , meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_GameEnd;


public:
	virtual void BindToPlayerState ( class AEFHPlayerState* newPS ) override;
	virtual void BindToGameState ( class AEFHGameState* newGS ) override;

protected:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;

	
	void SetTeamImg ();
	void SetTeamScoreText ();
	void SetRemainingTimeText ();
	void SetGameStart ();

protected:

	//자신의 팀 표시 이미지
	UPROPERTY ( EditDefaultsOnly )
	UTexture2D* Flame_Sprite;

	bool bSetGameResult = false;
	bool bSetPawn = false;

	int32 teamOrginScoreA = 0;
	int32 teamOrginScoreB = 0;

	UPROPERTY ()
	UUMGSequencePlayer* AnimTimePlayer;

	bool bStartTimerAnim = false;

	bool bGame = false;


};
