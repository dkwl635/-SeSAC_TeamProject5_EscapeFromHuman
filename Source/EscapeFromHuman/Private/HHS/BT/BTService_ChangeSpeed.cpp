// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BT/BTService_ChangeSpeed.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/BaseAIHunter.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp , NodeMemory);
	if (auto const Cont = OwnerComp.GetAIOwner())
	{
		if (auto* const Hunter = Cast<ABaseAIHunter>(Cont->GetPawn()))
		{
			Hunter->GetCharacterMovement()->MaxWalkSpeed = Speed;	// 캐릭터 최대 걷기 속도 설정
		}
	}
}
