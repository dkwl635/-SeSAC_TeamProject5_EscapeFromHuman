// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/AI/GunHunter.h"

#include "NiagaraComponent.h"
#include "HHS/Actor/Bullet.h"
#include "Kismet/GameplayStatics.h"


AGunHunter::AGunHunter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/HHS/AI/ABP_HunterA.ABP_HunterA'"));
	if (MeleeAnim.Succeeded())
	{
		MeleeMontage = MeleeAnim.Object;
	}

	AttackRange = 400.f;
	DefaultAttackType = EAttackType::Gun;
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TmpGun(TEXT("/Script/Engine.SkeletalMesh'/Game/HHS/Assets/Rifle/source/Rifle.Rifle'"));
	if (TmpGun.Succeeded())
	{
		GunMesh->SetSkeletalMesh(TmpGun.Object);
	}
	GunMesh->SetRelativeLocation(FVector(-20.625538, 9.726541, -9.419830));
	GunMesh->SetRelativeRotation(FRotator(3.179104, 168.971325, 0.118677));
	GunMesh->SetRelativeScale3D(FVector(0.3, 0.3, 0.3));

	FireNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleNiagaraComp"));
	FireNiagaraComp->SetupAttachment(GunMesh, TEXT("Muzzle"));
	FireNiagaraComp->SetAutoActivate(false);

	CurrentAimRotation = GetActorForwardVector().Rotation();
}

void AGunHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector MuzzleLocation = GunMesh->GetSocketLocation(TEXT("Muzzle"));

	AActor* CurrentTarget = this->GetTargetActor();

	if (CurrentTarget)
	{
		if (AAnimalBase* AnimalTarget = Cast<AAnimalBase>(CurrentTarget))
		{
			USkeletalMeshComponent* TargetMesh = AnimalTarget->GetMesh();
			if (TargetMesh)
			{
				LastAimLocation = TargetMesh->GetSocketLocation(FName("Chest"));
			}
		}
	}

	FVector TargetAimDir;
	if (!LastAimLocation.IsZero())
	{
		TargetAimDir = (LastAimLocation - MuzzleLocation).GetSafeNormal();
	}
	else
	{
		TargetAimDir = GetActorForwardVector();
	}

	FRotator TargetRotation = TargetAimDir.Rotation();

	const float InterpSpeed = 10.f;
	CurrentAimRotation = FMath::RInterpTo(CurrentAimRotation, TargetRotation, DeltaTime, InterpSpeed);
}

void AGunHunter::AttackStart()
{
	Super::AttackStart();

	GunAttack();
}

void AGunHunter::AttackEnd()
{
	Super::AttackEnd();

	EndGun();
}

void AGunHunter::GunAttack()
{
	if (!HasAuthority() || !BulletClass) return;

	FVector MuzzleLocation = GunMesh->GetSocketLocation(TEXT("Muzzle"));

	// 보간된 회전값 사용
	FRotator MuzzleRotation = CurrentAimRotation;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);

	Multicast_FireEffect();

}

void AGunHunter::EndGun()
{
}

float AGunHunter::GetAttackRange(EAttackType AttackType) const
{
	return Super::GetAttackRange(AttackType);
}

UAnimMontage* AGunHunter::GetMontage(EAttackType AttackType) const
{
	if (AttackType == EAttackType::Gun)
	{
		return GunMontage;
	}
	return nullptr;
}

void AGunHunter::Multicast_FireEffect_Implementation()
{
	if (FireNiagaraComp && !FireNiagaraComp->IsActive())
	{
		FireNiagaraComp->Activate(true);
		FTimerHandle TempHandle;
		GetWorldTimerManager().SetTimer(TempHandle, [this]()
		{
			if (FireNiagaraComp)
			{
				FireNiagaraComp->Deactivate();
			}
		}, 0.5f, false);
	}
	if (PlayFireSound)
	{
		FVector SoundLocation = GunMesh->GetSocketLocation(TEXT("Muzzle"));
		UGameplayStatics::PlaySoundAtLocation(this, PlayFireSound, SoundLocation);
	}
}