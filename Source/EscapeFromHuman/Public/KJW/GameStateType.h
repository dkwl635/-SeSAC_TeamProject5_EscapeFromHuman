// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM ()
enum class EGameState : uint8
{
	READY,
	GAME ,
	END ,
	MAX ,
};


UENUM ()
enum class EGameTeam : uint8
{
	A,
	B,
	MAX ,
};

