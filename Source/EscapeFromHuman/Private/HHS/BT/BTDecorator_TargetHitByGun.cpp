// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTDecorator_TargetHitByGun.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"

UBTDecorator_TargetHitByGun::UBTDecorator_TargetHitByGun()
{
	NodeName = "Target Was Hit By Gun";
}

bool UBTDecorator_TargetHitByGun::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return false;

	UObject* TargetObject = BlackboardComponent->GetValueAsObject(GetSelectedBlackboardKey());
	AAnimalBase* TargetAnimal = Cast<AAnimalBase>(TargetObject);
	if (!TargetAnimal) return false;

	return TargetAnimal->StatusComp->bHitByGun;
}
