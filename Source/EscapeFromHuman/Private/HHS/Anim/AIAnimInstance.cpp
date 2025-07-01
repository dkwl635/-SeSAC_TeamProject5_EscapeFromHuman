// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/AIAnimInstance.h"


void UAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	TickLassoLoop(DeltaSeconds);
}

void UAIAnimInstance::TickLassoLoop(float DeltaTime)
{
	if (bIsPullingLasso && LassoMontage && Montage_IsPlaying(LassoMontage))
	{
		FName CurrentSection = Montage_GetCurrentSection();

		if (CurrentSection == FName("LoopPull"))
		{
			// 섹션 끝날 때 다시 시작
			Montage_JumpToSection(FName("LoopPull"), LassoMontage);
		}
	}
}
