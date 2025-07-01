// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_TargetHitByGun.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UBTDecorator_TargetHitByGun : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_TargetHitByGun();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
