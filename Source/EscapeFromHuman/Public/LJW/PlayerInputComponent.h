#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEFROMHUMAN_API UPlayerInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInputComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class AAnimalBase* me;

	UPROPERTY()
	class UCharacterMovementComponent* MoveComp;

	void SetupInputBinding ( class UEnhancedInputComponent* input );

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_ItemInteraction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Drop;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Ice;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_ItemChange;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Shouting;

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_ReadyAction;

	FVector Direction;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSetting)
	float WalkSpeed = 300.f;



	void Turn ( const struct FInputActionValue& InputValue );
	void LookUp ( const struct FInputActionValue& InputValue );
	void Move ( const struct FInputActionValue& InputValue );
	void DashStart ( const struct FInputActionInstance& InputValue );
	void DashEnd ();
	void ItemInterActionStarted ( const struct FInputActionInstance& InputValue );
	void ItemInterActionEnded ( const struct FInputActionInstance& InputValue );
	void Drop ( const struct FInputActionInstance& InputValue );
	void IceStart ( const struct FInputActionInstance& InputValue );
	void IceEnd ( const struct FInputActionInstance& InputValue );
	void ItemChange ( const struct FInputActionInstance& InputValue );
	void Shouting ( const struct FInputActionInstance& InputValue );
	void GameReady ( const struct FInputActionInstance& InputValue );

	FTimerHandle IceTimer;
	FTimerHandle DashTimer;
	FTimerHandle ShoutCooldownHandle;
	FTimerHandle DashCoolTimer;
	FTimerHandle IceCoolTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashCooldown = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ice")
	float IceCooldown = 3.f;

	UFUNCTION(Server, Reliable)
	void ServerRPC_DashStart();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DashStart();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DashEnd();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DashEnd();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Shouting();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Shouting();

	UFUNCTION(Server, Reliable)
	void ServerRPC_IceStart();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_IceStart();

	UFUNCTION(Server, Reliable)
	void ServerRPC_IceEnd();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_IceEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ChickenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* CowSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SheepSound;
};
