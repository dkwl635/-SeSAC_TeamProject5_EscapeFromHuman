// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/PullLoopAnimNotify.h"

#include "HHS/AI/NetHunter.h"

void UPullLoopAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp , Animation);
	
	if (!MeshComp) return;

	ANetHunter* NetHunter = Cast<ANetHunter>(MeshComp->GetOwner());
	if (!NetHunter || !NetHunter->AttackMontage) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	UE_LOG(LogTemp, Warning, TEXT("[LoopNotify] bShouldEndPull: %s"), NetHunter->bShouldEndPull ? TEXT("true") : TEXT("false"));

	if (NetHunter->bShouldEndPull)
	{
		// 키 눌렀으면 End 섹션으로 점프 (루프 종료)
		AnimInstance->Montage_JumpToSection(FName("End"), NetHunter->AttackMontage);
	}
	else
	{
		// 키 안눌렀으면 다시 Loop 섹션으로 점프 (루프 계속)
		AnimInstance->Montage_JumpToSection(FName("Loop"), NetHunter->AttackMontage);
	}
}
