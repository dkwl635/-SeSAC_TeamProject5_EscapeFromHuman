// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Game/EFHPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "KJW/ActorComponent/UIActorComponent.h"
#include "KJW/UI/AnimalMainUI.h"
#include "KJW/Game/EFHPlayerState.h"
#include "KJW/Game/EFHGameState.h"
#include "LJW/PlayerGameMode.h"
#include "GameFramework/SpectatorPawn.h"

#include "GameFramework/GameMode.h"
#include "KJW/Game/EFHGameMode.h"


AEFHPlayerController::AEFHPlayerController ()
{
	UIActorComp = CreateDefaultSubobject<UUIActorComponent> ( TEXT ( "UIActorComp" ) );
}

void AEFHPlayerController::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( AEFHPlayerController , PlayerIndex );
}

void AEFHPlayerController::BeginPlay ()
{
	Super::BeginPlay ();


	if (HasAuthority ())
	{
		OnRep_PlayerState ();
	}

	
	gm = Cast<APlayerGameMode>(GetWorld()->GetAuthGameMode());
	
	AEFHGameMode* GM = GetWorld ()->GetAuthGameMode <AEFHGameMode> ();
	if (GM)
	{
		PlayerIndex = GM->Player;
		GM->Player++;
	}
	
	SetShowMouseCursor ( true );

	AAnimalBase* AnimalBase = Cast<AAnimalBase>(GetPawn());
	if (AnimalBase)
	{
		TeamAnimal = AnimalBase;
		TeamId = FGenericTeamId(TeamAnimal->ID);
	}
}

void AEFHPlayerController::OnRep_PlayerState ()
{
	Super::OnRep_PlayerState ();

	if (PS) return;

	PS = GetPlayerState<AEFHPlayerState> ();
	if (IsLocalController ())
	{
		if (PS)
		{
			AnimalUI->BindToPlayerState ( PS ); // 사용자 정의 함수
			
			PS->OnSelectAnimal.AddUObject(this , &ThisClass::GameStartEvent );
		}
	}

}

void AEFHPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetGameState();
}

ETeamAttitude::Type AEFHPlayerController::GetTeamAttitudeTowards(const AActor* Other) const
{
	const APawn* OtherPawn = Cast<APawn>(Other);
	if (OtherPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	}
	auto PlayerTI = Cast<IGenericTeamAgentInterface>(Other);
	class IGenericTeamAgentInterface* BotTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (BotTI == nullptr && PlayerTI == nullptr)
	{
		return ETeamAttitude::Neutral;
	}
	FGenericTeamId OtherActorTeamId = NULL;
	if (BotTI != nullptr)
	{
		OtherActorTeamId = BotTI->GetGenericTeamId();
	}
	else if (PlayerTI != nullptr)
	{
		OtherActorTeamId = PlayerTI->GetGenericTeamId();
	}

	FGenericTeamId ThisId = GetGenericTeamId();
	if (OtherActorTeamId == 8)				// TeamId가 8이면 중립.
	{
		return ETeamAttitude::Neutral;
	}
	else if (OtherActorTeamId == ThisId)			// TeamId가 같으면 같은 팀.
	{
		return ETeamAttitude::Friendly;
	}
	else							// TeamId가 다르면 적.
	{
		return ETeamAttitude::Hostile;
	}
}

void AEFHPlayerController::SetGameState()
{
	//if (!IsLocalController()) return;
	if (GS) return;

	GS = GetWorld()->GetGameState<AEFHGameState>();

	if (GS && AnimalUI)
	{
		AnimalUI->BindToGameState(GS);

		GS->OnReadyStateEvent.AddUObject ( this , &ThisClass::GameStartEvent );
	}
}

void AEFHPlayerController::GameStartEvent ()
{
	SetShowMouseCursor ( false );



}

void AEFHPlayerController::ServerRPC_SpawnActorAndPossess_Implementation ( int32 AnimalIndex )
{
	AEFHGameMode* GM = GetWorld ()->GetAuthGameMode <AEFHGameMode>();
	if (GM)
	{
		UnPossess ();

		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		TArray<AActor*> SpawnLoc = GS->GetWationPoint ();
		
		//TSubclassOf<AAnimalBase> animalClass = GM->GetAnimalClass ( AnimalIndex );
		TSubclassOf<AAnimalBase> animalClass = GS->AnimalsClass[AnimalIndex];
		if (animalClass)
		{
			FVector waitingpoint;
			if (PS->GameTeam == EGameTeam::A)
			{
				waitingpoint = SpawnLoc[0]->GetActorLocation ();
			}
			else
			{
				waitingpoint = SpawnLoc[1]->GetActorLocation ();
			}
			AAnimalBase* animal = GetWorld ()->SpawnActor<AAnimalBase> ( animalClass , waitingpoint , FRotator::ZeroRotator , Params);
			if (animal)
			{
				Possess ( animal );
			}
		}
	}
}

void AEFHPlayerController::ServerRPC_RespawnPlayer_Implementation ()
{
	auto player = GetPawn ();
	UnPossess ();
	player->Destroy ();
	if (gm)
	{
		gm->RestartPlayer ( this );
	}
}

void AEFHPlayerController::ServerRPC_ChangeToSpectator_Implementation ()
{
	auto player = GetPawn ();

	if (player)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto spectator = GetWorld ()->SpawnActor<ASpectatorPawn> ( gm->SpectatorClass , player->GetActorTransform () , params );
		Possess ( spectator );
		player->Destroy ();
		FTimerHandle handle;
		GetWorldTimerManager ().SetTimer ( handle , this , &AEFHPlayerController::ServerRPC_RespawnPlayer_Implementation , 5.0 , false);

	}
}
