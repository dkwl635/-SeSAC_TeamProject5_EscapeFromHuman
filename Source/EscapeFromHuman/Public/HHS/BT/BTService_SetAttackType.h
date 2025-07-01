// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SetAttackType.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UBTService_SetAttackType : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_SetAttackType();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTypeKey;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float TranquilizerRange = 300.0f;
	
};
