// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Actor/PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

FVector APatrolPath::GetPatrolPoint(int const index) const
{
	if (PatrolPoints.IsValidIndex(index))
	{
		return PatrolPoints[index];
	}

	// 유효하지 않은 인덱스일 경우 기본값 반환 또는 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("Invalid patrol point index: %d"), index);
	return FVector::ZeroVector;
}

int APatrolPath::Num() const
{
	return PatrolPoints.Num();
}


