// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Actor/ScoreSpawn.h"
#include "EscapeFromHuman.h"
#include "Components/BoxComponent.h"

#include "Algo/RandomShuffle.h"
#include "KJW/Actor/ScoreActor.h"
#include "KJW/Game/EFHGameState.h"
#include "KJW/Actor/ScoreSpawnPoint.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AScoreSpawn::AScoreSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	BoxComp = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "BoxComp" ) );
	SetRootComponent ( BoxComp );

	BoxComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

}

// Called when the game starts or when spawned
void AScoreSpawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority ())
	{
		GS = GetWorld ()->GetGameState<AEFHGameState> ();
		if (GS)
		{
		
			UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AScoreSpawnPoint::StaticClass () , Points );
			

			GS->OnStartStateEvent.AddUObject ( this , &AScoreSpawn::StartSpawnScore );
			//GS->OnEndStateEvent.AddUObject ( this , &AScoreSpawn::StopSpawnScore );


			PRINTLOG ( TEXT ( "Find GS" ) );
		}
	}
	else
	{
		Destroy ();
	}
	
}

// Called every frame
void AScoreSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreSpawn::StartSpawnScore ()
{
	SpawnScore ();
	GetWorldTimerManager ().SetTimer ( SpawnTimerHandle , this , &AScoreSpawn::SpawnScore , ScoreSpawnTimer , true );

}

void AScoreSpawn::SpawnScore ()
{
	TArray<AActor*> CopyPoint = Points;

	Algo::RandomShuffle ( Points );


	for (int32 i = 0; i < ScoreSpawnCount; ++i)
	{
		AActor* point = Points[i];


		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator rot = FRotator::ZeroRotator;
		//FVector pos = GetSpawnPointRandeom ();

		FVector pos = point->GetActorLocation ();


		PRINTLOG ( TEXT ( "StartSpawnScore" ) );
		AScoreActor* spawnScore = GetWorld ()->SpawnActor<AScoreActor> ( ScoreActorClass , pos , rot , param );
		spawnScore->OnReturnObj.BindUObject ( this , &AScoreSpawn::ReturnScore );
		SpawnedScore.Add ( spawnScore );

	}
}

void AScoreSpawn::StopSpawnScore ()
{
	for (int32 i = 0; i < SpawnedScore.Num (); ++i)
	{
		if (IsValid ( SpawnedScore[i] ))
		{
			SpawnedScore[i]->Destroy ();
		}
	}
	SpawnedScore.Empty ();

	GetWorldTimerManager ().ClearTimer ( SpawnTimerHandle );
}

void AScoreSpawn::ReturnScore ( AScoreActor* ScoreActor )
{
	SpawnedScore.Remove ( ScoreActor );
	ScoreActor->Destroy ();
	//if (SpawnedScore.Num () <= 0)
	//{
	//	StartSpawnScore ();
	//}
}

FVector AScoreSpawn::GetSpawnPointRandeom ()
{
	FVector boxExtent =  BoxComp->GetUnscaledBoxExtent ();

	FVector actorPos = GetActorLocation ();

	float x = FMath::RandRange ( actorPos.X + boxExtent.X , actorPos.X - boxExtent.X );
	float y = FMath::RandRange ( actorPos.Y + boxExtent.Y , actorPos.Y - boxExtent.Y );


	return FVector (x, y, 70.0f);
}

