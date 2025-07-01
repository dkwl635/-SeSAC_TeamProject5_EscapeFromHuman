// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreBillboardActor.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API AScoreBillboardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreBillboardActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* WidgetComp;
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	UPROPERTY(EditDefaultsOnly)
	float CheckDisLen = 800.0f;

	UPROPERTY()
	class UPlayerScore* PlayerScore;
	
	UPROPERTY ()
	APawn* OtherAnimal;

	UPROPERTY ()
	class AAnimalBase* OwnerAnimal;

	FTimerHandle SetPSHandle;
	void SetPlayerState ();

	void SetPlayerTempScore ();

	void StartGame ();
	void CheckDisanceAnima ();

};
