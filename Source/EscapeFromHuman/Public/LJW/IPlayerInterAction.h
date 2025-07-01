// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KJW/ItemType.h"
#include "IPlayerInterAction.generated.h"

UINTERFACE(MinimalAPI)
class UIPlayerInterAction : public UInterface
{
	GENERATED_BODY()
};

class ESCAPEFROMHUMAN_API IIPlayerInterAction
{
	GENERATED_BODY()

public:
    virtual void UseItem(EItemType ItemType);
    virtual void ThrowItem(EThrowType ThrowType);
	virtual void BuffItem ( EUSINGType BuffType );
};
