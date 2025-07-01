// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "CowSkill.h"
#include "AnimalSkillComponent.generated.h"


UCLASS ( ClassGroup = (Custom) , meta = (BlueprintSpawnableComponent) )
class ESCAPEFROMHUMAN_API UAnimalSkillComponent : public UActorComponent
{
	GENERATED_BODY ()

public:
	// Sets default values for this component's properties
	UAnimalSkillComponent ();

protected:
	// Called when the game starts
	virtual void BeginPlay () override;

public:
	// Called every frame
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY ()
	class AAnimalBase* me;

	void ChickenSkill ();
	void CowSkill ();
	void SheepSkill ();

	FTimerHandle TimerHandle;

	float EaseOutBounce ( float x );
	float FlippedEaseOutBounce ( float x );
	void ApplyBouncingEffect ( AActor* TargetActor , FVector Location , float BounceHeight , float Duration );

	void SpawnObject ();
	void DestroyOldestActor();
	TQueue<ACowSkill*> SkillQueue;

	void StartFeared ();
	void EndFeared ();

	FTimerHandle FearTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> FluffWidgetClass;

	UUserWidget* FluffWidgetInstance;

	FTimerHandle FluffTimer;

	void ShowFluffEffectStart ();
	void ShowFluffEffectEnd ();

	void ChickenEffectOn();
	void ChickenEffectOff();

	void SheepEffectOn ();
	void SheepEffectOff ();
};
