// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/ActorComponent/UIActorComponent.h"
#include "KJW/UI/AnimalMainUI.h" 
#include "KJW/Game/EFHPlayerController.h"

// Sets default values for this component's properties
UUIActorComponent::UUIActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated ( true );
	
	// ...

	

}


// Called when the game starts
void UUIActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (!MainUIClass) return;

	AnimalPc  = Cast<AEFHPlayerController> ( GetOwner() );

	if (!AnimalPc) return;

	if (AnimalPc->IsLocalController ())
	{
		MainUI = CreateWidget<UAnimalMainUI> ( AnimalPc , MainUIClass );
		if (MainUI)
		{
			MainUI->AddToViewport ();
		}
		AnimalPc->AnimalUI = MainUI;
	}
	
}


// Called every frame
void UUIActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUIActorComponent::ShowUI ( FGameplayTag UITag )
{
	ServerRPC_ShowUI ( UITag );
}

void UUIActorComponent::HideUI ( FGameplayTag UITag )
{
	ServerRPC_HideUI ( UITag );
}

void UUIActorComponent::ServerRPC_ShowUI_Implementation ( FGameplayTag UITag )
{
	ClientRPC_ShowUI ( UITag );
}

void UUIActorComponent::ClientRPC_ShowUI_Implementation ( FGameplayTag UITag )
{
	if (MainUI)
	{
		MainUI->ShowUI ( UITag );
	}
}

void UUIActorComponent::ServerRPC_HideUI_Implementation ( FGameplayTag UITag )
{
	ClientRPC_HideUI ( UITag );
}

void UUIActorComponent::ClientRPC_HideUI_Implementation ( FGameplayTag UITag )
{
	if (MainUI)
	{
		MainUI->HideUI ( UITag );
	}
}

