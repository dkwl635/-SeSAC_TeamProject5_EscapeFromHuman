﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CowSkill.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API ACowSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACowSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick ( float DeltaTime ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxcomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshcomp;
};
