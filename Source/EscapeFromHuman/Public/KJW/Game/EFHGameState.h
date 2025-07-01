// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KJW/GameStateType.h"
#include "EFHGameState.generated.h"



DECLARE_MULTICAST_DELEGATE ( FChangeState  );


UCLASS()
class ESCAPEFROMHUMAN_API AEFHGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	
	AEFHGameState ();
	// Called when the game starts or when spawned
	virtual void BeginPlay () override;

public:
	//던지는 아이템 리스트 셋팅
	void SetThrowItems();
	void AddThrowItem ( class AThrowItem* Item );
	//스폰된 동물 리스트 셋팅
	void SetAnimals ();
	//던지기 아이템 삭제 -> 풀링으로 사용 예정
	void DestroyThrowItem ( class AThrowItem* Item);


	TArray<class AThrowItem*>& GetThrowItems () { return ThrowItems; }
	TArray<class AAnimalBase*>& GetAnimals () { return Animals; }
	float GetTimerDuration () {return TimerDuration;}

	//현제 게임상태
	EGameState GetGameState () {return GameState;}
	//게임 상태 변경 -> 서버에서만 호출
	void ChangeGameState ( EGameState NewGameState );
	//현제 게임 상태 텍스트로 리턴
	FString GetGameStateString ();
	void StartTimer (  );
	//게임 남은 시간 
	float GetRemainingTime ();
	//남은 대기 시간
	float GetRemainingWatingTime ();

	//맵 다시 시작
	void OpenNewLevel ();

	void SetTeamScore ( EGameTeam Team , int32 Score );

	void ChangeReadyPlayer ();

	//시작 대기 위치
	TArray<AActor*>& GetWationPoint(){ return WaitingPoint; }

	float GetWaitingTime () {return ReadyWatingTime;}

public :
	FChangeState OnReadyStateEvent;
	FChangeState OnStartStateEvent;
	FChangeState OnEndStateEvent;
	FChangeState OnChangeTeamScoreEvent;
	FChangeState OnChangePlayerReady;
	FChangeState OnMoveLevel;

public:
	UPROPERTY ( ReplicatedUsing = OnRep_TeamScore )
	int32 TeamAScore = 0;
	UPROPERTY ( ReplicatedUsing = OnRep_TeamScore )
	int32 TeamBScore = 0;

	UPROPERTY ()
	TArray<AActor*> SpawnPoint;

	UPROPERTY ( EditDefaultsOnly )
	TArray<TSubclassOf< class AAnimalBase>> AnimalsClass;
protected:
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
private :
	TArray<class AThrowItem*> ThrowItems;
	TArray<class AAnimalBase*> Animals;



	//게임 플레이 시간 체크
	// 타이머 시작 시점 (서버 시간 기준)
	UPROPERTY ( VisibleAnywhere, Replicated )
	float TimerStartTime = -1.0f;
	UPROPERTY ( VisibleAnywhere  )
	float ServerTimeOffset = -1.0f;
	//타이머 길이
	UPROPERTY (EditDefaultsOnly, Replicated )
	float TimerDuration = 60.0f;
	//남은 시간
	UPROPERTY ( VisibleAnywhere )
	float RemainingTimer = 0.0f;
	//게임 플레이 시간 체크
	 
	
	//대기 시간
	UPROPERTY ( VisibleAnywhere , Replicated )
	float StartReadyTime = -1.0f;
	UPROPERTY ( VisibleAnywhere )
	float ServerReadyTimeOffset = -1.0f;
	UPROPERTY ( EditDefaultsOnly , Replicated )
	float ReadyWatingTime = 10.0f;
	UPROPERTY ( VisibleAnywhere )
	float RemainingReadyTimer = 0.0f;
	//대기 시간 동기화

	UPROPERTY ( VisibleAnywhere , ReplicatedUsing = OnRep_GameState )
	EGameState GameState = EGameState::READY;

	//FVector WaitingPoint;
	UPROPERTY()
	TArray<AActor*> WaitingPoint;

	FTimerHandle CheckAnimlsHandle;

	//시간에 따른 머티리얼 변화
	UPROPERTY()
	class UMaterialInstanceDynamic* PPI;
	UPROPERTY()
	class ADirectionalLight* SunLight;

	UPROPERTY(EditDefaultsOnly)
	FRotator SunRise;
	UPROPERTY(EditDefaultsOnly)
	FRotator SunSet;

protected:
	//게임 -> 현제 서버에서 호출됨
	void GameReadyAction ();
	void GameStartAction ();
	void GameEndAction ();
	void SetGameResult ();

	void SetSun_Color ();
	

protected:
	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_SyncServerTime ( float ServerTime );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_SyncServerReadyTime ( float ServerTime );

	UFUNCTION ()
	void OnRep_GameState ();
	UFUNCTION ()
	void OnRep_TeamScore ();
	
	void CheckAnimals ();

	void MovePlayer ();
	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_MoveLevelEvent ();
};
