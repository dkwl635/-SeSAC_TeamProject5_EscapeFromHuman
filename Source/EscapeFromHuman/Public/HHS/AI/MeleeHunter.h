// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHS/BaseAIHunter.h"
#include "MeleeHunter.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API AMeleeHunter : public ABaseAIHunter
{
	GENERATED_BODY()

public:
	AMeleeHunter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* HitNiagaraComp;

protected:
	virtual void BeginPlay() override;
	void MeleeAttack();
	void EndMelee();

public:
	virtual void AttackStart() override;
	virtual void AttackEnd() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MeleeAttackRange = 80.f;
	
	virtual float GetAttackRange(EAttackType AttackType) const override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	//UCapsuleComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Collision, meta=(AllowPrivateAccess="true"))
	class USphereComponent* CollisionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	class USphereComponent* CollisionComp2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool bDidHitPlayer = false;
	
	virtual UAnimMontage* GetMontage(EAttackType AttackType) const override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_HitEffect(FVector Location);

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* PlayHitSound;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitSound(FVector Location);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hit")
	FVector LastHitLocation;
};
