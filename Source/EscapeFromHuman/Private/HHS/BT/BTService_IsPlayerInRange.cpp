// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTService_IsPlayerInRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/BaseAIController.h"
#include "HHS/BaseAIHunter.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsPlayerInRange::UBTService_IsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Attack Range");
}

void UBTService_IsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI 컨트롤러 및 헌터 캐릭터 가져오기
	auto const* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	auto const* const Hunter = Cast<ABaseAIHunter>(Controller ? Controller->GetPawn() : nullptr);

	if (!Hunter) return;

	// Blackboard에서 공격 타입 가져오기
	EAttackType AttackType = Hunter->DefaultAttackType;
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		uint8 AttackTypeValue = Blackboard->GetValueAsEnum(AttackTypeKey.SelectedKeyName);
		AttackType = static_cast<EAttackType>(AttackTypeValue);
	}

	// 공격 타입에 따라 사거리 설정
	float AttackRange = Hunter->GetAttackRange(AttackType);
	
	// 현재 월드에서 모든 캐릭터 가져오기
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), PlayerActors);

	AActor* ClosestPlayer = nullptr;
	float MinDistance = FLT_MAX;

	for (AActor* Player : PlayerActors)
	{
		// AI 자신은 제외
		if (Player == Hunter) continue;

		float Distance = FVector::Dist(Hunter->GetActorLocation(), Player->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPlayer = Player;
		}
	}

	// 가장 가까운 플레이어가 존재하면 거리 비교 후 Blackboard 값 설정
	bool bInRange = (ClosestPlayer && MinDistance <= AttackRange);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bInRange);
	
}
