// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Widget/ItemBoxSlotWidget.h"


TSharedRef<SWidget> UItemBoxSlotWidget::RebuildWidget ()
{
    ItemSlotBox = SNew ( SItemSlotBox )
        .ItemBrushes ( ItemBrushes );

    return ItemSlotBox.ToSharedRef ();
}

void UItemBoxSlotWidget::ReleaseSlateResources ( bool bReleaseChildren )
{
    Super::ReleaseSlateResources ( bReleaseChildren );

    // Slate 포인터 해제
    ItemSlotBox.Reset ();
}
