// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/AnimalUI.h"
#include "KJW/ActorComponent/ItemComponent.h"
#include "KJW/Game/EFHGameState.h"
#include "KJW/Game/EFHPlayerState.h"
#include "KJW/Actor/ThrowItem.h"
#include "LJW/AnimalBase.h"
#include "LJW/AnimalDataAsset.h"
#include "LJW/StatusComponent.h"
#include "LJW/PlayerInputComponent.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

#include "Animation/UMGSequencePlayer.h"

void UAnimalUI::BindToPlayerState(AEFHPlayerState* newPS)
{
	UEFHBaseUI::BindToPlayerState ( newPS );

}

void UAnimalUI::BindToGameState(AEFHGameState* newGS)
{
	UEFHBaseUI::BindToGameState ( newGS );


}

void UAnimalUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	
	Img_ItemB->SetVisibility ( ESlateVisibility::Hidden );
	Img_ItemA->SetVisibility ( ESlateVisibility::Hidden );

	Text_StunEvent->SetVisibility ( ESlateVisibility::Hidden );
}

void UAnimalUI::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );
	SetPawn ();

}

void UAnimalUI::SetPawn ()
{
	if (bSetPawn) return;

	APawn* owner = GetOwningPlayerPawn ();
	if (owner)
	{
		AAnimalBase* animal = Cast<AAnimalBase> ( owner );
		if (!animal) return;


		ItemComp = animal->ItemComp;
		StatusComp = animal->StatusComp;
		InputComp = animal->InputComp;

		ItemComp->OnItemEvent.AddUObject ( this , &UAnimalUI::SetItemSlotImg );
		SetItemSlotImg ();
	
		 UTexture2D* skillicon =  animal->AnimalData->AnimalStats.Skillicon;
		 SkillImg->SetBrushFromTexture ( skillicon );

		bSetPawn = true;

		GetWorld ()->GetTimerManager ().SetTimer ( CheckTimerHandle , this , &ThisClass::CheckFunc , CheckDelay , true );

		animal->OnStunEvent.AddUObject ( this , &ThisClass::ShowState );
	}
}

void UAnimalUI::SetItemSlotImg ()
{
	if (ItemComp)
	{
		int32 AIndex = 0;

		if (ItemComp->GetItem ( AIndex ) && ItemComp->GetItem ( AIndex )->ItemIcon)
		{
			Img_ItemA->SetVisibility ( ESlateVisibility::Visible );
			Img_ItemA->SetBrushFromTexture ( ItemComp->GetItem ( AIndex )->ItemIcon );
		}
		else
		{
			Img_ItemA->SetVisibility ( ESlateVisibility::Hidden );
		}

		int32 BIndex = 1;
		if (ItemComp->ItemListMax == BIndex)
		{
			BIndex = 0;
		}


		if (ItemComp->GetItem ( BIndex ) && ItemComp->GetItem ( BIndex )->ItemIcon)
		{
			Img_ItemB->SetVisibility ( ESlateVisibility::Visible );
			Img_ItemB->SetBrushFromTexture ( ItemComp->GetItem ( BIndex )->ItemIcon );
		}
		else
		{
			Img_ItemB->SetVisibility ( ESlateVisibility::Hidden );
		}
	}
}

void UAnimalUI::CheckFunc ()
{
	CheckCanDash ();
	CheckCanIce ();
	CheckCanSkill ();
	CheckHp ();
}

void UAnimalUI::CheckCanDash ()
{
	if (!StatusComp && !InputComp) return;

	if (DashTime > 0.0f)
	{
		DashTime -= CheckDelay;
		TextBlock_DashCount->SetText ( FText::AsNumber ( (int32)DashTime + 1 ) );

		if (DashTime <= 0.0f)
			TextBlock_DashCount->SetVisibility ( ESlateVisibility::Collapsed );
	}

	if ( StatusComp->bCanDash == bDash) return;


	if (StatusComp->bCanDash && !bDash)
	{
		bDash = true;
		DsahImg->SetColorAndOpacity ( FLinearColor::White );
		PlayAnimation ( Anim_CanDash );

	}
	else if (!StatusComp->bCanDash && bDash)
	{
		bDash = false;
		DsahImg->SetColorAndOpacity ( FLinearColor::Gray );
		DashTime = InputComp->DashCooldown ;
		TextBlock_DashCount->SetVisibility ( ESlateVisibility::Visible );

	}
}

void UAnimalUI::CheckCanIce ()
{
	if (!StatusComp || !InputComp) return;

	if (IceTime > 0.0f)
	{
		IceTime -= CheckDelay;
		TextBlock_IceCount->SetText ( FText::AsNumber ( (int32)IceTime + 1 ) );

		if (IceTime <= 0.0f)
			TextBlock_IceCount->SetVisibility ( ESlateVisibility::Collapsed );
	}

	if (StatusComp->bCanIce == bIce) return;

	if (StatusComp->bCanIce && !bIce)
	{
		bIce = true;
		IceImg->SetColorAndOpacity ( FLinearColor::White );
		PlayAnimation ( Anim_CanIce );
	}
	else if (!StatusComp->bCanIce && bIce)
	{
		bIce = false;
		IceImg->SetColorAndOpacity ( FLinearColor::Gray );
		IceTime = InputComp->IceCooldown;
		TextBlock_IceCount->SetVisibility ( ESlateVisibility::Visible );
	}
}

void UAnimalUI::CheckCanSkill ()
{
	if (!StatusComp || !InputComp) return;

	if (SkillTime > 0.0f)
	{
		SkillTime -= CheckDelay;
		TextBlock_SkillCount->SetText ( FText::AsNumber ( (int32)SkillTime + 1 ) );

		if (SkillTime <= 0.0f)
			TextBlock_SkillCount->SetVisibility ( ESlateVisibility::Collapsed );
	}

	if (StatusComp->bSkill == bSkill) return;

	if (StatusComp->bSkill && !bSkill)
	{
		bSkill = true;
		SkillImg->SetColorAndOpacity ( FLinearColor::White );
		PlayAnimation ( Anim_CanSkill );
	}
	else if (!StatusComp->bSkill && bSkill)
	{
		bSkill = false;
		SkillImg->SetColorAndOpacity ( FLinearColor::Gray );
		SkillTime = StatusComp->ShoutingCoolDown;
		TextBlock_SkillCount->SetVisibility ( ESlateVisibility::Visible );
	}
}

void UAnimalUI::CheckHp ()
{
	if (!StatusComp) return;

	int32 hp = StatusComp->hp;
	if (CurrentHp == hp) return;

	CurrentHp = hp;

	if (hp == 0)
	{
		Hp1_Img->SetVisibility ( ESlateVisibility::Collapsed );
		Hp2_Img->SetVisibility ( ESlateVisibility::Collapsed );
	}
	else if (hp == 1)
	{
		Hp1_Img->SetVisibility ( ESlateVisibility::Collapsed );
		Hp2_Img->SetVisibility ( ESlateVisibility::Visible );
	}
	else if (hp == 2)
	{
		Hp1_Img->SetVisibility ( ESlateVisibility::Visible );
		Hp2_Img->SetVisibility ( ESlateVisibility::Visible );
	}


}

void UAnimalUI::ShowState ( int32 Event )
{
	Text_StunEvent->SetVisibility ( ESlateVisibility::Visible );

	switch (Event)
	{
	case 1 :
		Text_StunEvent->SetText ( FText::FromString ( TEXT ( "돌에 맞음!!" )));
		break;
	case 2:
		Text_StunEvent->SetText ( FText::FromString ( TEXT ( "덫에 걸림!!" ) ) );
		break;
	case 3:
		Text_StunEvent->SetText ( FText::FromString ( TEXT ( "기  절!!" ) ) );
		break;
	default:
		Text_StunEvent->SetVisibility ( ESlateVisibility::Collapsed );
		break;
	}

	if (StunTextHandle.IsValid ())
	{
		GetWorld ()->GetTimerManager ().ClearTimer ( StunTextHandle );
	}

	TWeakObjectPtr<UTextBlock> WeakThis = Text_StunEvent;
	GetWorld ()->GetTimerManager ().SetTimer ( StunTextHandle , FTimerDelegate::CreateLambda ( [WeakThis]
		{
			if (!WeakThis.IsValid ()) return;

			WeakThis->SetVisibility ( ESlateVisibility::Collapsed );
		} ) , 2.0f , false );
}


