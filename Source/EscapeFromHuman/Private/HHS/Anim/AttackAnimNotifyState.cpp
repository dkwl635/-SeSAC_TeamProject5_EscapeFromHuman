// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/AttackAnimNotifyState.h"

#include "HHS/BaseAIHunter.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseAIHunter* const ai = Cast<ABaseAIHunter>(MeshComp->GetOwner()))
		{
			ai->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ABaseAIHunter* const ai = Cast<ABaseAIHunter>(MeshComp->GetOwner()))
		{
			ai->AttackEnd();
		}
	}
}
