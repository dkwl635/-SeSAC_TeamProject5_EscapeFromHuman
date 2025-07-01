// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Actor/CarActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/AnimalBase.h"
#include "Net/UnrealNetwork.h"
#include "LJW/StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACarActor::ACarActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(300.0f, 150.0f, 130.0f));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACarActor::HitPlayer);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBody"));
	CarMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CarMesh->SetupAttachment(RootComponent);
	CarMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CarMesh->SetRelativeLocation(FVector(0.0f, 8.0f, -55.0f));

}

// Called when the game starts or when spawned
void ACarActor::BeginPlay()
{
	Super::BeginPlay();

	currentTime = 0.0f;
	startPos = GetActorLocation();
	SetReplicateMovement(true);
	SetReplicates(true);	
	
	//if (HasAuthority())
	//{
	//	Multicast_PlayDriveSound();
	//}
}

// Called every frame
void ACarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
	{
		currentTime += DeltaTime;

		if (currentTime < MovingTime)
		{
			SetActorLocation(startPos + GetActorForwardVector() * MovingSpeed * currentTime);
		}
		else
		{
			Destroy();
		}
	}
}

void ACarActor::HitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AAnimalBase* HitPlayer = Cast<AAnimalBase>(OtherActor);
		if (HitPlayer)
		{
			if (HitPlayer->StatusComp->IsIce)
			{
				HitPlayer->StatusComp->IsIce = false;
				HitPlayer->GetCharacterMovement()->bOrientRotationToMovement = true;
				HitPlayer->meshcomp->SetVisibility ( false );
			}
			else
			{
				HitPlayer->Die();
			}
		}
	}
}



//void ACarActor::Multicast_PlayDriveSound_Implementation()
//{
//	if (DrivingSound)
//	{
//		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DrivingSound, GetActorLocation());
//	}
//}

void ACarActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACarActor, currentTime);
}


