// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimalAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UAnimalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// 플레이어 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimalAnimInstance)
	float Speed = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimalAnimInstance)
	float SpeedMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimalAnimInstance)
	bool IsDash = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimalAnimInstance)
	bool IsDead = false;
};
