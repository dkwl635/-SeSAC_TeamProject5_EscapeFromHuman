// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnimalPortraitBox.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UAnimalPortraitBox : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct () override;

protected :
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Select;
	UPROPERTY ( meta = (BindWidget) )
	class UButton* Btn_Start;
	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Animal;

	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_Name;

	UPROPERTY ( meta = (BindWidget) )
	class UImage* Img_Skill;
	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_SkillName;

	UPROPERTY ( meta = (BindWidget) )
	class UTextBlock* TextBlock_SkillInfo;

protected:
	

	UPROPERTY(EditDefaultsOnly)
	TArray<UTexture2D* > Flag_Sprite;

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<UTexture2D*> Frame_Sprites;

	UPROPERTY ()
	TSubclassOf<class AAnimalBase> SlotAnimal;


public:

	UFUNCTION ( BlueprintCallable)
	void SetAnimalData ( TSubclassOf<class AAnimalBase> AnimalClass);
	UFUNCTION()
	void SelectAnimal ();
	UFUNCTION()
	void StartAnimal ();

	void UnselectAnimal ();

	class UAnimalDataAsset* GetAnimalData ();

	void SetPlayerIcon (int32 TeamIndex);
public:
	
	UPROPERTY()
	class UAnimalStartUI* StartUI;

	UPROPERTY(EditAnywhere)
	int32 AnimalIndex = 0;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite)
	float AnimTimer = 0.0f;


private:
	bool bOpen = false;
private:
	void ToggleButton ( bool bShouldOpen );
	
};
