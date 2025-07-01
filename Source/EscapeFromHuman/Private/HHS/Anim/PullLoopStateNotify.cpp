// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/PullLoopStateNotify.h"

#include "HHS/AI/NetHunter.h"

void UPullLoopStateNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp , Animation , TotalDuration);
	
	if (!MeshComp) return;

	ANetHunter* NetHunter = Cast<ANetHunter>(MeshComp->GetOwner());
	if (NetHunter)
	{
		NetHunter->bAlreadyJumpedToEnd = false; // 커스텀 bool 변수: 중복 점프 방지
	}
}

void UPullLoopStateNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp , Animation , FrameDeltaTime);
	
}

void UPullLoopStateNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp , Animation);

	if (!MeshComp) return;

	ANetHunter* NetHunter = Cast<ANetHunter>(MeshComp->GetOwner());
	if (!NetHunter || !NetHunter->AttackMontage) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	if (NetHunter->bShouldEndPull)
	{
		if (!NetHunter->bAlreadyJumpedToEnd)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LoopNotify] Pull Ended, jumping to 'End' section"));
			AnimInstance->Montage_JumpToSection(FName("End"), NetHunter->AttackMontage);
			NetHunter->bAlreadyJumpedToEnd = true;
		}
	}
	else
	{
		// 루프를 다시 시작
		UE_LOG(LogTemp, Warning, TEXT("[LoopNotify] Pull continuing, jumping back to 'Loop' section"));
		AnimInstance->Montage_JumpToSection(FName("Loop"), NetHunter->AttackMontage);
	}
}
