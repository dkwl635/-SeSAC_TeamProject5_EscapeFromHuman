// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScore.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UPlayerScore : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;
public:
	
	void SetScoreText ();
protected :
	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_Score;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* Text_Score;
public:
	UPROPERTY()
	class   AEFHPlayerState* PS;
	UPROPERTY ()
	class   AAnimalBase* Animal;
};
