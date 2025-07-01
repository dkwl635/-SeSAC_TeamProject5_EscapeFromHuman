// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class ESCAPEFROMHUMAN_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool Attack(EAttackType AttackType);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	//int MeleeAttack();
//
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	//int TranquilizerAttack();
//
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	//int NetAttack();
};
