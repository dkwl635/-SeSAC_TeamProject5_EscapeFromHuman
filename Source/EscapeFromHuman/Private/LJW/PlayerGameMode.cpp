// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/PlayerGameMode.h"

void APlayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// 1. 플레이어가 스폰될 위치와 회전 설정
	AActor* PlayerStart = FindPlayerStart(NewPlayer);
	FVector SpawnLoc = PlayerStart ? PlayerStart->GetActorLocation() : FVector(0, 0, 300);
	FRotator SpawnRot = PlayerStart ? PlayerStart->GetActorRotation() : FRotator::ZeroRotator;
	
	// 2. 스폰 파라미터 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = NewPlayer; // 소유자 설정
	SpawnParams.Instigator = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 3. AnimalClass를 스폰 (AnimalClass는 AAnimalBase를 상속한 클래스)
	AAnimalBase* NewPawn = GetWorld()->SpawnActor<AAnimalBase>(AnimalClass, SpawnLoc, SpawnRot, SpawnParams);
	if (NewPawn)
	{
		// 4. 서버에서 해당 컨트롤러가 이 Pawn을 조종하게 함
		NewPlayer->Possess(NewPawn);

		UE_LOG(LogTemp, Log, TEXT("Player possessed new animal pawn"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn AnimalBase pawn for player!"));
	}
}
