// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "EFHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API AEFHPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	AEFHPlayerController ();
protected:
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void BeginPlay () override;
	virtual void OnRep_PlayerState () override;
	virtual void Tick ( float DeltaSeconds ) override;

	FGenericTeamId TeamId;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor* Other) const;
public:

	UPROPERTY ( Replicated )
	int32 PlayerIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	class AAnimalBase* TeamAnimal;

	virtual FGenericTeamId GetGenericTeamId() const {return TeamId;}

	UPROPERTY ()
	class UAnimalMainUI* AnimalUI;
protected:
	UPROPERTY ( EditDefaultsOnly )
	class UUIActorComponent* UIActorComp;

private:


	UPROPERTY ()
	class AEFHPlayerState* PS;
	UPROPERTY ()
	class AEFHGameState* GS;



private:
	void SetGameState ();

	void GameStartEvent ();
public:
	UPROPERTY()
	class APlayerGameMode* gm;

	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeToSpectator();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnActorAndPossess (int32 AnimalIndex);

};
