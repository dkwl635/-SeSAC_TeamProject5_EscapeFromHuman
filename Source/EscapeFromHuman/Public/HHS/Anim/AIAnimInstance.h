// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void TickLassoLoop(float DeltaTime);

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lasso")
	bool bIsPullingLasso;

	UPROPERTY(EditDefaultsOnly, Category = "Lasso")
	UAnimMontage* LassoMontage;
};
