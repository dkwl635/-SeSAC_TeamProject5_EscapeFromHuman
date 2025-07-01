// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "AI_UserWidget.generated.h"

UENUM(BlueprintType)
enum class EAlertType : uint8
{
	None        UMETA(DisplayName = "None"),
	Exclamation UMETA(DisplayName = "!"),
	Question    UMETA(DisplayName = "?")
};

UCLASS()
class ESCAPEFROMHUMAN_API UAI_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 외부에서 이걸 호출
	UFUNCTION(BlueprintCallable)
	void UpdateIcon(EAlertType NewType);

protected:
	// 위젯에서 바인딩한 이미지
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	// 아이콘들 (에디터에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert")
	UTexture2D* ExclamationTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alert")
	UTexture2D* QuestionTexture;

	virtual void NativeConstruct() override;
};
