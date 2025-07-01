// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "IPlayerInterAction.h"
#include "CowSkill.h"
#include "AnimalBase.generated.h"

class AAnimalBase;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractionEvent, AAnimalBase*);
DECLARE_MULTICAST_DELEGATE (FOnHitEvent);
DECLARE_MULTICAST_DELEGATE ( FOnHasteEvent );
DECLARE_MULTICAST_DELEGATE ( FOnChangeHPValue );
DECLARE_MULTICAST_DELEGATE_OneParam ( FOnInvisibleEvent , bool );
DECLARE_MULTICAST_DELEGATE_OneParam ( FOnStunEvent , int32 );


UCLASS()
class ESCAPEFROMHUMAN_API AAnimalBase : public ACharacter, public IIPlayerInterAction, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAnimalBase();
	FOnInteractionEvent OnInteractionEvent;
	FOnHitEvent OnHitEvent;
	FOnHasteEvent OnHasteEvent;
	FOnInvisibleEvent OnInvisible;
	FOnChangeHPValue OnChangeHPValue;
	FOnStunEvent OnStunEvent;

	// TeamID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 ID = 2;

	virtual FGenericTeamId GetGenericTeamId() const override {return TeamId;}

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy ( AController* NewController ) override;
	virtual void OnRep_Controller () override;
	void InitControl ();
	FGenericTeamId TeamId;
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ThrowItem(EThrowType ThrowType)override;

	virtual void BuffItem ( EUSINGType BuffType ) override;

	UPROPERTY(EditAnywhere)
	class UPlayerInputComponent* InputComp;

	UPROPERTY(EditAnywhere)
	class UItemComponent* ItemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatusComponent* StatusComp;

	UPROPERTY(EditAnywhere)
	class UAnimalSkillComponent* SkillComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* dashrange;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* CamComp;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* IMC_Animals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* ArrowComp;

	UPROPERTY ( EditAnywhere )
	class UNiagaraComponent* ProjectileNiagaraComp;
	UPROPERTY ( EditAnywhere )
	class UNiagaraComponent* RingNiagaraComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* StateNiagaraComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* HasteNiagaraComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ChickenNiagaraComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* SheepNiagaraComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* DashNiagaraComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* RingComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SkillRange;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshcomp;

	UPROPERTY(EditAnywhere)
	class USceneComponent* Scenecomp;

	UPROPERTY(EditAnywhere)
	FVector icesize;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetPlayerType(int32 PlayerType);

	FTimerHandle HasteTimer;
	void Haste ();
	void HasteOn ();
	void HasteOff ();

	FTimerHandle InvisibleTimer;
	void Invisible ();
	void InvisibleOn ();
	void InvisibleOff ();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Invisible();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Invisible();

	void HitStone ();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HitStone();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HitStone();

	FTimerHandle HitTrapTimer;
	void HitTrap ();
	void TrapOn ();
	void TrapOff ();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HitTrap();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HitTrap();

	void Die ();

	void Respawn ();

	FTimerHandle RespawnTimer;

	UPROPERTY(EditAnywhere)
	FVector ASite;// = FVector( 7.820001f , -2280.3893180f, 60.1503710f );

	UPROPERTY(EditAnywhere)
	FVector BSite;// = FVector( 10.000000f , 2200.587609f ,  60.560645f );

	class AEFHPlayerController* EFHPC;

	UPROPERTY()
	class APlayerController* PC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal")
    class UAnimalDataAsset* AnimalData;

	void ApplyAnimalData();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ApplyAnimalData(int32 playertype);

	class UAnimalAnimInstance* AnimalAnim;

	FTimerHandle HitGunTimer;
	void HitGun ();
	void GunSlowOn ();
	void GunSlowOff ();
	void ApplyDamage ( int damage );
	void DashAttack (AAnimalBase* player1, AAnimalBase* player2);

	UFUNCTION ( Server , Reliable )
	void ServerRPC_HitGun();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HitGun();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DashAttack(AAnimalBase* player1, AAnimalBase* player2);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_DashAttack(AAnimalBase* player1, AAnimalBase* player2);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* hitmontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* attackmontage;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HasteEffectOn();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HasteEffectOff();

#pragma region 올가미
	void HandleDashStart();

	
	UPROPERTY()
	class ANetHunter* GrabbedByHunter;
	
	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bIsGrabbedByHunter = false;

	// 올가미에 걸렸는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCaughtByLasso = false;

	// 올가미에서 벗어나기 위해 스페이스를 누른 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 LassoEscapePressCount = 0;

	// 스페이스를 누를 수 있는 상태인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanTryEscapeLasso = false;

	void HitLasso();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_HitLasso();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HitLasso();

	void EscapeLasso();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_EscapeLasso();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_EscapeLasso();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lasso")
	UAnimMontage* LassoCaughtMontage = nullptr;

	UPROPERTY(BlueprintReadWrite, Category="Lasso")
	bool bShouldEndPull = false;
	
	// 최대 누를 횟수
	const int32 MaxSpaceBarPress = 10;
	
#pragma endregion 
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetRingColor (int32 NewColor );

	AActor* LastHitActor = nullptr;
	
	UPROPERTY()
	TSet<AActor*> TransparentActors;
	
	void SetActorMeshVisibility ( AActor* TargetActor , bool bVisible );

	UFUNCTION()
	void OnAnimalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	int32 animaltype = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACowSkill> CowSkillFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 type = 0;

	UFUNCTION()
	void OnSkillRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSkillRangeEndOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex);

	class AAnimalBase* CounterPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* skillicon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText skillname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText skillinfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText animalName;

	void DeadEvent (AAnimalBase* player);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DeadEvent(AAnimalBase* player);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DeadEvent( AAnimalBase* player);

	class AEFHGameState* GS;
};
