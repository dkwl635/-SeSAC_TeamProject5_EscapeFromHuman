// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Game/EFHGameInstance.h"
#include "Online/CoreOnline.h"
//#include "Online\OnlineSessionNames.h"

#include "EscapeFromHuman.h"
#include "Online/OnlineSessionNames.h"



void UEFHGameInstance::Init ()
{
	Super::Init ();

	if (auto subsys = IOnlineSubsystem::Get ())
	{
		//인터페이스 가져오기
		SessionInterface = subsys->GetSessionInterface ();
		//방 생성시 완료 함수 콜백 등록
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject ( this , &UEFHGameInstance::OnCreateSessionComplete );
		//방 찾기시 완료 함수 콜백 등록
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject ( this , &UEFHGameInstance::OnFindSessionComplete );
		//방 입장시 함수 콜백 등록
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject ( this , &UEFHGameInstance::OnJoinSessionCompleted );
		//방 종료시 함수 콜백 등록
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject ( this , &UEFHGameInstance::OnMyExitSessionCompleted );

	

	}

}


void UEFHGameInstance::CreateMySession ( FString RoomName , int32 playerCount )
{
	ExitSession ();

	//방 이름 중복 방지
	MySessionName.Empty ();
	MySessionName.Append ( FString::Printf ( TEXT ( "_%d" ) , FMath::Rand32 () ) );

	FOnlineSessionSettings sessionSettings;
	//1. Dedicated 접속여부
	sessionSettings.bIsDedicated = false;
	//2. 랜선 매칭
	FName subsystemName = IOnlineSubsystem::Get ()->GetSubsystemName ();
	sessionSettings.bIsLANMatch = (subsystemName == "Null");

	//3.매칭 온라인
	sessionSettings.bShouldAdvertise = true;

	//4. 온라인 상태 여부
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;
	//5. 게임 진행 중 참여 허가 여부
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;
	//6. 세션에 참여할 수 있는 공개 (public) 연결 최대 허용수
	sessionSettings.NumPublicConnections = 2;

	//7. 커스텀 룸 네임 설정
	sessionSettings.Set ( FName ( "ROOM_NAME" ) , StringBase64Encode ( RoomName ) , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	//8. 호스트 네임
	sessionSettings.Set ( FName ( "HOST_NAME" ) , StringBase64Encode ( MySessionName ) , EOnlineDataAdvertisementType::ViaOnlineService );

	// 9. 게임 분류
	FString GAME_NAME = FString::Printf ( TEXT ( "EFH" ) );
	sessionSettings.Set ( FName ( "GAME_NAME" ) , GAME_NAME , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	//10. 커스텀 세션 정보 추가
	sessionSettings.Set ( FName ( "GAME_STATE" ) , 0 , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );

	// net ID
	FUniqueNetIdPtr netID = GetWorld ()->GetFirstLocalPlayerFromController ()->GetUniqueNetIdForPlatformUser ().GetUniqueNetId ();


	PRINTLOG ( TEXT ( "Create Session Start : %d" ) , *MySessionName );
	SessionInterface->CreateSession ( *netID , FName ( MySessionName ) , sessionSettings );

}

void UEFHGameInstance::ServerMapReStart ()
{
	const FString TravelURL = MainLevel + TEXT ( "?listen" ); // 서버가 listen 상태 유지
	GetWorld ()->ServerTravel ( TravelURL );
}

void UEFHGameInstance::EndterFindRoom ()
{

	auto sr = SessionSearch->SearchResults;
	if (sr.Num () >= 1)
	{
		for (auto result : sr)
		{
			FString GameName;
			result.Session.SessionSettings.Get ( FName ( "GAME_NAME" ) , GameName );
			if (GameName.Equals ( "EFH" ))
			{

				result.Session.SessionSettings.bUseLobbiesIfAvailable = true;
				result.Session.SessionSettings.bUsesPresence = true;

				OnSearchState.Broadcast ( true );
				OnEnterSession.Broadcast ();
				SessionInterface->JoinSession ( 0 , FName ( MySessionName ) , result );
				return;
			}
		}

		OnSearchState.Broadcast ( false );
		return;
	
	}
	else
	{
		OnSearchState.Broadcast ( false );
	}



	
}

void UEFHGameInstance::OnCreateSessionComplete ( FName SessionName , bool bWasSuccessful )
{
	PRINTLOG ( TEXT ( "SessionName : %s, bWasSuccessful : %d" ) , *SessionName.ToString () , bWasSuccessful );

	OnCreateState.Broadcast ( bWasSuccessful );

	if (bWasSuccessful == true)
	{
		OnEnterSession.Broadcast ();
		//	///Script/Engine.World'/Game/LJW/Maps/levelproto.levelproto'
		//FString MainLevel = TEXT ( "/Game/LJW/Maps/levelproto" );
		GetWorld ()->ServerTravel ( MainLevel + (TEXT ("?listen" )) ); // TEXT에 URL이 들어감 
	}

}

void UEFHGameInstance::OnFindSessionComplete ( bool bWasSuccessful )
{
	OnSearchState.Broadcast ( bWasSuccessful );
	// 찾기 실패 시 
	if (bWasSuccessful == false)
	{
		PRINTLOG ( TEXT ( "Session search failed..." ) );
		return;
	}
	
	{
		EndterFindRoom ();
		return;
	}

	// 세션 검색 결과 배열
	//auto results = SessionSearch->SearchResults;
	//PRINTLOG ( TEXT ( "Search Result Cont : %d" ) , results.Num () );



	//// 정보를 가져온다. 
	//for (int i = 0; i < results.Num (); ++i)
	//{
	//	// 정보가 유효한지 체크
	//	auto sr = results[i];
	//	if (sr.IsValid () == false)
	//	{
	//		continue;
	//	}

	//	// for Decode
	//	FString roomName;
	//	FString hostName;
	//	FString GameName;

	//	FSessionInfo sessionInfo;
	//	sessionInfo.index = i;

	//
	//	sr.Session.SessionSettings.Get ( FName ( "ROOM_NAME" ) , roomName );

	//	sr.Session.SessionSettings.Get ( FName ( "HOST_NAME" ) , hostName );

	//	sr.Session.SessionSettings.Get ( FName ( "GAME_NAME" ) , GameName );
	//	if (!GameName.Equals ( "EFH" ))
	//	{
	//		continue;
	//	}

	//	sessionInfo.roomName = StringBase64Decode ( roomName );
	//	sessionInfo.hostName = StringBase64Decode ( hostName );

	//	int32 GameState = -1;
	//	sr.Session.SessionSettings.Get ( FName ( "GAME_STATE" ) , GameState );
	//	sessionInfo.gameState = GameState;
	//	// 입장 가능한 플레이어수 

	//	int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;

	//	// 현재 입장가능한 플레이어수 (최대 - 현재 입장가능한 수 )
	//	int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;

	//	sessionInfo.pingSpeed = sr.PingInMs;

	//	sessionInfo.playerCount = FString::Printf ( TEXT ( "(%d/%d)" ) , currentPlayerCount , maxPlayerCount );

	//	PRINTLOG ( TEXT ( "%s" ) , *sessionInfo.ToString () );

	//	OnSearchCompleted.Broadcast ( sessionInfo );
	//}
}

void UEFHGameInstance::FindOtherSession ()
{
	//OnSearchState.Broadcast ( true );
	PRINTLOG ( TEXT ( "FindOtherSession..." ) );
	SessionSearch = MakeShareable ( new FOnlineSessionSearch () );

	// 1.세션 검색 조건 설정
	FString GAME_NAME = FString::Printf ( TEXT ( "EFH" ) );

	SessionSearch->QuerySettings.Set ( SEARCH_PRESENCE , true , EOnlineComparisonOp::Equals );
	SessionSearch->QuerySettings.Set ( FName ( "GAME_NAME" ) , GAME_NAME , EOnlineComparisonOp::Equals );

	// 2. Lan 여부
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get ()->GetSubsystemName () == FName ( "NULL" );

	// 3. 최대 검색 세션 수 
	SessionSearch->MaxSearchResults = 4;

	// 4. 세션 검색여부
	SessionInterface->FindSessions ( 0 , SessionSearch.ToSharedRef () );
}


void UEFHGameInstance::JoinSelectedSession ( int32 Index )
{
	auto sr = SessionSearch->SearchResults;

	sr[Index].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	sr[Index].Session.SessionSettings.bUsesPresence = true;


	SessionInterface->JoinSession ( 0 , FName ( MySessionName ) , sr[Index] );
}

void UEFHGameInstance::OnJoinSessionCompleted ( FName SessionName , EOnJoinSessionCompleteResult::Type Result )
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto pc = GetWorld ()->GetFirstPlayerController ();

		FString url;
		SessionInterface->GetResolvedConnectString ( SessionName , url );
		MySessionName = SessionName.ToString();

		PRINTLOG ( TEXT ( "Join URL : %s" ) , *url );

		OnEnterSession.Broadcast ();
		if (!url.IsEmpty ())
		{
			pc->ClientTravel ( url , ETravelType::TRAVEL_Absolute );
		}
	}
	else
	{
		PRINTLOG ( TEXT ( "Join Session failed : %d" ) , Result );
	}
}

void UEFHGameInstance::ExitSession ()
{
	if (SessionInterface.IsValid ())
		if (SessionInterface->GetNamedSession ( FName ( *MySessionName ) ))
		{
			SessionInterface->DestroySession ( FName ( *MySessionName ) );
		}
	
}

void UEFHGameInstance::OnMyExitSessionCompleted ( FName SessionName , bool bWasSuccessful )
{
	auto pc = GetWorld ()->GetFirstPlayerController ();
	///Script/Engine.World'/Game/SSA/Map/LobbyMap.LobbyMap'
	///Script/Engine.World'/Game/Level/LobbyLevel.LobbyLevel'	/

	FString url = TEXT ( "/Game/Level/LobbyLevel" );
	pc->ClientTravel ( url , TRAVEL_Absolute );


	ExitSession ();
}

void UEFHGameInstance::GameExit ()
{
}

FString UEFHGameInstance::StringBase64Encode ( const FString& Str )
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArry<uint8> -> base64로 Encode 

	std::string utf8String = TCHAR_TO_UTF8 ( *Str );
	TArray<uint8> arrayData = TArray<uint8> ( (uint8*)utf8String.c_str () , utf8String.length () );

	return FBase64::Encode ( arrayData );
}

FString UEFHGameInstance::StringBase64Decode ( const FString& Str )
{
	// Get 할 때 ( 서버에서 방 정보들을 받아올 때)
	// : base64로 Decode -> TArray<uint8> -> TCHAR 
	TArray<uint8> arrayData;
	FBase64::Decode ( Str , arrayData );
	std::string utf8String ( (char*)(arrayData.GetData ()) , arrayData.Num () );

	return UTF8_TO_TCHAR ( utf8String.c_str () );
}

