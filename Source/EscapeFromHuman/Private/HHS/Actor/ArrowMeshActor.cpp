// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Actor/ArrowMeshActor.h"

// Sets default values
AArrowMeshActor::AArrowMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
}

void AArrowMeshActor::SetupArrowMeshes(int32 Count, float Spacing, float Duration, const FVector& StartLocation, const FVector& Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupArrowMeshes called with Count: %d"), Count);

	for (UStaticMeshComponent* Comp : ArrowMeshComponents)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	ArrowMeshComponents.Empty();

	for (int32 i = 0; i < Count; ++i)
	{
		UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(this);
		if (!MeshComp || !ArrowMesh) continue;

		MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		MeshComp->RegisterComponent();

		MeshComp->SetStaticMesh(ArrowMesh);
		MeshComp->SetMaterial(0, ArrowMaterial);

		// 방향 기준 위치 계산
		FVector Offset = Direction * (i * Spacing);
		FVector MeshLocation = StartLocation + Offset;

		MeshComp->SetWorldLocation(MeshLocation);
		MeshComp->SetWorldRotation(Direction.Rotation());
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetCastShadow(false);
		MeshComp->SetVisibility(true);

		ArrowMeshComponents.Add(MeshComp);

		UE_LOG(LogTemp, Warning, TEXT("Arrow %d 생성됨: 위치=%s"), i, *MeshLocation.ToString());
	}

	SetActorHiddenInGame(false);

	GetWorldTimerManager().ClearTimer(HideTimerHandle);
	GetWorldTimerManager().SetTimer(HideTimerHandle, this, &AArrowMeshActor::HideAll, Duration, false);
}

void AArrowMeshActor::HideAll()
{
	for (UStaticMeshComponent* Comp : ArrowMeshComponents)
	{
		if (Comp)
		{
			Comp->SetVisibility(false);
		}
	}
	SetActorHiddenInGame(true);
}