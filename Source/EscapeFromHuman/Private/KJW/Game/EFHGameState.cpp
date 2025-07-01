// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Game/EFHGameState.h"
#include "Net/UnrealNetwork.h"
#include "EscapeFromHuman.h"

#include "KJW/Actor/ThrowItem.h"
#include "LJW/AnimalBase.h"
#include "Kismet/GameplayStatics.h"

#include "KJW/Game/EFHPlayerState.h"
#include "KJW/Game/EFHPlayerController.h"
#include "KJW/Game/EFHGameInstance.h"
#include "KJW/Actor/WaitingPoint.h"
#include "KJW/Actor/HomeActor.h"

#include "Engine/PostProcessVolume.h"
#include "Engine/DirectionalLight.h"
#include "Materials/MaterialInstanceDynamic.h"

AEFHGameState::AEFHGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	TimerStartTime = -1.f;
	TimerDuration = 0.f;

	SpawnPoint.Init ( nullptr , 2 );
	WaitingPoint.Init ( nullptr , 2 );
}

void AEFHGameState::BeginPlay ()
{
	Super::BeginPlay ();
	
	RemainingTimer = TimerDuration;
	RemainingReadyTimer = ReadyWatingTime;

	if (HasAuthority ())
	{
		//ChangeGameState ( EGameState::READY );

		GetWorldTimerManager ().SetTimer ( CheckAnimlsHandle , this , &ThisClass::CheckAnimals , 1.0f , true );
	}
	
	/*AActor* find = 	UGameplayStatics::GetActorOfClass ( GetWorld () , AWaitingPoint::StaticClass () );
	if (find)
	{
		WaitingPoint = find->GetActorLocation ();

	}*/

	TArray<AActor*> find;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AWaitingPoint::StaticClass () , find );
	if (find.Num() > 0)
	{
		for (AActor* actor : find)
		{
			AWaitingPoint* Waiting = Cast<AWaitingPoint> ( actor );
			if (Waiting)
			{
				WaitingPoint[Waiting->Index] = Waiting;
			}
		}
	}

	TArray<AActor*> finds;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AHomeActor::StaticClass () , finds);
	
	if (finds.Num () > 0)
	{
		for (AActor* actor : finds)
		{
			AHomeActor* home = Cast<AHomeActor> ( actor );
			if (home && home->Index < 2)
			{
				SpawnPoint[home->Index] = home;
			}
		}
	}

}

void AEFHGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEFHGameState, TimerStartTime);
	DOREPLIFETIME(AEFHGameState, TimerDuration);
	DOREPLIFETIME(AEFHGameState, ReadyWatingTime );
	DOREPLIFETIME(AEFHGameState, StartReadyTime );
	DOREPLIFETIME(AEFHGameState, GameState);
	DOREPLIFETIME(AEFHGameState, TeamAScore );
	DOREPLIFETIME(AEFHGameState, TeamBScore );
}


void AEFHGameState::GameReadyAction ()
{
	PRINTLOG ( TEXT ( __FUNCTION__ ) );

	//SetThrowItems ();

	//대기 시간 타이머 시작 해야 함
	//시간 시작 끝나고 게임 시작되게 해야함

	if (HasAuthority ())
	{
		MulticastRPC_SyncServerReadyTime ( GetWorld ()->GetTimeSeconds () );
		StartReadyTime = GetWorld ()->GetTimeSeconds (); // 서버 시간
		//PRINTLOG ( TEXT ( "Timer started on server: %f (Duration: %f)" ) , TimerStartTime , TimerDuration );
	}

	//플레이어 이동
	FTimerHandle MovePlayerHandle;
	GetWorldTimerManager ().SetTimer ( MovePlayerHandle , this ,&AEFHGameState::MovePlayer, ReadyWatingTime + 1.2f, false );

	//게임 단계를  게임 중으로 변경 시키기
	FTimerHandle GameStartHandle;
	GetWorldTimerManager ().SetTimer ( GameStartHandle , FTimerDelegate::CreateLambda ( [this] {
		ChangeGameState ( EGameState::GAME );
		}) , ReadyWatingTime + 7.3f , false);
}

void AEFHGameState::GameStartAction ()
{
	PRINTLOG ( TEXT ( __FUNCTION__ ) );
	StartTimer ();

	FTimerHandle TimerHandle;
	GetWorldTimerManager ().SetTimer ( TimerHandle , FTimerDelegate::CreateLambda ( [this] {
		ChangeGameState ( EGameState::END );
		} ) , TimerDuration + 3.0f, false);


}


void AEFHGameState::GameEndAction ()
{
	PRINTLOG ( TEXT ( __FUNCTION__ ) );
	SetGameResult ();


	GetWorld ()->GetFirstPlayerController ()->SetShowMouseCursor ( true );
}

void AEFHGameState::SetGameResult ()
{
	int maxScore = 0;
	TArray<FString> winners;

	for (APlayerState* player : PlayerArray)
	{
		int32 playerScore = player->GetScore ();
		FString UniqueId =  player->GetUniqueId ()->ToString ();
		PRINTLOG ( TEXT ( "Player UniqueID : %s" ) , *UniqueId );
		if (playerScore == maxScore)
		{
			winners.Add ( UniqueId );
		}
		else if(playerScore > maxScore)
		{
			maxScore = playerScore;
			winners.Empty ();
			winners.Add ( UniqueId );
		}
	}

	for (APlayerState* player : PlayerArray)
	{
		AEFHPlayerState* PS = Cast<AEFHPlayerState> ( player );
		if (PS)
		{
			PS->ClientRPC_SetGameResult ( winners );
		}

	}



}

void AEFHGameState::SetSun_Color ()
{
	float percent = 1 - (RemainingTimer / TimerDuration);
	

	if (PPI)
	{
		PPI->SetScalarParameterValue ( TEXT ( "Timer" ) , percent );
	}

	if (SunLight)
	{
		FRotator newRot = FMath::Lerp ( SunRise , SunSet , percent );
		SunLight->SetActorRotation ( newRot );
	}

}



void AEFHGameState::OnRep_GameState ()
{
	switch (GameState)
	{
	case EGameState::READY:
	{
		APostProcessVolume* pp = Cast<APostProcessVolume> ( UGameplayStatics::GetActorOfClass ( GetWorld () , APostProcessVolume::StaticClass () ) );
		if (pp && pp->Settings.WeightedBlendables.Array.Num() > 0)
		{
			FWeightedBlendable& blendable = pp->Settings.WeightedBlendables.Array[0];

			if (UMaterialInterface* material = Cast<UMaterialInterface> ( blendable.Object ))
			{
				PPI = Cast<UMaterialInstanceDynamic> ( material );
				if (!PPI)
				{
					PPI = UMaterialInstanceDynamic::Create ( material , this );
					blendable.Object = PPI; // 덮어씌워야 적용됨
				}
			}
		}

		SunLight = Cast<ADirectionalLight> ( UGameplayStatics::GetActorOfClass ( GetWorld () , ADirectionalLight::StaticClass () ) );

		if (SunLight) SunLight->SetActorRotation ( SunRise );

		OnReadyStateEvent.Broadcast ();
	}
		break;
	case EGameState::GAME:
	{	
		OnStartStateEvent.Broadcast ();
	}
		break;
	case EGameState::END:
	{
		OnEndStateEvent.Broadcast ();
	}
		break;
	case EGameState::MAX:
		break;
	default:
		break;
	}
	
}

void AEFHGameState::OnRep_TeamScore ()
{
	PRINTLOG ( TEXT ( "OnRep_TeamScore" ) );
	OnChangeTeamScoreEvent.Broadcast ();
}

void AEFHGameState::CheckAnimals ()
{
	if (PlayerArray.Num () < 2) return;

	bool gameStart = false;

	int32 readyCount = 0;

	for (APlayerState* player : PlayerArray)
	{
		AEFHPlayerState* PS = Cast<AEFHPlayerState> ( player );
		if (PS)
		{
			if(PS->IsReady())
			{ 
				readyCount++;
			}
			//if (!PS->IsSetAnimal ())
			//{
			//	gameStart= false;
			//	break;
			//
			//}
		}
	}

	if (readyCount == 2)
	{
		gameStart = true;
	}

	if (gameStart)
	{
		GetWorldTimerManager ().ClearTimer ( CheckAnimlsHandle );

		ChangeGameState ( EGameState::READY );
		//ChangeGameState ( EGameState::GAME );
	}
	
}

void AEFHGameState::MovePlayer ()
{
	PRINTLOG ( TEXT ( __FUNCTION__ ) );


	for (FConstPlayerControllerIterator Iterator = GetWorld ()->GetPlayerControllerIterator (); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get ();
		AEFHPlayerController* efhPC = Cast<AEFHPlayerController> ( PC );

		if (efhPC)
		{
			AActor* point = SpawnPoint[efhPC->PlayerIndex];
			PC->ClientSetRotation ( point->GetActorRotation () );
			efhPC->GetPawn ()->SetActorLocationAndRotation ( point->GetActorLocation () , point->GetActorRotation () );
		}
	}
}




void AEFHGameState::SetThrowItems ()
{
	ThrowItems.Empty ();

	TArray<AActor*> finds;
	UGameplayStatics::GetAllActorsOfClass( GetWorld () , AThrowItem::StaticClass () , finds );
	for (AActor* find : finds)
	{
		AThrowItem* item = Cast<AThrowItem> ( find );
		
		if (!item->GetOwner ())
			ThrowItems.Add ( item );
	}

}

void AEFHGameState::AddThrowItem ( AThrowItem* Item )
{
	if (ThrowItems.Contains ( Item )) return;

	ThrowItems.Add ( Item );
}

void AEFHGameState::SetAnimals ()
{
	Animals.Empty ();
	
	TArray<AActor*> finds;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AAnimalBase::StaticClass () , finds );
	for (AActor* find : finds)
	{
		AAnimalBase* Animal = Cast<AAnimalBase> ( find );
		Animals.Add ( Animal );
	}
}

void AEFHGameState::DestroyThrowItem ( AThrowItem* Item )
{
	if (ThrowItems.Contains ( Item ))
	{
		ThrowItems.Remove ( Item );
		Item->Destroy ();
	}
}

void AEFHGameState::ChangeGameState ( EGameState NewGameState )
{
	if (NewGameState == GameState) return;
	
	switch (NewGameState)
	{
	case EGameState::READY:
		GameReadyAction ();
		break;
	case EGameState::GAME:
		GameStartAction ();
		break;
	case EGameState::END:
		GameEndAction ();
		break;
	case EGameState::MAX:
		break;
	default:
		break;
	}
	
	
	GameState = NewGameState;

	OnRep_GameState ();
}

FString AEFHGameState::GetGameStateString ()
{
	switch (GameState)
	{
	case EGameState::READY:
		return TEXT ( "READY" );
	case EGameState::GAME:
		return TEXT ( "GAME" );
	case EGameState::END:
		return TEXT ( "END" );
	case EGameState::MAX:
	default:
		return TEXT ( "NONE" );
	}
}

void AEFHGameState::StartTimer()
{
	if (HasAuthority())
	{
		MulticastRPC_SyncServerTime ( GetWorld ()->GetTimeSeconds () );
		TimerStartTime = GetWorld()->GetTimeSeconds(); // 서버 시간
		PRINTLOG(TEXT("Timer started on server: %f (Duration: %f)"), TimerStartTime, TimerDuration);
	}
}

float AEFHGameState::GetRemainingTime()
{

	if (TimerDuration <= 0 || RemainingTimer<= 0.0f)
	{
		return 0.0f;
	}

	if (TimerStartTime < 0)
	{
		return RemainingTimer;
	}

	//현제 게임 시간
	const float currentTime = GetWorld()->GetTimeSeconds() + ServerTimeOffset;
	//실제 흐른 시간
	const float elapsed = currentTime - TimerStartTime;
	const float remaining = TimerDuration - elapsed;
	
	RemainingTimer = FMath::Clamp(remaining, 0.0f, TimerDuration);

	SetSun_Color ();

	return RemainingTimer;

}

float AEFHGameState::GetRemainingWatingTime ()
{
	if (ReadyWatingTime <= 0 || RemainingReadyTimer <= 0.0f)
	{
		return 0.0f;
	}	
	if (StartReadyTime <= 0)
	{
		return 0.0f;
	}


	//현제 게임 시간
	const float currentTime = GetWorld ()->GetTimeSeconds () + ServerReadyTimeOffset;
	//실제 흐른 시간
	const float elapsed = currentTime - StartReadyTime;
	const float remaining = ReadyWatingTime - elapsed;

	RemainingReadyTimer = FMath::Clamp ( remaining , 0.0f , ReadyWatingTime );

	return RemainingReadyTimer;
}

void AEFHGameState::OpenNewLevel ()
{
	if (UGameInstance* GI = GetGameInstance ())
	{
		if (UEFHGameInstance* MyGI = Cast<UEFHGameInstance> ( GI ))
		{
			MyGI->ServerMapReStart ();

			MulticastRPC_MoveLevelEvent ();
		}
	}
}

void AEFHGameState::SetTeamScore ( EGameTeam Team , int32 Score )
{
	if (Team == EGameTeam::A)
	{
		TeamAScore = Score;
	}
	else
	{
		TeamBScore = Score;

	}

	OnRep_TeamScore();
}

void AEFHGameState::ChangeReadyPlayer ()
{
	OnChangePlayerReady.Broadcast ();
}


void AEFHGameState::MulticastRPC_SyncServerTime_Implementation ( float ServerTime )
{
	const float ClientLocalTime = GetWorld ()->GetTimeSeconds ();
	ServerTimeOffset = ServerTime - ClientLocalTime;
}

void AEFHGameState::MulticastRPC_SyncServerReadyTime_Implementation ( float ServerTime )
{
	const float ClientLocalTime = GetWorld ()->GetTimeSeconds ();
	ServerReadyTimeOffset = ServerTime - ClientLocalTime;
}

void AEFHGameState::MulticastRPC_MoveLevelEvent_Implementation ()
{
	OnMoveLevel.Broadcast ();
}