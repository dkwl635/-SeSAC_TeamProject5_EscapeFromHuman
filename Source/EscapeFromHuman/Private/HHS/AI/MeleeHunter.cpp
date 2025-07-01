// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/AI/MeleeHunter.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMeleeHunter::AMeleeHunter()	
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/HHS/AI/ABP_HunterC.ABP_HunterC'"));
	if (MeleeAnim.Succeeded())
	{
		MeleeMontage = MeleeAnim.Object;
	}
	
	AttackRange = 80.f;
	DefaultAttackType = EAttackType::Melee;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshTemp(TEXT("/Script/Engine.StaticMesh'/Game/3D_LOW_POLY_FarmPack/Meshes/Props/SM_Fork.SM_Fork'"));
	if (MeshTemp.Succeeded())
	{
		WeaponMesh->SetStaticMesh(MeshTemp.Object);
	}
	WeaponMesh->SetRelativeLocation(FVector(-7.425181, -0.466370, -28.457949));
	WeaponMesh->SetRelativeScale3D(FVector(1.f));

	CollisionComp=CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	CollisionComp->SetRelativeScale3D(FVector(0.75f));
	CollisionComp->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionComp->SetGenerateOverlapEvents(false);

	CollisionComp2 = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp2"));
	CollisionComp2->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	CollisionComp->SetRelativeScale3D(FVector(0.75f));
	CollisionComp2->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	CollisionComp2->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionComp2->SetGenerateOverlapEvents(false);

	HitNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitNiagaraComp"));
	HitNiagaraComp->SetupAttachment(RootComponent);
	HitNiagaraComp->SetAutoActivate(false);
}

void AMeleeHunter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CollisionComp)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMeleeHunter::OnAttackOverlap);		
	}
	if (CollisionComp2)
	{
		CollisionComp2->OnComponentBeginOverlap.AddDynamic(this, &AMeleeHunter::OnAttackOverlap);
	}
}

void AMeleeHunter::AttackStart()
{
	Super::AttackStart();
	
	MeleeAttack();
}

void AMeleeHunter::AttackEnd()
{
	Super::AttackEnd();
	
	EndMelee();
}

void AMeleeHunter::MeleeAttack()
{
	if (CollisionComp)
	{
		CollisionComp->SetCollisionProfileName("Melee");
		CollisionComp->SetGenerateOverlapEvents(true);
		bDidHitPlayer = false;

		TArray<AActor*> OverlappingActors;
		CollisionComp->GetOverlappingActors(OverlappingActors);
	}
	if (CollisionComp2)
	{
		CollisionComp2->SetCollisionProfileName("Melee");
		CollisionComp2->SetGenerateOverlapEvents(true);
		bDidHitPlayer = false;

		TArray<AActor*> OverlappingActors;
		CollisionComp2->GetOverlappingActors(OverlappingActors);
	}
	
}

void AMeleeHunter::EndMelee()
{
	if (CollisionComp)	
	{
		CollisionComp->SetCollisionProfileName("NoCollision");
		CollisionComp->SetGenerateOverlapEvents(false);
	}
	if (CollisionComp)
	{
		CollisionComp->SetCollisionProfileName("NoCollision");
		CollisionComp->SetGenerateOverlapEvents(false);
	}	
	if (CollisionComp2)
	{
		CollisionComp2->SetCollisionProfileName("NoCollision");
		CollisionComp2->SetGenerateOverlapEvents(false);
	}
}

float AMeleeHunter::GetAttackRange(EAttackType AttackType) const
{
	return Super::GetAttackRange(AttackType);
}

void AMeleeHunter::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bDidHitPlayer) return;

	AAnimalBase* Player = Cast<AAnimalBase>(OtherActor);
	if (Player && Player->StatusComp && !Player->StatusComp->IsIce)
	{
		bDidHitPlayer = true;
		LastHitLocation = Player->GetActorLocation();
		
		if (Player->StatusComp->IsIce)
		{
			Player->StatusComp->IsIce = false;
			Player->GetCharacterMovement()->bOrientRotationToMovement = true;
			Player->meshcomp->SetVisibility ( false );
		}
		else
		{
			Player->Die();
		}
		
		CollisionComp->SetGenerateOverlapEvents(false);
		CollisionComp2->SetGenerateOverlapEvents(false);
		
		Multicast_HitEffect(LastHitLocation);
		Multicast_PlayHitSound(LastHitLocation);

		//Multicast_PlayHitSound(Player->GetActorLocation());
	}
}

UAnimMontage* AMeleeHunter::GetMontage(EAttackType AttackType) const
{
	if (AttackType == EAttackType::Melee)
	{	
		return MeleeMontage;
	}
	return nullptr;
}

void AMeleeHunter::Multicast_HitEffect_Implementation(FVector Location)
{
	if (HitNiagaraComp)
	{
		HitNiagaraComp->SetWorldLocation(Location);
		HitNiagaraComp->Activate(true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]()
			{
				if (HitNiagaraComp)
				{
					HitNiagaraComp->Deactivate();
				}
			}, 0.5f,false);
	}
}

void AMeleeHunter::Multicast_PlayHitSound_Implementation(FVector Location)
{
	if (bDidHitPlayer && PlayHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PlayHitSound, Location);
	}
}
