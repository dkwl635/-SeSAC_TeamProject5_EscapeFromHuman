// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Actor/Bullet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"

// Sets default values
ABullet::ABullet()
{
	bReplicates = true;
	SetReplicateMovement(true);  

	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.f);
	CollisionComp->SetCollisionProfileName(NAME_None);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	CollisionComp->SetGenerateOverlapEvents(true);

	RootComponent = CollisionComp;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bSweepCollision = true;
	
	SetLifeSpan(5.f);


	HitNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitNiagaraComp"));
	HitNiagaraComp->SetupAttachment(RootComponent);
	HitNiagaraComp->SetAutoActivate(false); 

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComp)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this,&ABullet::OnAttackOverlap);
	}
}

void ABullet::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (bDidHitPlayer) return;

	AAnimalBase* Player = Cast<AAnimalBase>(OtherActor);
	if (Player)
	{
		if (Player->StatusComp && Player->StatusComp->IsIce)
		{
			//UE_LOG(LogTemp, Warning, TEXT("❄플레이어 [%s]가 얼음 상태 총알 무시"), *Player->GetName());
			return; 
		}

		//UE_LOG(LogTemp, Warning, TEXT("총알이 [%s] 에게 적중"), *Player->GetName());
		
		bDidHitPlayer = true;
		Player->HitGun();

		Multicast_HitEffect(Player->GetActorLocation());
		Multicast_PlayHitSound(Player->GetActorLocation());
		
		SetLifeSpan(0.2f);
	}
}

void ABullet::Multicast_PlayHitSound_Implementation(FVector Location)
{
	if (PlayHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PlayHitSound, Location);
	}
}

void ABullet::Multicast_HitEffect_Implementation(FVector Location)
{
	if (HitNiagaraComp)
	{
		HitNiagaraComp->SetWorldLocation(Location);
		HitNiagaraComp->Activate(true);
	}
}
