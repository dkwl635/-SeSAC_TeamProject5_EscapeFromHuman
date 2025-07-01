// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnimalAnimInstance.h"
#include "AnimalDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAnimalInfo {
	GENERATED_BODY ()
	
	UPROPERTY(EditAnywhere)
	float AnimalWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AnimalSkillCoolTime;

	UPROPERTY ( EditAnywhere )
	USkeletalMesh* AnimalMesh1;

	UPROPERTY ( EditAnywhere )
	USkeletalMesh* AnimalMesh2;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	TSubclassOf<UAnimalAnimInstance> AnimBlueprint;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	UAnimMontage* HitMontage;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	UAnimMontage* AttackMontage;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	UTexture2D* AnimalTexture;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	FVector MeshScale;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	int32 animaltype;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	UStaticMesh* IceMesh;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	UTexture2D* Skillicon;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	FText SkillName;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	FText AnimalName;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	FText Skillinfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector IceScale;
};

struct FSkillCooldown
{
	float SkillCool = 10.f;
};

UCLASS()
class ESCAPEFROMHUMAN_API UAnimalDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAnimalInfo AnimalStats;

	void LoadSkillCool ();
};