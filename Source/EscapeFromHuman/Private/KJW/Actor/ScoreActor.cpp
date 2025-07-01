// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/ScoreActor.h"
#include "Net/UnrealNetwork.h"
#include "EscapeFromHuman.h"
#include "Components/SphereComponent.h"
#include "Components/ChildActorComponent.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"
#include "KJW/Game/EFHPlayerState.h"
#include "KJW/Game/EFHGameState.h"
#include "NiagaraFunctionLibrary.h"





// Sets default values
AScoreActor::AScoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement ( true );

	SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	SetRootComponent ( SphereComp );

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	MeshComp->SetupAttachment ( GetRootComponent () );

	lightComp = CreateDefaultSubobject<UChildActorComponent> ( TEXT ( "LightComp" ) );
	lightComp->SetupAttachment ( GetRootComponent () );

	RingComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "RingComp" ) );
	RingComp->SetupAttachment ( SphereComp );
	RingComp->SetVisibility ( false );


	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
}

void AScoreActor::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );


	DOREPLIFETIME ( AScoreActor , ScoreState );
}

// Called when the game starts or when spawned
void AScoreActor::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnEffect ();
	if (HasAuthority ())
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic ( this , &AScoreActor::OnOverlapBeginComponent );
		SetRandomMesh();

		AEFHGameState* GS = GetWorld ()->GetGameState<AEFHGameState> ();
		if (GS)
		{
			GS->OnReadyStateEvent.AddUObject ( this ,  &ThisClass::OnReadyDestroy );
		}
	}
	else
	{
		//SphereComp->SetVisibility ( false );
		SphereComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	}

	OrginLocation = GetActorLocation();
}

// Called every frame
void AScoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#pragma region 
	//if (ScoreState == EScoreState::NONE || ScoreState == EScoreState::HOME)
//{
//	ElapsedTime += DeltaTime;
//	ElapsedTime = FMath::Fmod ( ElapsedTime , 2 * PI );

//	// 진동 범위와 속도 설정
//	const float Amplitude = 30.0f;      // 최대 위아래 거리
//	const float Frequency = 0.5f;       // 초당 왕복 횟수 (1이면 1초에 위-아래 한 번)

//	// 새로운 위치 계산
//	FVector NewLocation = GetActorLocation ();
//	NewLocation.Z = 70.0f;
//	NewLocation.Z += FMath::Sin ( ElapsedTime * Frequency * 2 * PI ) * Amplitude;

//	// 위치 적용
//	//SetActorLocation ( NewLocation );
//	MeshComp->SetWorldLocation ( NewLocation );
//}
#pragma endregion



	if (HasAuthority ())
	{
		
		AddActorWorldRotation ( FRotator ( 0.0f , 50.0f * DeltaTime , 0.0f ) );
		//if (!HasAuthority ()) return;
		if (ScoreState == EScoreState::TRAKING)
		{
			Move ( DeltaTime );

		}
	}

}



void AScoreActor::InHome (FVector Location)
{
	if (ScoreState == EScoreState::TOP)
	{
		ScoreDrop ();
	}

	bHome = true;
	ScoreState = EScoreState::HOME;
	HomePos = Location;
	HomePos.Z = 70.0f;
	
	if (HomeAnimal == nullptr)
	{
		HomeAnimal = OwnerAnimal;
	}

	SetActorLocation ( HomePos );

	MulticastRPC_SetScale ( 1.0f );
	OnInvisible ( false );

	if (HasAuthority ())
	{
		OnRep_ScoreState ();
		//ServerRPC_SetLocation ( Location );
	}
}

void AScoreActor::OutHome ()
{
	HomeAnimal = nullptr;
	bHome = false;


}

void AScoreActor::OnInvisible ( bool bShow )
{
	MulticastRPC_ShowMesh ( bShow );
}

void AScoreActor::OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if (ScoreState == EScoreState::TRAKING || ScoreState == EScoreState::TOP) return;

	AAnimalBase* animal = Cast<AAnimalBase> ( OtherActor );
	if (!animal) return;

	if (ScoreState == EScoreState::NONE)
	{
		if (OwnerAnimal == animal) return;

		
		PS = animal->GetPlayerState<AEFHPlayerState> ();

		ScoreState = EScoreState::TOP;
		SetOwnerAnimal ( animal );
		SpawnGetEffect ();
	
	

		//ScoreState = EScoreState::TRAKING;
		
		//Destroy ();
		//if (OnReturnObj.IsBound ()) OnReturnObj.Execute ( this );	
	}
	else if (ScoreState == EScoreState::HOME  )
	{
		if (HomeAnimal == animal) return;

		PS = animal->GetPlayerState<AEFHPlayerState> ();
		ScoreState = EScoreState::TOP; 
		SetOwnerAnimal ( animal );
	
	}
	

	OnRep_ScoreState ();

}

void AScoreActor::SetRandomMesh()
{
	if (MeshList.Num() <= 0) return;

	int32 rand = FMath::RandRange(0, MeshList.Num() - 1);

	MulticastRPC_SetMesh(rand);

}



void AScoreActor::SetStateNone ()
{
	if (GetOwner ())
	{
		SetOwnerAnimal ( nullptr );
		

		if (HasAuthority ())
			OnRep_ScoreState ();
	}

}

void AScoreActor::OnRep_ScoreState ()
{
	if (ScoreState == EScoreState::HOME)
	{
		lightComp->SetVisibility ( false );
	}
	else if (ScoreState == EScoreState::TRAKING)
	{
		if (GetWorld ()->GetFirstPlayerController ()->GetPawn () == GetOwner())
		{
		lightComp->SetVisibility ( false );
		}
		else
		lightComp->SetVisibility ( true );

	}
	else if (ScoreState == EScoreState::NONE)
	{
		lightComp->SetVisibility ( true );
	}
	else if (ScoreState == EScoreState::TOP)
	{
		lightComp->SetVisibility ( false );
	}
}

void AScoreActor::SetOwnerAnimal ( AAnimalBase* Animal )
{
	if (OwnerAnimal)
	{
		OwnerAnimal->OnHitEvent.RemoveAll ( this );		
		OwnerAnimal->OnInvisible.RemoveAll ( this );
	}

	if (Animal)
	{
		//Top
		if (ScoreState == EScoreState::TOP)
		{
			AttachToComponent ( Animal->Scenecomp , FAttachmentTransformRules::KeepWorldTransform );

			int32 a = Animal->Scenecomp->GetNumChildrenComponents ();
			if(a == 1)
				SetActorRelativeLocation ( FVector ( 0.0f , 0.0f , 0.0f ) );
			else
				SetActorRelativeLocation ( FVector ( 0.0f , 0.0f , (a-1) * 40.f ) );

			MulticastRPC_SetScale (0.5f );
			
		}

		OwnerAnimal = Animal;
		SetOwner ( Animal );
		OwnerAnimal->OnHitEvent.AddUObject ( this , &ThisClass::SetStateNone );
		OwnerAnimal->OnInvisible.AddUObject ( this , &ThisClass::OnInvisible );
		AEFHPlayerState* newPS = Animal->GetPlayerState<AEFHPlayerState> ();

		if (OwnerAnimal->StatusComp->IsInvisible)
		{
			OnInvisible (true);
		}
		
		//ServerRPC_SetRing ( newPS ->GameTeam == EGameTeam::A ? 0 : 1 );
		
	}
	else
	{
		if (bHome)
		{
			InHome ( HomePos );
		}
		else
		{
			if (ScoreState == EScoreState::TOP)
			{
				ScoreDrop ();
			}

			PS = nullptr;
			SetOwner ( nullptr );
			OwnerAnimal = nullptr;
			ScoreState = EScoreState::NONE;
		}

		//ServerRPC_SetRing ( 2 );
		//SphereComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
		//SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	}

	

}

void AScoreActor::Move ( float DeltaTime )
{
	if (GetOwner ())
	{
		FVector myLocation = GetActorLocation ();
		FVector targetLocation = GetOwner ()->GetActorLocation ();

		float distance = FVector::Dist ( myLocation , targetLocation );

		if (distance > StopDistance)
		{
			FollowSpeed += (DeltaTime * FollowAddSpeed);
		}
		else
		{
			FollowSpeed  = 0.0f;
		}

		FollowSpeed = FMath::Clamp ( FollowSpeed , 0.0f , FollowMaxSpeed );

		//FVector newLocation = FMath::VInterpTo ( myLocation , targetLocation , DeltaTime , FollowSpeed );
		FVector newLocation = myLocation  + ((targetLocation - myLocation).GetSafeNormal () * DeltaTime * FollowSpeed);
		
		SetActorLocation ( newLocation );
	}
}

void AScoreActor::ScoreDrop ()
{
	FVector pos;
	pos.X = GetActorLocation ().X;
	pos.Y = GetActorLocation ().Y;
	pos.Z = 70.0f;
	if (OwnerAnimal)
	{
		FVector2D rand = FMath::RandPointInCircle ( 1.0f );
		float randDis = FMath::RandRange( 30.0f , 100.0f );
		pos.X += (rand.X * randDis);
		pos.Y += (rand.Y * randDis);
	}

	OnInvisible ( false );
	DetachFromActor ( FDetachmentTransformRules::KeepWorldTransform );
	MulticastRPC_SetScale (  1.0f  );
	SetActorLocation ( pos );
	
}

void AScoreActor::SpawnEffect ()
{
	if (NS_SpawnEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation (
			GetWorld () ,
			NS_SpawnEffect ,
			MeshComp->GetComponentLocation ()
		);
	}
}

void AScoreActor::SpawnGetEffect ()
{
	MulticastRPC_SpawnGetEffect ();
}

void AScoreActor::MulticastRPC_SetScale_Implementation ( float Scale )
{
	SetActorScale3D ( FVector ( Scale ) );
}


void AScoreActor::MulticastRPC_SetMesh_Implementation(int32 MeshIndex)
{
	UStaticMesh* newMesh = MeshList[MeshIndex];

	MeshComp->SetStaticMesh(newMesh);
}

void AScoreActor::ServerRPC_SetLocation_Implementation ( FVector Location )
{
	MulticastRPC_SetLocation ( Location );
}



void AScoreActor::MulticastRPC_SetLocation_Implementation ( FVector Location )
{
	SetActorLocation ( Location );
}

void AScoreActor::ServerRPC_SetRing_Implementation ( int32 TeamIndex )
{
	//MulticastRPC_SetRing ( TeamIndex );
}

void AScoreActor::MulticastRPC_SetRing_Implementation ( int32 TeamIndex )
{
	if(TeamIndex == 2)
	{ 
		RingComp->SetVisibility ( false );
	}
	else
	{
		RingComp->SetVisibility ( true );

		FLinearColor ringColor = FColor::White;
		if (TeamIndex == 0)
		{
			ringColor = FColor::Purple;
		}
		else if (TeamIndex == 1)
		{
			ringColor = FColor::Red;
		}


		int32 materialCount = RingComp->GetNumMaterials ();
		for (int32 i = 0; i < materialCount; ++i)
		{
			if (!RingDynMaterial)
			{
				// 기존 머티리얼을 가져옴
				UMaterialInterface* Material = RingComp->GetMaterial ( i );
				
				// 새 동적 인스턴스를 생성
				RingDynMaterial = UMaterialInstanceDynamic::Create ( Material , this );
				// 해당 슬롯에 새 인스턴스 할당
				RingComp->SetMaterial ( i , RingDynMaterial );
			}
			

			RingDynMaterial->SetVectorParameterValue ( FName ( "Color" ) , ringColor );
		}
	}


}

void AScoreActor::MulticastRPC_ShowMesh_Implementation ( bool bShow )
{
	if (bShow)
	{
		APawn* pawn = Cast<APawn> ( GetOwner () );
		if (pawn && pawn->IsLocallyControlled ())
		{
			MeshComp->SetMaterial ( 0 , MT_Invisible );
		}
		else
		{
			MeshComp->SetVisibility ( false );
		}
	}
	else
	{
		MeshComp->SetVisibility ( true );
		MeshComp->SetMaterial ( 0 , MT_UnInvisible );
	}

}

void AScoreActor::MulticastRPC_SpawnGetEffect_Implementation ()
{
	if (NS_GetScoreEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation (
			GetWorld () ,
			NS_GetScoreEffect ,
			MeshComp->GetComponentLocation ()
		);
	}

}

void AScoreActor::OnReadyDestroy ()
{
	Destroy ();
}
