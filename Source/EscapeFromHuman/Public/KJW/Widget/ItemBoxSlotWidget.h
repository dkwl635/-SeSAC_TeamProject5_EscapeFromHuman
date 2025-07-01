// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "KJW/SlateWidget/SItemSlotBox.h"
#include "ItemBoxSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UItemBoxSlotWidget : public UWidget
{
	GENERATED_BODY()
	

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemSlotBox")
    TArray<FSlateBrush> ItemBrushes;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources ( bool bReleaseChildren ) override;

private:
    TSharedPtr<SItemSlotBox> ItemSlotBox;
};
