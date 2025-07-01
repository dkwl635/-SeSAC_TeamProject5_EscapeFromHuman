// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTTask_FindPathPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/BaseAIController.h"
#include "HHS/BaseAIHunter.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 컨트롤러 가져오기
	if (auto* const cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner()))
	{
		// BT에서 BB 구성요소 가져오기
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			auto const Index = bc->GetValueAsInt(GetSelectedBlackboardKey());

			// hunter 가져오기
			if (auto* Hunter = Cast<ABaseAIHunter>(cont->GetPawn()))
			{
				if (auto* PatrolPath = Hunter->GetPatrolPath())
				{
					// hunter 현a재 순찰 경로 가져오기
					auto const Point = Hunter->GetPatrolPath()->GetPatrolPoint(Index);

					// 로컬 벡터 -> 전역 벡터
					auto const GlobalPoint = Hunter->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
					bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

					// 성공
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
