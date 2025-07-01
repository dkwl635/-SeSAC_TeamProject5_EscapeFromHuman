// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/SpinNetAnimNotify.h"

#include "HHS/AI/NetHunter.h"

void USpinNetAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp , Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ANetHunter* NetHunter = Cast<ANetHunter>(MeshComp->GetOwner()))
		{
			if (NetHunter->HasAuthority()) // 서버 인스턴스에서만 실행
			{
				NetHunter->StartLassoSpin();
			}
		}
	}
}

