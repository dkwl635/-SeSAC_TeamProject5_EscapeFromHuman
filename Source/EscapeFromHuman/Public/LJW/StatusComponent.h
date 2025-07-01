#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEFROMHUMAN_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	class AAnimalBase* me;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsIce = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bSkill = true;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsInvisible = false;

	float ShoutingCoolDown = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxIceStamina = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float IceStaminaDuration = 10.f;
	
	float IceStamina = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxDashStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float DashStaminaDuration = 5;

	float DashStamina = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "dash")
	bool bCanDash = true;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ice")
	bool bCanIce = true;

	void DecreaseIceStamina ();
	void IncreaseIceStamina ();
	void IncreaseDashStamina ();
	void ResetShout ();
	void ResetDash ();
	void ResetIce ();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool GameStart = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsTrapped = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsDash = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsUsingItem = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bHitByGun = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsDead = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bCanSkilled = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bCanSkill = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bFeared = false;

	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite, Category = "HP")
	int32 hp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HP")
	int32 maxhp = 2;

	UFUNCTION()
	void StartGame();
};
