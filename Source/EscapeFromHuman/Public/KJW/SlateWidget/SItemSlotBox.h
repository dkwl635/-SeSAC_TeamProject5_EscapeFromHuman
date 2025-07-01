// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class ESCAPEFROMHUMAN_API SItemSlotBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SItemSlotBox){}
	//변수 
	SLATE_ARGUMENT(TArray<FSlateBrush>, ItemBrushes)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	int32 OnPaint ( const FPaintArgs& Args , const FGeometry& AllottedGeometry , const FSlateRect& MyCullingRect , FSlateWindowElementList& OutDrawElements , int32 LayerId , const FWidgetStyle& InWidgetStyle , bool bParentEnabled ) const override;

	virtual void Tick( const FGeometry& AllottedGeometry , const double InCurrentTime , const float InDeltaTime ) override;

private:
	TArray<FSlateBrush> Brushes;
	float RotationOffset = 0.0f;
};

