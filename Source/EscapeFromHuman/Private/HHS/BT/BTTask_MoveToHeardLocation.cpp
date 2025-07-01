// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTTask_MoveToHeardLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToHeardLocation::UBTTask_MoveToHeardLocation(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Move To Heard Location");
}

EBTNodeResult::Type UBTTask_MoveToHeardLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* const BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr) return EBTNodeResult::Failed;

	const FVector HeardLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());

	if (SearchRandom)
	{
		FNavLocation Loc;

		if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			if (NavSys->GetRandomPointInNavigableRadius(HeardLocation, SearchRadius, Loc))
			{
				BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(),Loc.Location);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	else
	{
		BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(),HeardLocation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
