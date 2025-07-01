// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowMeshActor.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API AArrowMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowMeshActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	UStaticMesh* ArrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	UMaterialInterface* ArrowMaterial;

	void SetupArrowMeshes(int32 Count, float Spacing, float Duration, const FVector& StartLocation, const FVector& Direction);

protected:
	FTimerHandle HideTimerHandle;

	UPROPERTY()
	TArray<UStaticMeshComponent*> ArrowMeshComponents;

	void HideAll();
};