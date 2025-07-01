// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowDecalActor.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API AArrowDecalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowDecalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetArrowVisible(bool bVisible);
	void SetDecalLength(int32 Count); // 길이 설정
	void DestroyDecal();

private:
	UPROPERTY()
	TArray<UDecalComponent*> ArrowDecals;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* ArrowDecalMaterial;

	UPROPERTY(EditDefaultsOnly)
	float DecalSpacing = 260.f; // 데칼 간격

	UPROPERTY(EditDefaultsOnly)
	//FVector DecalSize = FVector(32, 120, 300); // 데칼 크기
	FVector DecalSize = FVector(64.f, 200.f, 130.f);

	FTimerHandle DestroyDecalTimerHandle;

	UFUNCTION ( NetMulticast , Reliable )
	void Multicast_SetDecalLength ( int32 Count );

};
