// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KJW/GameStateType.h"
#include "EFHPlayerState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE ( FChangeValue )
DECLARE_MULTICAST_DELEGATE ( FGameEndEvent )

UCLASS()
class ESCAPEFROMHUMAN_API AEFHPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	AEFHPlayerState ();
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

public:

	UPROPERTY ( VisibleAnywhere , Replicated )
	EGameTeam GameTeam = EGameTeam::MAX;

	//홈 점수 추가하기
	void AddScore ( int32 AddScore );
	int32 GetLocalScore () { return LocalScore; }

	//들고 다니는 점수
	void AddTempScore ( int32 AddScore );
	int32 GetTempScore () {return TempScore;}

	//레디 상태 변화 
	void SetRaedy (bool bNewReady);
	bool IsReady () { return bReady; }
	bool IsSetAnimal () { return bSetAnimal; }
	FString GetGameResultStr ();
public:
	void SetAnimal ();

	UFUNCTION ( Client , Reliable )
	void ClientRPC_SetGameResult ( const TArray<FString>& Winners );
	
	void SetGameTeamScore ();

	
public:
	FChangeValue OnChangeScore;
	FChangeValue OnChangeTeamScore;
	FGameEndEvent OnEndEvent;
	FChangeValue OnSelectAnimal;
	FChangeValue OnReadyEvent;


	
protected:
	virtual void BeginPlay () override;
	virtual void OnRep_Score () override;
	UFUNCTION()
	void OnRep_TempScore ();
	

protected:
	//게임 준비 상태
	bool bReady = false;


	bool IsLocalController = false;
	int32 LocalScore = 0;

	UPROPERTY ( ReplicatedUsing = OnRep_TempScore )
	int32 ServerTempScore = 0;

	int32 TempScore = 0;
	
	UPROPERTY()
	class AEFHGameState* GS;

	//0 Win, 1 lose, 2 Draw
	int32 GameResult = -1;

	UPROPERTY(Replicated)
	bool bSetAnimal = false;


protected:

	//실제 홈점수 추가는 서버에서만 
	UFUNCTION ( Server , Reliable )
	void ServerRPC_AddScore ( int32 AddScore );

	UFUNCTION ( Client , Reliable )
	void ClientRPC_AddScore ( int32 AddScore );

	//실제 임시점수 추가는 서버에서만 
	UFUNCTION ( Server , Reliable )
	void ServerRPC_AddTempScore ( int32 AddScore );

	UFUNCTION ( Client , Reliable )
	void ClientRPC_AddTempScore ( int32 AddScore );


	UFUNCTION ( Server , Reliable )
	void ServerRPC_SetReady ( bool bNewReady );
	

	UFUNCTION ( Server , Reliable )
	void ServerRPC_SetAnimal (  );
};
