// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HunterFSM.generated.h"


UENUM(BlueprintType)
enum class EHUNTERSTATE : uint8
{
	IDLE        UMETA(DisplayName = "Idle"),
	PATROL      UMETA(DisplayName = "Patrol"),
	TRACKING    UMETA(DisplayName = "Tracking"),
	ATTACK      UMETA(DisplayName = "Attack"),
	CATCH       UMETA(DisplayName = "Catch"),
	STUN        UMETA(DisplayName = "Stun"),
	TAUNT       UMETA(DisplayName = "Taunt")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEFROMHUMAN_API UHunterFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHunterFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EHUNTERSTATE mState = EHUNTERSTATE::IDLE;

	void State_Idle();

	void State_Patrol();
	
	//void State_Tracking();
	
	void State_Attack();
	
	void State_Catch();
	
	void State_Stun();
	
	void State_Taunt();

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float AttackRange = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float MoveSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float AttackSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float IdleDelayTime = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float CurrentTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float AttackDelayTime = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class AAnimalBase* target;
	
	UPROPERTY()
	class AHunter* me;

	UPROPERTY()
	class AAIController* ai;
	
	FVector randomPos;
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);
};
