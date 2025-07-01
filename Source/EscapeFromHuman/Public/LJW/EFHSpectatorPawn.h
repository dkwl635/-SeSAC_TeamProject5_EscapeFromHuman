// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "EFHSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEFROMHUMAN_API AEFHSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	AEFHSpectatorPawn ();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* CamComp;
};
