// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHS/BaseAIHunter.h"
#include "GunHunter.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API AGunHunter : public ABaseAIHunter
{
	GENERATED_BODY()
protected:
	virtual void Tick(float DeltaTime) override;

private:
	FVector CurrentAimDirection; 
public:
	AGunHunter();

	void GunAttack();
	void EndGun();
	
	virtual void AttackStart() override;
	virtual void AttackEnd() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float GunAttackRange = 400.f;

	virtual float GetAttackRange(EAttackType AttackType) const override;
	virtual UAnimMontage* GetMontage(EAttackType AttackType) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float AimingDuration = 1.5f; // 조준 시간

	void PerformGunFire();
	FTimerHandle GunFireTimerHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* FireNiagaraComp;

	FTimerHandle MuzzleVFXTimerHandle;

	FVector LastAimLocation = FVector::ZeroVector;

	FRotator CurrentAimRotation;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireEffect();

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* PlayFireSound;

};
