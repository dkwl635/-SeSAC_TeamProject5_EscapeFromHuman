// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KJW/UI/EFHBaseUI.h"
#include "AnimalUI.generated.h"

/**
 * 
 */
UCLASS ()
class ESCAPEFROMHUMAN_API UAnimalUI : public UEFHBaseUI
{
	GENERATED_BODY ()


public:
	

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_ItemA;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_ItemB;


	UPROPERTY ( meta = (BindWidget) )
	class UImage* DsahImg;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_DashCount;
	UPROPERTY ( meta = (BindWidget) )
	class UImage* IceImg;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_IceCount;

	UPROPERTY ( meta = (BindWidget) )
	class UImage* SkillImg;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_SkillCount;
	UPROPERTY ( meta = (BindWidget) )
	class UImage* Hp1_Img;
	UPROPERTY ( meta = (BindWidget) )
	class UImage* Hp2_Img;

	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* Text_StunEvent;

	
	UPROPERTY ( Transient, meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_CanDash;
	UPROPERTY ( Transient, meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_CanIce;
	UPROPERTY ( Transient, meta = (BindWidgetAnim) )
	class UWidgetAnimation* Anim_CanSkill;


public:
	virtual void BindToPlayerState ( class AEFHPlayerState* newPS ) override;
	virtual void BindToGameState ( class AEFHGameState* newGS ) override;

protected:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;

	void SetPawn ();
	void SetItemSlotImg ();

	void CheckFunc();
	//대쉬
	void CheckCanDash ();
	//얼음
	void CheckCanIce ();
	//스킬
	void CheckCanSkill ();
	//체력
	void CheckHp ();
	//상태 메세지
	void ShowState ( int32 Event );

protected:
	UPROPERTY ()
	class UItemComponent* ItemComp;

	UPROPERTY ()
	class UStatusComponent* StatusComp;

	UPROPERTY ()
	class UPlayerInputComponent* InputComp;
	

	
	bool bSetPawn = false;
	bool bGame = false;

	FTimerHandle CheckTimerHandle;
	float CheckDelay = 0.02f;
	//대쉬 확인
	bool bDash = true;
	float DashTime = 0.0f;
	//얼음 확인
	bool bIce = true;
	float IceTime = 0.0f;
	//스킬 확인
	bool bSkill = true;
	float SkillTime = 0.0f;
	//hp 확인
	int32 CurrentHp = 0;


	//스턴 이벤트 텍스트 타이머
	FTimerHandle StunTextHandle;
};
