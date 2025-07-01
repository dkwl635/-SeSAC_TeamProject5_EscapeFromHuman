// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsTargetFrozen.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UBTDecorator_IsTargetFrozen : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsTargetFrozen();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetActorKey;
};
