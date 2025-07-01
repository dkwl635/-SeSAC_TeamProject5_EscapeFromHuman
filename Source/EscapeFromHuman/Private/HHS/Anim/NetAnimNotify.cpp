// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/NetAnimNotify.h"

#include "HHS/AI/NetHunter.h"

void UNetAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp , Animation);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ANetHunter* NetHunter = Cast<ANetHunter>(MeshComp->GetOwner()))
		{
			NetHunter->ThrowNet();
		}
	}
}
