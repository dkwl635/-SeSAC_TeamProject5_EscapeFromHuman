// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTTask_IncrementPathIndex.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/BaseAIController.h"
#include "HHS/BaseAIHunter.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI 컨트롤러 가져오기
	if (auto* const Cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner()))
	{
		// Hunter 가져오기
		if (auto* const Hunter = Cast<ABaseAIHunter>(Cont->GetPawn()))
		{
			// blackboard 가져오기
			if (auto* const BC = OwnerComp.GetBlackboardComponent())
			{
				// 순찰 지점수와 최소, 최대 가져오기
				auto const NoOfPoints = Hunter->GetPatrolPath()->Num();
				auto const MinIndex = 0; 
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = BC->GetValueAsInt(GetSelectedBlackboardKey());			// 여러 지점을 최대 인덱스로 가져오고 블랙보드에서 순찰 경로의 현재 인덱스 가져옴

				// 양방향에 있는 경우 -> 첫 번째 또는 마지막 인덱스일 때 방향 변경
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// blackboard에 새로운 index 값 쓰기
				BC->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? ++Index : -- Index) % NoOfPoints);

				// 성공
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	// 실패
	return EBTNodeResult::Failed;
}
