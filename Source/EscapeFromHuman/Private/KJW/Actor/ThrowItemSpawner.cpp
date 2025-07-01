// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Actor/ThrowItemSpawner.h"
#include "EscapeFromHuman.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

#include "KJW/Actor/ThrowItem.h"
#include "KJW/Game/EFHGameState.h"
#include "NiagaraComponent.h"
// Sets default values
AThrowItemSpawner::AThrowItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	SetRootComponent ( SphereComp );

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	MeshComp->SetupAttachment ( GetRootComponent () );

	ItemSpawnPoint = CreateDefaultSubobject<USceneComponent> ( TEXT ( "ItemSpawnPoint" ) );
	ItemSpawnPoint->SetupAttachment ( GetRootComponent () );

	NS_Spawner = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "NS_Spawner" ) );
	NS_Spawner->SetupAttachment ( GetRootComponent () );

	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	SphereComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	bReplicates = true;
}

// Called when the game starts or when spawned
void AThrowItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority ())
	{
		GS = GetWorld ()->GetGameState<AEFHGameState> ();
		if (GS)
		{
			if (bTutorial)
			{
				SpawnItem ();
				GS->OnReadyStateEvent.AddUObject ( this , &ThisClass::ClearItem );
			}
			else
			{
				GS->OnStartStateEvent.AddUObject ( this , &ThisClass::SpawnItem );
			}

		}
	}

}

void AThrowItemSpawner::Tick ( float DeltaSeconds )
{
	Super::Tick ( DeltaSeconds );
	if (!HasAuthority ()) return;

	if (SpawnedItem)
	{
		ElapsedTime += DeltaSeconds;
		ElapsedTime = FMath::Fmod ( ElapsedTime , 2 * PI );

		// 진동 범위와 속도 설정
		const float Amplitude = 30.0f;      // 최대 위아래 거리
		const float Frequency = 0.5f;       // 초당 왕복 횟수 (1이면 1초에 위-아래 한 번)

		// 새로운 위치 계산
		FVector NewLocation = ItemSpawnPoint->GetComponentLocation();
		NewLocation.Z += FMath::Sin ( ElapsedTime * Frequency * 2 * PI ) * Amplitude;

		// 위치 적용
		SpawnedItem->SetActorLocation ( NewLocation );
	}
}



void AThrowItemSpawner::SpawnItem ()
{

	if (Items.Num () <= 0)return;

	if (SpawnedItem && SpawnedItem->OnAddItem.IsBound())
	{
		SpawnedItem->OnAddItem.Unbind ();
	}


	int32 rand = FMath::RandRange ( 0 , Items.Num () - 1 );
	TSubclassOf<AThrowItem> ItemClass = Items[rand];

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AThrowItem* spawnItem = GetWorld ()->SpawnActor<AThrowItem> ( ItemClass , ItemSpawnPoint->GetComponentTransform () , param );
	if (spawnItem)
	{
		SpawnedItem = spawnItem;
		SpawnedItem->OnAddItem.BindUObject ( this , &AThrowItemSpawner::UsingItem );

		if (GS)
		{
			GS->AddThrowItem ( SpawnedItem );
		}
	}

	MulticastRPC_ShowEffect ( true );

}

void AThrowItemSpawner::UsingItem ()
{
	PRINTLOG ( TEXT ( "Add Success" ) );

	SpawnedItem->OnAddItem.Unbind ();
	SpawnedItem = nullptr;

	MulticastRPC_ShowEffect ( false );


	GetWorld ()->GetTimerManager ().SetTimer ( SpawnerTimerHandle , this , &AThrowItemSpawner::SpawnItem , SpawnerDelay , false );
}

void AThrowItemSpawner::ClearItem ()
{
	if (SpawnerTimerHandle.IsValid ())
	{
		GetWorld ()->GetTimerManager ().ClearTimer ( SpawnerTimerHandle );
	}

	if (SpawnedItem && SpawnedItem->OnAddItem.IsBound ())
	{
		SpawnedItem->OnAddItem.Unbind ();
		SpawnedItem->Destroy ();
		SpawnedItem = nullptr;
	

		MulticastRPC_ShowEffect ( false );
	}

}

void AThrowItemSpawner::MulticastRPC_ShowEffect_Implementation ( bool bShow )
{
	NS_Spawner->SetVisibility ( bShow );
	if (bShow)
	{
		NS_Spawner->Activate ();
	}
	else
	{
		NS_Spawner->Deactivate ();
	}
}

