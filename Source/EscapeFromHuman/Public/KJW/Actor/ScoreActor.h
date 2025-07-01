// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreActor.generated.h"

UENUM ()
enum class EScoreState : uint8
{
	NONE,
	TRAKING,
	STEAL,
	HOME,
	TOP,
};

DECLARE_DELEGATE_OneParam (FReturnObejct , AScoreActor*)

UCLASS()
class ESCAPEFROMHUMAN_API AScoreActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreActor();

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
protected:
	UPROPERTY ( EditDefaultsOnly )
	class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RingComp;

	UPROPERTY(EditDefaultsOnly)
	class UChildActorComponent* lightComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InHome ( FVector Location );
	void OutHome ();

	void OnInvisible ( bool bShow );
	
	FReturnObejct OnReturnObj;

public:
	UPROPERTY( ReplicatedUsing = OnRep_ScoreState, VisibleAnywhere)
	EScoreState ScoreState = EScoreState::NONE;

	UPROPERTY ()
	class AEFHPlayerState* PS = nullptr;
	
	
protected:
	UPROPERTY(EditDefaultsOnly)
	int32 Score = 1;

	UPROPERTY ( EditDefaultsOnly )
	TArray<UStaticMesh*> MeshList;


	FVector OrginLocation = FVector::ZeroVector;
	float ElapsedTime = 0.0f;


	 float StopDistance = 100.0f;
	 float FollowSpeed = 0.0f; 
	 UPROPERTY(EditDefaultsOnly)
	 float FollowAddSpeed = 150.0f; 
	 UPROPERTY(EditDefaultsOnly)
	 float FollowMaxSpeed = 700.0f; 


	UPROPERTY()
	UMaterialInstanceDynamic* RingDynMaterial;

	UPROPERTY()
	class AAnimalBase* OwnerAnimal = nullptr;
	
	//집 정보
	UPROPERTY ()
	class AAnimalBase* HomeAnimal = nullptr;


	bool bHome = false;
	FVector HomePos = FVector::ZeroVector;

	//투명화
	UPROPERTY(EditDefaultsOnly)
	class UMaterial* MT_Invisible;
	UPROPERTY(EditDefaultsOnly)
	class UMaterial* MT_UnInvisible;

	//등장 이펙트
	UPROPERTY ( EditDefaultsOnly )
	class UNiagaraSystem* NS_SpawnEffect;
	//
	UPROPERTY ( EditDefaultsOnly )
	class UNiagaraSystem* NS_GetScoreEffect;


protected:

	UFUNCTION ()
	void OnOverlapBeginComponent ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult );
	
	void SetRandomMesh ();

	void SetStateNone ();

	UFUNCTION() 
	void OnRep_ScoreState ();

	void SetOwnerAnimal ( class AAnimalBase* Animal );

	void Move ( float DeltaTime );
	
	void ScoreDrop ();
	void SpawnEffect ();
	void SpawnGetEffect ();

	UFUNCTION ( Server , Reliable )
	void ServerRPC_SetLocation ( FVector Location );


	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_SetMesh ( int32 MeshIndex );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_SetLocation ( FVector Location );
	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_SetScale ( float Scale );

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetRing (int32 TeamIndex);
	UFUNCTION( NetMulticast , Reliable)
	void MulticastRPC_SetRing (int32 TeamIndex);

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ShowMesh (bool bShow );
	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_SpawnGetEffect ();

	void OnReadyDestroy ();
};
