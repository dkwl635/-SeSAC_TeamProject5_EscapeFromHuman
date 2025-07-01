// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EFHGameInstance.generated.h"

USTRUCT ( BlueprintType )
struct FSessionInfo
{
	GENERATED_BODY ()

	UPROPERTY ( BlueprintReadOnly )
	FString roomName;

	UPROPERTY ( BlueprintReadOnly )
	FString hostName;

	UPROPERTY ( BlueprintReadOnly )
	FString playerCount;

	UPROPERTY ( BlueprintReadOnly )
	int32 pingSpeed = 0;

	UPROPERTY ( BlueprintReadOnly )
	int32 index = 0;

	UPROPERTY ( BlueprintReadOnly )
	int32 gameState;

	inline FString ToString ()
	{
		return FString::Printf ( TEXT ( "[%d] %s : %s - %s, %dms : GameState : %d" ) , index , *roomName , *hostName , *playerCount , pingSpeed , gameState );
	}
};

/**
 * 
 */
 // 세션검색 끝났을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam ( FSearchSignature , const FSessionInfo& , sessionInfo );

// 세션검색 상태 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam ( FSearchStateSignature , bool , bIsSearching );


UCLASS()
class ESCAPEFROMHUMAN_API UEFHGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init () override;
public:
	//방찾기 완료시 콜백을 등록한 델리게이트
	FSearchSignature OnSearchCompleted;
	//방생성 상태를 콜백 델리게이트
	FSearchStateSignature OnCreateState;
	//방찾기 상태를 콜백 델리게이트
	FSearchStateSignature OnSearchState;


	//방 입장 콜백 델리게이트
	FSimpleMulticastDelegate OnEnterSession;

private :
	// 방관리 인터페이스
	IOnlineSessionPtr SessionInterface;

	//방생성시 이름
	FString MySessionName = TEXT ( "EscapeFromHuman" );

	const FString MainLevel = TEXT ( "/Game/KJW/Tutorial_2" );

	//방 검색목록 리스트
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

public:
	//방 찾기
	void FindOtherSession ();
	//특정 방 입장
	void JoinSelectedSession ( int32 Index );
	//방 생성
	void CreateMySession ( FString RoomName , int32 playerCount = 2 );
	//맵 다시 시작
	void ServerMapReStart ();
	//찾은 방 바로 입장
	void EndterFindRoom ();
	
	//세션 나가기
	void ExitSession ();
private:
	//방 생성시 콜백 함수
	UFUNCTION()
	void OnCreateSessionComplete ( FName SessionName , bool bWasSuccessful );
	//방찾기 실행시 콜백 함수
	UFUNCTION()
	void OnFindSessionComplete ( bool bWasSuccessful );

	//방입장시 콜백 함수
	void OnJoinSessionCompleted ( FName SessionName , EOnJoinSessionCompleteResult::Type Result );

	//세션 추방 및 나가기 콜백 함수
	void OnMyExitSessionCompleted ( FName SessionName , bool bWasSuccessful );
	//게임 종료
	void GameExit ();

private:
	//다국어 코딩
	FString StringBase64Encode ( const FString& Str );
	FString StringBase64Decode ( const FString& Str );

	

};
