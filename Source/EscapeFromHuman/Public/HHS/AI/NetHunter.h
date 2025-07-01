// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHS/BaseAIHunter.h"
#include "NetHunter.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API ANetHunter : public ABaseAIHunter
{
	GENERATED_BODY()

public:
	ANetHunter();

	virtual void AttackStart() override;
	virtual void AttackEnd() override;

	virtual float GetAttackRange(EAttackType AttackType) const override;
	virtual UAnimMontage* GetMontage(EAttackType AttackType) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float NetAttackRange = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ALassoActor> LassoClass;

	UPROPERTY()
	ALassoActor* LassoActor;

	UPROPERTY()
	ALassoActor* CurrentLasso;

	UPROPERTY()
	bool bIsAttacking = false;

	// 루프 종료 요청 함수
	void EndPull();
	
	UFUNCTION(Server, Reliable)
	void Server_EndPull();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EndPull();
	
	UFUNCTION()
	void StartLassoSpin();

	UFUNCTION()
	void ThrowNet();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Lasso")
	bool bShouldEndPull = false;

	UPROPERTY()
	bool bAlreadyJumpedToEnd = false;

protected:
	// 루프 종료 체크용 Tick 오버라이드
	virtual void Tick(float DeltaTime) override;
	
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};