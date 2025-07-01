// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "EAttackType.h"
#include "HHS/Actor/PatrolPath.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "LJW/AnimalBase.h"
#include "UI/AI_UserWidget.h"
#include "BaseAIHunter.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API ABaseAIHunter : public ACharacter, public ICombatInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABaseAIHunter();
	
	// TeamId
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 ID = 1;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

protected:
	virtual void BeginPlay() override;
	FGenericTeamId TeamId;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AttackStart();
	virtual void AttackEnd();

	virtual float GetAttackRange(EAttackType AttackType) const;

	UBehaviorTree* GetBehaviorTree() const;
	APatrolPath* GetPatrolPath() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual bool Attack_Implementation(EAttackType AttackType) override;

	bool IsAttackOnCooldown(EAttackType Type) const;
	void StartCooldown(EAttackType Type, float Cooldown);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMontage(UAnimMontage* AnimMontage);

	UPROPERTY(ReplicatedUsing = OnRep_CanSeePlayer)
	bool bCanSeePlayer;

	UPROPERTY(ReplicatedUsing = OnRep_HeardNoise)
	bool bHeardNoise;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector HeardLocation;

	UFUNCTION()
	void OnRep_CanSeePlayer();

	UFUNCTION()
	void OnRep_HeardNoise();
	
	void DisableMovement();
	void EnableMovement();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAttackType DefaultAttackType;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	EAttackType CurrentAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAttackType FixedAttackType = EAttackType::None;

	virtual UAnimMontage* GetMontage(EAttackType AttackType) const;

	// ========================== UI ==========================

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetComponent* AlertWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UAI_UserWidget> AlertWidgetClass;
	
	FTimerHandle AlertHideTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FVector WidgetOffset = FVector(0, 0, 150.0f); // 캐릭터 머리 위
	
	void HideAlertIcon();
	
	void SetAlertType(EAlertType NewType);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetAlertType(EAlertType NewType);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetAlertType(EAlertType NewType);

	UPROPERTY(EditAnywhere, Category="Sound")
	TArray<USoundBase*> QuestionSound;

	UPROPERTY()
	UAudioComponent* CurrentAudio;

	// ========================== UI =========================
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* MeleeMontage;
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* GunMontage;
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* NetMontage;
	
	bool bDidHitPlayer = false;

	TMap<EAttackType, float> AttackCooldowns;
	
	// 추적 중인 타겟 액터 저장
		UPROPERTY()
		AAnimalBase* TargetActor;
		// 타겟 반환 함수
		AActor* GetTargetActor() const;

};
