// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API USessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_RoomName;
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_HostName;
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_PlayerCount;
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_PingSpeed;

	// 방 입장
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Btn_Join;

	int32 sessionNumber;

protected:
	virtual void NativeConstruct () override;
	
public:
	void Set ( const struct FSessionInfo& SessionInfo );

	UFUNCTION ()
	void JoinSession ();
};
