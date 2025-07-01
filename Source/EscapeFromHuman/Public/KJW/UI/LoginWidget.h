// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
		
	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UButton* Btn_CreateSession;

	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UButton* Btn_FindSession;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , meta = (BindWidget) )
	class UEditableText* Edit_RoomName;

	// 메인화면돌아가기 버튼
	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UButton* Btn_Back;

	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UWidgetSwitcher* WidgetSwitcher;

	//세션 결과
	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UTextBlock* Text_Log;


	// 세션슬롯
// Canvas_FindRoom 의 스크롤박스위젯
	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UScrollBox* Scroll_RoomList;

	// 방검색 버튼
	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UButton* Btn_Find;

	// 검색중 메시지
	UPROPERTY ( BlueprintReadOnly , meta = (BindWidget) )
	class UTextBlock* Text_FindingMsg;


public:
	virtual void NativeConstruct ()	override;

protected:

	UFUNCTION ()
	void CreateRoom ();

	UFUNCTION ()
	void SwitchFindPanel ();

	UFUNCTION ()
	void BackToMain ();


	// 방찾기 버튼 클릭시 호출될 콜백
	UFUNCTION ()
	void OnClickedFindSession ();

	// 방찾기 상태 이벤트 콜백
	UFUNCTION ()
	void OnChangeButtonEnable ( bool bIsSearching );

	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class USessionSlotWidget> sessionInfoWidget;

	UFUNCTION ()
	void AddSlotWidget ( const struct FSessionInfo& sessionInfo );



private:
	UFUNCTION ()
	void OnFindSession ( bool bFind );

	UFUNCTION ()
	void OnCreateSession ( bool bFind );
	UPROPERTY ()
	class UEFHGameInstance* GI;

	UFUNCTION()
	void OnFailFind ();

	UFUNCTION()
	void OnSuccessFind ();
	UFUNCTION()
	void OnMoveLevel ();
	UFUNCTION ()
	void OnFindingSession ();

	void ShowButtons (bool bShow);
	

};
