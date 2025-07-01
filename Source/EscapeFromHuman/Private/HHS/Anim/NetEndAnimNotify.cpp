// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/NetEndAnimNotify.h"

#include "HHS/Actor/LassoActor.h"
#include "HHS/AI/NetHunter.h"

void UNetEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp , Animation);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		{
			if (ANetHunter* NetHunter = Cast<ANetHunter>(MeshComp->GetOwner()))
			{
				if (NetHunter->bShouldEndPull && NetHunter->CurrentLasso)
				{
					NetHunter->EndPull();
				}
			}
		}
	}
}
