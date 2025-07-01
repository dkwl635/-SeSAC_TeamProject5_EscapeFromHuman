// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/GunAnimNotify.h"

#include "HHS/AI/GunHunter.h"

void UGunAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AGunHunter* GunHunter = Cast<AGunHunter>(MeshComp->GetOwner()))
		{
			GunHunter->AttackStart();
		}
	}
}
