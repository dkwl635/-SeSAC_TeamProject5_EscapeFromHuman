// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/SessionSlotWidget.h"
#include "KJW/Game/EFHGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionSlotWidget::NativeConstruct ()
{
	Super::NativeConstruct ();
	
	Btn_Join->OnClicked.AddDynamic ( this , &USessionSlotWidget::JoinSession );
}

void USessionSlotWidget::Set ( const struct FSessionInfo& SessionInfo )
{
	Text_RoomName->SetText ( FText::FromString ( SessionInfo.roomName ) );
	Text_HostName->SetText ( FText::FromString ( SessionInfo.hostName ) );
	Text_PlayerCount->SetText ( FText::FromString ( SessionInfo.playerCount ) );
	Text_PingSpeed->SetText ( FText::FromString ( FString::Printf ( TEXT ( "%dms" ) , SessionInfo.pingSpeed ) ) );

	sessionNumber = SessionInfo.index;
}


void USessionSlotWidget::JoinSession ()
{
	auto gi = Cast<UEFHGameInstance> ( GetWorld ()->GetGameInstance () );
	if (gi != nullptr)
	{
		gi->JoinSelectedSession ( sessionNumber );
	}
}
