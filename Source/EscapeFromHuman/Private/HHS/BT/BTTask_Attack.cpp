// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/BaseAIController.h"
#include "HHS/BaseAIHunter.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//PRINTLOG(TEXT("Attack Task Started"))
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AIController) {return EBTNodeResult::Failed;}
	ABaseAIHunter* Hunter = Cast<ABaseAIHunter>(AIController->GetPawn());
	if (!Hunter) {return EBTNodeResult::Failed;}
	
	// 범위 벗어나면 공격 x
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		// 성공
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	// 범위 내에 있음 -> AI컨트롤러랑 hunter 가져온다
	auto const* const cont = OwnerComp.GetAIOwner();
	//auto* const hunter = Cast<ABaseAIHunter>(cont->GetPawn());
	auto* const hunter = Cast<ABaseAIHunter>(cont ? cont->GetPawn() : nullptr);
	
	if (!hunter || !hunter->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Authority or Invalid Hunter"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	// hunter가 ICombatInterface를 지원하는 경우 Execute_MeleeAttack 함수를 호출
	if (auto* const icombat = Cast<ICombatInterface>(hunter))
	{
		// 블랙보드에서 공격 타입을 가져옴
		uint8 AttackTypeValue = OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AttackTypeKey.SelectedKeyName);
		EAttackType AttackType = static_cast<EAttackType>(AttackTypeValue);

		bool bSuccess = icombat->Execute_Attack(hunter, AttackType);
		if (!bSuccess)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}		
	}
	// 성공
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_Attack::MontageHasFinished(ABaseAIHunter* const hunter, EAttackType Type)
{
	return hunter->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(hunter->GetMontage(Type));

}
