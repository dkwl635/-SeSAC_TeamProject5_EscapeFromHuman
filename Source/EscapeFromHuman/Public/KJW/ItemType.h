// Fll out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM ()
enum class EItemType : uint8
{
	THROW ,
	USING ,
	TRAP ,
	MAX ,
};

UENUM ()
enum class EThrowType : uint8
{
	STONE , //돌
	NET , //끈끈이
	Trap , //트랩
	MAX ,
};

UENUM ()
enum class EUSINGType : uint8
{
	HASTE , //헤이스트
	INVISIBLE, //투명
	CLAIRVOYANCE,// 투시
	MAX ,
};




