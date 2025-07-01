// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/SlateWidget/SItemSlotBox.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SItemSlotBox::Construct(const FArguments& InArgs)
{
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/

	Brushes = InArgs._ItemBrushes;

}

int32 SItemSlotBox::OnPaint ( const FPaintArgs& Args , const FGeometry& AllottedGeometry , const FSlateRect& MyCullingRect , FSlateWindowElementList& OutDrawElements , int32 LayerId , const FWidgetStyle& InWidgetStyle , bool bParentEnabled ) const
{
    FVector2D Center = AllottedGeometry.GetLocalSize () * 0.5f;
    float Radius = 120.0f;
    int32 Count = Brushes.Num ();
    float AngleStep = 360.0f / FMath::Max ( Count , 1 );

   /* for (int32 i = 0; i < Count; ++i)
    {
        float AngleDeg = i * AngleStep + RotationOffset;
        float AngleRad = FMath::DegreesToRadians ( AngleDeg );

        FVector2D Pos = Center + FVector2D ( FMath::Cos ( AngleRad ) , FMath::Sin ( AngleRad ) ) * Radius;

    
        FVector2D Size ( Brushes[i].ImageSize.X , Brushes[i].ImageSize.Y);

        FSlateDrawElement::MakeBox (
            OutDrawElements ,
            LayerId ,
            AllottedGeometry.ToPaintGeometry ( Pos - Size * 0.5f , Size ) ,
            &Brushes[i] ,
            ESlateDrawEffect::None ,
            FLinearColor::White
        );
    }*/

    return LayerId + 1;
}

void SItemSlotBox::Tick ( const FGeometry& AllottedGeometry , const double InCurrentTime , const float InDeltaTime )
{
	RotationOffset += InDeltaTime * 30.0f;

	if (RotationOffset > 360.f)
		RotationOffset -= 360.f;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
