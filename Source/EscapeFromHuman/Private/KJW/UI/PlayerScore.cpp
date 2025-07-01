// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/PlayerScore.h"
#include "EscapeFromHuman.h"
#include "KJW/Game/EFHPlayerState.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPlayerScore::NativeConstruct ()
{
	Super::NativeConstruct ();

	if (GetOwningLocalPlayer ())
	{
		PRINTLOG ( TEXT ( "PC name : %s" ) ,*GetOwningLocalPlayer()->GetName() );
	}

	if (GetOwningPlayerState ())
	{
		PRINTLOG ( TEXT ( "PC name : %s" ) , *GetOwningLocalPlayer ()->GetName () );
	}
	

	SetScoreText ();
}

void UPlayerScore::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );

	if (!Animal) return;

	if (Animal->IsLocallyControlled ()) return;

	if (Animal->StatusComp->IsInvisible)
	{
		Text_Score->SetVisibility ( ESlateVisibility::Hidden );
		Img_Score->SetVisibility ( ESlateVisibility::Hidden );
	}
	else
	{
		Text_Score->SetVisibility ( ESlateVisibility::Visible );
		Img_Score->SetVisibility ( ESlateVisibility::Visible );
		//this->SetVisibility ( ESlateVisibility::Visible );
	}

}

void UPlayerScore::SetScoreText ()
{


	if (!PS) return;

	int32 score = PS->GetTempScore ();
	Text_Score->SetText ( FText::AsNumber ( score ) );

}
