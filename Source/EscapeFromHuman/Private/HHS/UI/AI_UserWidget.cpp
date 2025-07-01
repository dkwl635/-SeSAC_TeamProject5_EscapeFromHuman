// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/UI/AI_UserWidget.h"

#include "Components/Image.h"

void UAI_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 시작 시 이모티콘 안 보이게
	if (IconImage)
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UAI_UserWidget::UpdateIcon(EAlertType NewType)
{
	if (!IconImage) return;

	switch (NewType)
	{
	case EAlertType::Exclamation:
		IconImage->SetBrushFromTexture(ExclamationTexture);
		IconImage->SetVisibility(ESlateVisibility::Visible);
		break;

	case EAlertType::Question:
		if (!QuestionTexture)
		{
			UE_LOG(LogTemp, Error, TEXT("QuestionTexture is NULL!"));
			return;
		}
		IconImage->SetBrushFromTexture(nullptr); // 강제 리셋
		IconImage->SetBrushFromTexture(QuestionTexture);
		IconImage->SetVisibility(ESlateVisibility::Visible);
		break;

	case EAlertType::None:
	default:
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
}
