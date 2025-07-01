// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/HitSoundNotify.h"

#include "HHS/AI/MeleeHunter.h"

void UHitSoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp , Animation);
	
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	AMeleeHunter* MeleeHunter = Cast<AMeleeHunter>(Owner);
	if (MeleeHunter && MeleeHunter->bDidHitPlayer)
	{
		MeleeHunter->Multicast_PlayHitSound(MeleeHunter->LastHitLocation);
	}
}