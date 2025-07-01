// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KJW/UIGamePlayTag.h"
#include "UIActorComponent.generated.h"


UCLASS ( ClassGroup = (Custom) , meta = (BlueprintSpawnableComponent) )
class ESCAPEFROMHUMAN_API UUIActorComponent : public UActorComponent
{
	GENERATED_BODY ()

public:
	// Sets default values for this component's properties
	UUIActorComponent ();

protected:
	// Called when the game starts
	virtual void BeginPlay () override;

public:
	// Called every frame
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;

	void ShowUI (FGameplayTag UITag);
	void HideUI ( FGameplayTag UITag );

	UPROPERTY ()
	class UAnimalMainUI* MainUI;
protected:
	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class UAnimalMainUI> MainUIClass;

	UPROPERTY ()
	class AEFHPlayerController* AnimalPc;
	
protected:
	UFUNCTION ( Server , Reliable )
	void ServerRPC_ShowUI ( FGameplayTag UITag );
	UFUNCTION ( Client , Reliable )
	void ClientRPC_ShowUI ( FGameplayTag UITag );


	UFUNCTION ( Server , Reliable )
	void ServerRPC_HideUI ( FGameplayTag UITag );
	UFUNCTION ( Client , Reliable )
	void ClientRPC_HideUI ( FGameplayTag UITag );
};
