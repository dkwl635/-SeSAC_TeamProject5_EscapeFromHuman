// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/LoginWidget.h"
#include "KJW/Game/EFHGameInstance.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "KJW/UI/SessionSlotWidget.h"

void ULoginWidget::NativeConstruct ()
{
	Super::NativeConstruct ();

	GI = Cast<UEFHGameInstance> ( GetWorld ()->GetGameInstance () );
	
	GI->OnSearchCompleted.AddDynamic ( this , &ULoginWidget::AddSlotWidget );
	GI->OnSearchState.AddDynamic ( this , &ULoginWidget::OnChangeButtonEnable );
	GI->OnSearchState.AddDynamic ( this , &ULoginWidget::OnFindSession );


	Btn_CreateSession->OnClicked.AddDynamic ( this , &ULoginWidget::CreateRoom );
	Btn_FindSession->OnClicked.AddDynamic ( this , &ULoginWidget::OnClickedFindSession );
	
	//Btn_FindSession->OnClicked.AddDynamic ( this , &ULoginWidget::SwitchFindPanel );
	//Btn_Back->OnClicked.AddDynamic ( this , &ULoginWidget::BackToMain );
	//Btn_Find->OnClicked.AddDynamic ( this , &ULoginWidget::OnClickedFindSession );

	Text_Log->SetText(FText::FromString(TEXT("")));
}

void ULoginWidget::CreateRoom ()
{
	ShowButtons(false);
	FString str = TEXT("방 생성 중");
	Text_Log->SetText(FText::FromString(str));

	if (GI)
	{
		GI->CreateMySession (TEXT("EscapeFromHuman"));
	}
	
}

void ULoginWidget::SwitchFindPanel ()
{
	/*if (edit_userName->GetText ().IsEmpty () == false)
	{
		gi->mySessionName = edit_userName->GetText ().ToString ();
	}*/

	WidgetSwitcher->SetActiveWidgetIndex ( 1 );
	OnClickedFindSession ();
}

void ULoginWidget::BackToMain ()
{
	WidgetSwitcher->SetActiveWidgetIndex ( 0 );
}

void ULoginWidget::OnClickedFindSession ()
{
	// 기존 슬롯이 있다면 모두 지운다
	//Scroll_RoomList->ClearChildren ();
	ShowButtons(false);
	if (GI != nullptr)
	{
		OnFindingSession ();
		GI->FindOtherSession ();
	}
}


void ULoginWidget::OnChangeButtonEnable ( bool bIsSearching )
{
	Btn_Find->SetIsEnabled ( !bIsSearching );

	if (bIsSearching == true)
	{
		// 검색중 보이도록 처리
		Text_FindingMsg->SetVisibility ( ESlateVisibility::Visible );
	}
	else
	{
		// 검색중 사라지도록 처리
		Text_FindingMsg->SetVisibility ( ESlateVisibility::Hidden );
	}
}

void ULoginWidget::AddSlotWidget ( const struct FSessionInfo& sessionInfo )
{
	auto slot = CreateWidget<USessionSlotWidget> ( this , sessionInfoWidget );
	slot->Set ( sessionInfo );

	Scroll_RoomList->AddChild ( slot );
}

void ULoginWidget::OnFindSession(bool bFind)
{
	if (bFind)
	{
		OnSuccessFind();
	}
	else
	{
		OnFailFind();
	}
}

void ULoginWidget::OnCreateSession(bool bFind)
{
	ShowButtons(true);
	if (bFind)
	{
		FString str = TEXT("방 생성 성공");
		Text_Log->SetText(FText::FromString(str));
	}
	else
	{
		FString str = TEXT("방 생성 실패");
		Text_Log->SetText(FText::FromString(str));

	}
}

void ULoginWidget::OnFailFind()
{
	ShowButtons(true);
	FString str = TEXT("방 찾기 실패");
	Text_Log->SetText(FText::FromString(str));
}

void ULoginWidget::OnSuccessFind()
{
	FString str = TEXT("방 찾기 성공");
	Text_Log->SetText(FText::FromString(str));
}

void ULoginWidget::OnMoveLevel()
{
	FString str = TEXT("방 이동 중");
	Text_Log->SetText(FText::FromString(str));
}

void ULoginWidget::OnFindingSession()
{
	FString str = TEXT("방 검색 중");
	Text_Log->SetText(FText::FromString(str));
}

void ULoginWidget::ShowButtons(bool bShow)
{
	if (bShow)
	{
		Btn_CreateSession->SetVisibility(ESlateVisibility::Visible);
		Btn_FindSession->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		Btn_CreateSession->SetVisibility(ESlateVisibility::Collapsed);
		Btn_FindSession->SetVisibility(ESlateVisibility::Collapsed);
	}
}

