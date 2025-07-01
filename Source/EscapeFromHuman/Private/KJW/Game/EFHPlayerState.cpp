// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/Game/EFHPlayerState.h"
#include "KJW/Game/EFHGameState.h"
#include "Net/UnrealNetwork.h"
#include "EscapeFromHuman.h"
#include "LJW/AnimalBase.h"
#include "GameFramework/PlayerState.h"


AEFHPlayerState::AEFHPlayerState ()
{

	bReplicates = true;
}

void AEFHPlayerState::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME( AEFHPlayerState  , ServerTempScore );
	DOREPLIFETIME( AEFHPlayerState  , bSetAnimal );
}

void AEFHPlayerState::BeginPlay ()
{
	Super::BeginPlay ();

	const AController* OwnerController = Cast<AController> ( GetOwner () );
	if (OwnerController)
	{
		IsLocalController = OwnerController->IsLocalController ();
	}

		GS = GetWorld ()->GetGameState<AEFHGameState> ();
		if (GS)
		{
			if (HasAuthority ())
			{
				PRINTLOG ( TEXT ( "PlayerArray : %d" ) , GS->PlayerArray.Num () );
				if (GS->PlayerArray.Num () == 1)
				{
					GameTeam = EGameTeam::A;
					bReady = false;
				}
				else
				{
					GameTeam = EGameTeam::B;
					bReady = false;
				}
			}

			if (IsLocalController)
			{
				GS->OnChangeTeamScoreEvent.AddUObject ( this , &AEFHPlayerState::SetGameTeamScore );
			}
		}

	
	

}



void AEFHPlayerState::AddScore ( int32 AddScore )
{
	if (!GS) return;

	if (GS->GetGameState () != EGameState::GAME) return;

	ClientRPC_AddScore ( AddScore );
	ServerRPC_AddScore ( AddScore );
}

void AEFHPlayerState::AddTempScore ( int32 AddScore )
{
	ClientRPC_AddTempScore ( AddScore );
	ServerRPC_AddTempScore ( AddScore );
}

void AEFHPlayerState::SetRaedy ( bool bNewReady )
{
	if (bReady) return;

	bReady = bNewReady;
	OnReadyEvent.Broadcast ();
	ServerRPC_SetReady ( bNewReady );
}

FString AEFHPlayerState::GetGameResultStr ()
{
	switch (GameResult)
	{
	case 1 :
		return TEXT ( "승리" );
	case 2:
		return TEXT ( "패배" );
	case 3:
		return TEXT ( "무승부" );
	default:
		return TEXT ( "뭐시여 " );
	}
}

void AEFHPlayerState::SetAnimal ()
{
	bSetAnimal = true;

	OnSelectAnimal.Broadcast ();

	ServerRPC_SetAnimal ();
}

void AEFHPlayerState::SetGameTeamScore ()
{
	PRINTLOG ( TEXT ( "SetGameTeamScore" ) );
	OnChangeTeamScore.Broadcast ();
	//ClientRPC_SetGameTeamScroe ();
}


void AEFHPlayerState::OnRep_Score ()
{
	Super::OnRep_Score ();
	int32 CurrentScore =  GetScore();

	OnChangeScore.Broadcast ();

}

void AEFHPlayerState::OnRep_TempScore ()
{
	TempScore = ServerTempScore;
	//OnChangeScore.Broadcast ();
	OnChangeScore.Broadcast ();
}

void AEFHPlayerState::ServerRPC_SetAnimal_Implementation ()
{
	bSetAnimal = true;
}


void AEFHPlayerState::ServerRPC_AddTempScore_Implementation ( int32 AddScore )
{
	ServerTempScore = ServerTempScore +  AddScore;
	TempScore = ServerTempScore;

	OnRep_Score ();
}

void AEFHPlayerState::ClientRPC_AddTempScore_Implementation ( int32 AddScore )
{
	TempScore = TempScore + AddScore;
	
	if (IsLocalController)
	{
		OnChangeScore.Broadcast ();
	}
}


void AEFHPlayerState::ClientRPC_AddScore_Implementation ( int32 AddScore )
{
	LocalScore += AddScore;

	if (IsLocalController)
	{
		OnChangeScore.Broadcast ();
	}
}

void AEFHPlayerState::ServerRPC_AddScore_Implementation ( int32 AddScore )
{
	int32 newScore = GetScore () + AddScore;
	SetScore ( newScore );

	if (GS)
	{
		GS->SetTeamScore ( GameTeam , newScore );
	}

	LocalScore = newScore;
	OnRep_Score ();

	
}

void AEFHPlayerState::ClientRPC_SetGameResult_Implementation ( const TArray<FString>& Winners )
{
	FString uniqueId = GetUniqueId()->ToString ();
	if (Winners.Contains ( uniqueId ))
	{
		if (Winners.Num () >= 2)
		{
			PRINTLOG ( TEXT ( "Game Draw" ) );
			GameResult = 3;
		}
		else
		{
			PRINTLOG ( TEXT ( "Game Win" ) );
			GameResult = 1;
		}
	}
	else
	{
		PRINTLOG ( TEXT ( "Game Lose" ) );
		GameResult = 2;
	}

	OnEndEvent.Broadcast ();

}


void AEFHPlayerState::ServerRPC_SetReady_Implementation ( bool bNewReady )
{
	bReady = bNewReady;

	if (GS)
	{
		GS->ChangeReadyPlayer ();
	}
}
