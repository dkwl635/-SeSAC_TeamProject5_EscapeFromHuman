// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Anim/HunterAnim.h"

#include "HHS/BaseAIHunter.h"

void UHunterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABaseAIHunter* hunter = Cast<ABaseAIHunter>(TryGetPawnOwner());
	if (!hunter) return;
	FVector velocity = hunter->GetVelocity();
	FVector forwardVector = hunter->GetActorForwardVector();

	Speed = FVector::DotProduct(velocity, forwardVector);
}
