// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HunterAnim.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UHunterAnim : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Anim)
	float Speed = 0.f;
	
};
