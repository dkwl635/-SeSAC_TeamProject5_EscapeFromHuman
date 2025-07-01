// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTDecorator_IsTargetFrozen.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"

UBTDecorator_IsTargetFrozen::UBTDecorator_IsTargetFrozen()
{
	NodeName = "Target Is Not Frozen";
}

bool UBTDecorator_IsTargetFrozen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return false;

	UObject* TargetObject = BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AAnimalBase* TargetAnimal = Cast<AAnimalBase>(TargetObject);
	if (!TargetAnimal || !TargetAnimal->StatusComp) return false;
	
	const bool bIsFrozen = TargetAnimal->StatusComp->IsIce;
	const bool bIsInvisible = TargetAnimal->StatusComp->IsInvisible;

	return !bIsFrozen && !bIsInvisible;
}
