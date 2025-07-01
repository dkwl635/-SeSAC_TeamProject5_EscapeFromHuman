#include "LJW/StatusComponent.h"
#include "LJW/AnimalBase.h"
#include "LJW/PlayerInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KJW/Game/EFHGameState.h"

UStatusComponent::UStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<AAnimalBase> ( GetOwner () );
	hp = maxhp;
	GetWorld ()->GetGameState<AEFHGameState> ()->OnStartStateEvent.AddUObject ( this , &UStatusComponent::StartGame );
}

void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStatusComponent::ResetShout ()
{
	bSkill = true;
}

void UStatusComponent::ResetDash ()
{
	bCanDash = true;
}

void UStatusComponent::ResetIce ()
{
	bCanIce = true;
}

void UStatusComponent::StartGame ()
{
	GameStart = true;
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatusComponent, IsIce);
	DOREPLIFETIME(UStatusComponent, bSkill);
	DOREPLIFETIME ( UStatusComponent , IsInvisible );
	DOREPLIFETIME ( UStatusComponent , IsTrapped );
	DOREPLIFETIME ( UStatusComponent , IsDash );
	DOREPLIFETIME ( UStatusComponent , IsUsingItem );
	DOREPLIFETIME ( UStatusComponent , bHitByGun );
	DOREPLIFETIME ( UStatusComponent , IsDead );
	DOREPLIFETIME ( UStatusComponent , bCanSkilled );
	DOREPLIFETIME ( UStatusComponent , bCanSkill );
	DOREPLIFETIME ( UStatusComponent , bFeared );
	DOREPLIFETIME ( UStatusComponent , bCanIce );
	DOREPLIFETIME ( UStatusComponent , hp );
	DOREPLIFETIME ( UStatusComponent , GameStart );
}