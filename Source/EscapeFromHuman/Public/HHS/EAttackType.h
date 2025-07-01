#pragma once

#include "CoreMinimal.h"
#include "EAttackType.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None		 UMETA(DisplayName = "None"),
	Melee		 UMETA(DisplayName = "Melee"),
	Gun			 UMETA(DisplayName = "Gun"),
	Net			 UMETA(DisplayName = "Net")
}; 