// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit ABaseAIController(FObjectInitializer const& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite)
	class ABaseAIHunter* TeamHunter;

	virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }

	UFUNCTION(BlueprintCallable, Category="AI")
	AActor* GetCurrentTarget() const;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	FGenericTeamId TeamId;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

private:
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	

	UPROPERTY(Replicated)
	class AAnimalBase* DetectedAnimal;

	UPROPERTY(Replicated)
	class ABaseAIHunter* Hunter;

	UPROPERTY(Replicated)
	class UBlackboardComponent* BB;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* bIsFrozenSound;
};
