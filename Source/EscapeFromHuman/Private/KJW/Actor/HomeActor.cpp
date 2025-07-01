// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/HomeActor.h"
#include "EscapeFromHuman.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "KJW/Game/EFHPlayerState.h"
#include "LJW/AnimalBase.h"

#include "KJW/Actor/ScoreActor.h"

// Sets default values
AHomeActor::AHomeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	SetRootComponent ( MeshComp );

	SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	SphereComp->SetupAttachment ( GetRootComponent () );


	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );

}

// Called when the game starts or when spawned
void AHomeActor::BeginPlay()
{
	Super::BeginPlay();
	
	//서버에서만 충돌 체크
	if (HasAuthority ())
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic ( this , &ThisClass::OnOverlapBeginComponent );
		SphereComp->OnComponentEndOverlap.AddDynamic ( this , &ThisClass::OnOverlapEndinComponent );
	}
	else
	{
		SphereComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	}
}

// Called every frame
void AHomeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHomeActor::OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if (!MyTeamAnimal)
	{
		MyTeamAnimal = Cast<AAnimalBase> ( OtherActor );
		if (MyTeamAnimal)
		{
			SetOwner ( MyTeamAnimal );
			PS = MyTeamAnimal->GetPlayerState<AEFHPlayerState> ();
		}
	}

	if (!PS) return;

	
	AScoreActor* scoreActor = Cast<AScoreActor> ( OtherActor );
	if (scoreActor)
	{
		if (MyScores.Contains ( scoreActor )) return;
		if (scoreActor->ScoreState == EScoreState::TRAKING|| scoreActor->ScoreState == EScoreState::TOP	)
		{
			if (scoreActor->PS == PS)
			{
				
				scoreActor->InHome ( GetHomePointRand () );
			
				PS->AddScore ( 1 );

				MyScores.Add ( scoreActor );
			}
		}
	}

	//if (MyTeamAnimal == OtherActor)
	//{
	//	PRINTLOG ( TEXT ( "MyTeamAnimal == OtherActor" ) );
	//	
	//	int32 animalTempScore = PS->GetTempScore ();
	//	PS->AddTempScore ( -animalTempScore );
	//
	//	PS->AddScore (animalTempScore);
	//}
}

void AHomeActor::OnOverlapEndinComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex )
{
	if (!MyTeamAnimal) return;
	if (!PS) return;

	AScoreActor* scoreActor = Cast<AScoreActor> ( OtherActor );
	if (scoreActor)
	{
		if (MyScores.Contains ( scoreActor ))
		{
			scoreActor->OutHome ();
			PS->AddScore ( -1 );
			MyScores.Remove ( scoreActor );
		}
	}
}

FVector AHomeActor::GetHomePointRand ()
{
	//FVector boxExtent = SphereComp->GetUnscaledBoxExtent ();
	/*FVector boxExtent = SphereComp->GetScaledBoxExtent ();

	FVector actorPos = GetActorLocation ();

	float x = FMath::RandRange ( actorPos.X + boxExtent.X , actorPos.X - boxExtent.X );
	float y = FMath::RandRange ( actorPos.Y + boxExtent.Y , actorPos.Y - boxExtent.Y );*/
	//return FVector ( x , y , 70.0f );


	float Radius = SphereComp->GetScaledSphereRadius ();
	FVector ActorPos = GetActorLocation ();

	// X/Y 평면에서 원 안의 랜덤 위치 가져오기
	FVector2D Random2D = FMath::RandPointInCircle ( Radius );

	return FVector ( ActorPos.X + Random2D.X , ActorPos.Y + Random2D.Y , 70.0f );
	
}

