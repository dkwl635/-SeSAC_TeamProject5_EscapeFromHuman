// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Engine/TriggerVolume.h"
#include "HHS/BaseAIHunter.h"
#include "LJW/AnimalBase.h"


UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Find Player Location");
}


EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnimalBase::StaticClass(), PlayerActors);

	
	if (PlayerActors.Num() == 0) return EBTNodeResult::Failed;

	AActor* ClosestPlayer = nullptr;
	float MinDistance = FLT_MAX;

	AActor* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector MyLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	// 기지 위치를 나타내는 액터들 가져오기
	TArray<AActor*> BaseZones;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("BaseZone"), BaseZones);

	for (AActor* Player : PlayerActors)
	{
		// 자신(AI)이 아닌 다른 캐릭터일 경우
		if (Player == OwnerComp.GetAIOwner()->GetPawn()) continue;

		// TeamId 비교
		IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Player);
		if (TeamAgent)
		{
			FGenericTeamId MyTeamId = OwnerComp.GetAIOwner()->GetGenericTeamId();
			FGenericTeamId OtherTeamId = TeamAgent->GetGenericTeamId();

			if (MyTeamId == OtherTeamId) continue; // 같은 팀이면 무시
		}

		// 기지 안에 있으면 무시
		bool bIsInBase = false;
		for (AActor* Base : BaseZones)
		{
			if (Base && Base->IsA(ATriggerVolume::StaticClass()))
			{
				ATriggerVolume* Trigger = Cast<ATriggerVolume>(Base);
				if (Trigger && Trigger->IsOverlappingActor(Player))
				{
					bIsInBase = true;
					break;
				}
			}
		}

		if (bIsInBase)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player %s is inside base zone. Ignored."), *Player->GetName());
			continue;
		}

		float Distance = FVector::Dist(MyLocation, Player->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPlayer = Player;
		}
	}

	if (!ClosestPlayer) 
	{
		// Blackboard에서 타겟 제거
		OwnerComp.GetBlackboardComponent()->ClearValue("TargetActor");

		// 패트롤 복귀를 위해 PatrolIndex 증가
		ABaseAIHunter* Hunter = Cast<ABaseAIHunter>(MyPawn);
		if (Hunter && Hunter->GetPatrolPath())
		{
			int32 Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt("PatrolIndex");
			int32 NumPoints = Hunter->GetPatrolPath()->Num();

			if (NumPoints > 0)
			{
				Index = (Index + 1) % NumPoints;
				OwnerComp.GetBlackboardComponent()->SetValueAsInt("PatrolIndex", Index);
			}
		}

		return EBTNodeResult::Failed; // 순찰 노드로 넘어감
	}

	FVector TargetLocation = ClosestPlayer->GetActorLocation();

	ABaseAIHunter* Hunter = Cast<ABaseAIHunter>(MyPawn); 
	if (Hunter)
	{
		// Gun 타입일 경우, 사거리만큼 떨어진 위치 계산
		if (Hunter->DefaultAttackType == EAttackType::Gun)
		{
			float Range = Hunter->GetAttackRange(EAttackType::Gun);
			float KeepDistance = Range * 0.8f;

			FVector ToPlayer = (TargetLocation - MyLocation);
			float CurrentDistance = ToPlayer.Size();

			if (CurrentDistance > KeepDistance + 50.0f) // 너무 멀면 다가감
			{
				FVector Direction = ToPlayer.GetSafeNormal();
				TargetLocation = TargetLocation - Direction * KeepDistance;
			}
			else
			{
				// 이미 사거리 안에 있음: 그대로 위치 유지 (또는 공격)
				TargetLocation = MyLocation;
			}
		}
	}

	if (SearchRandom)
	{
		FNavLocation Loc;
		if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			if (NavSys->GetRandomPointInNavigableRadius(TargetLocation, SearchRadius, Loc))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), TargetLocation);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;

}
