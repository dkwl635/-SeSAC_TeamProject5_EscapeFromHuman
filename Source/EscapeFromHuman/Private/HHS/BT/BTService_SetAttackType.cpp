// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTService_SetAttackType.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/BaseAIHunter.h"

UBTService_SetAttackType::UBTService_SetAttackType()
{
	NodeName = TEXT("Set Attack Type");
	Interval = 0.5f;

}

void UBTService_SetAttackType::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* const Blackboard = OwnerComp.GetBlackboardComponent();
	auto* const Controller = OwnerComp.GetAIOwner();
	auto* const Hunter = Cast<ABaseAIHunter>(Controller ? Controller->GetPawn() : nullptr);

	if (!Blackboard || !Hunter) return;

	// 무조건 에너미가 설정한 FixedAttackType을 블랙보드에 설정
	Blackboard->SetValueAsEnum(AttackTypeKey.SelectedKeyName, static_cast<uint8>(Hunter->FixedAttackType));
}