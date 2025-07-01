// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Actor/ItemSpawner.h"
#include "EscapeFromHuman.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

#include "KJW/Actor/UsingItem.h"
#include "NiagaraComponent.h"

#include "KJW/Game/EFHGameState.h"

// Sets default values
AItemSpawner::AItemSpawner()
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
	NS_Spawner->SetupAttachment ( GetRootComponent() );

	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	SphereComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	bReplicates = true;
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
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

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority ()) return;

	if (SpawnedItem)
	{
		ElapsedTime += DeltaTime;
		ElapsedTime = FMath::Fmod ( ElapsedTime , 2 * PI );

		// 진동 범위와 속도 설정
		const float Amplitude = 30.0f;      // 최대 위아래 거리
		const float Frequency = 0.5f;       // 초당 왕복 횟수 (1이면 1초에 위-아래 한 번)

		// 새로운 위치 계산
		FVector NewLocation = ItemSpawnPoint->GetComponentLocation ();
		NewLocation.Z += FMath::Sin ( ElapsedTime * Frequency * 2 * PI ) * Amplitude;

		// 위치 적용
		SpawnedItem->SetActorLocation ( NewLocation );
	}

}

void AItemSpawner::SpawnItem ()
{
	if (Items.Num () <= 0)return;

	if (SpawnedItem && SpawnedItem->OnUsingItem.IsBound())
	{
		SpawnedItem->OnUsingItem.Unbind ();
	}


	int32 rand = FMath::RandRange ( 0 , Items.Num () -1);
	TSubclassOf<AUsingItem> ItemClass = Items[rand];

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AUsingItem* spawnItem = GetWorld ()->SpawnActor<AUsingItem> ( ItemClass , ItemSpawnPoint->GetComponentTransform () , param );
	if (spawnItem)
	{
		SpawnedItem = spawnItem;
		spawnItem->OnUsingItem.BindUObject ( this , &AItemSpawner::UsingItem );

	}


	MulticastRPC_ShowEffect ( true );
}



void AItemSpawner::UsingItem ()
{
	if (SpawnedItem && SpawnedItem->OnUsingItem.IsBound ())
	{
		SpawnedItem->OnUsingItem.Unbind ();
	}

	SpawnedItem = nullptr;
	MulticastRPC_ShowEffect ( false );


	GetWorld ()->GetTimerManager ().SetTimer ( SpawnerTimerHandle , this , &AItemSpawner::SpawnItem , SpawnerDelay , false );
}

void AItemSpawner::ClearItem ()
{
	if (SpawnerTimerHandle.IsValid ())
	{
		GetWorld ()->GetTimerManager ().ClearTimer ( SpawnerTimerHandle );
	}

	if (SpawnedItem && SpawnedItem->OnUsingItem.IsBound ())
	{
		SpawnedItem->OnUsingItem.Unbind ();

		SpawnedItem->Destroy ();
		SpawnedItem = nullptr;

	
		MulticastRPC_ShowEffect ( false );
	}
	
}

void AItemSpawner::MulticastRPC_ShowEffect_Implementation ( bool bShow )
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

