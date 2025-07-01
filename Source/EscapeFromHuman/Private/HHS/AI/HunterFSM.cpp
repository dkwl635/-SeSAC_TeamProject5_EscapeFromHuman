// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/AI/HunterFSM.h"

#include "AIController.h"
#include "AITypes.h"
#include "NavigationSystem.h"
#include "HHS/AI/Hunter.h"
#include "Navigation/PathFollowingComponent.h"
#include "EscapeFromHuman.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/AnimalBase.h"

// Sets default values for this component's properties
UHunterFSM::UHunterFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHunterFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AAnimalBase::StaticClass());
	target = Cast<AAnimalBase>(actor);
	me = Cast<AHunter>(GetOwner());
	if (me)
	{
		//Anim = Cast<>(me->GetMesh()->GetAnimInstance());
	}
	//Anim = Cast<>(me->GetMesh()->GetAnimInstance());
	ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UHunterFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch ( mState )
	{
		case EHUNTERSTATE::IDLE:		{ State_Idle();		}		break;
		case EHUNTERSTATE::PATROL:		{ State_Patrol();	}		break;
		//case EHUNTERSTATE::TRACKING:	{ State_Tracking();	}		break;
		case EHUNTERSTATE::ATTACK:		{ State_Attack();	}		break;
		case EHUNTERSTATE::CATCH:		{ State_Catch();	}		break;
		case EHUNTERSTATE::STUN:		{ State_Stun();		}		break;
		case EHUNTERSTATE::TAUNT:		{ State_Taunt();	}		break;
	}
}

void UHunterFSM::State_Idle()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if ( CurrentTime > IdleDelayTime )
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
		mState = EHUNTERSTATE::PATROL;
		CurrentTime = 0.0f;
		//Anim->AnimState = mState;
	}
}

void UHunterFSM::State_Patrol()
{
	FVector destination = target->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	ai->BuildPathfindingQuery(req, query);
	FPathFindingResult r = ns->FindPathSync(query);
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos);
		if ( result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
		}
	}
	if ( dir.Size() < AttackRange )
	{
		ai->StopMovement();
		mState = EHUNTERSTATE::ATTACK;
		//Anim->AnimState = mState;
		//Anim->bAttackPlay = true;
		CurrentTime = AttackDelayTime;
	}
}

//void UHunterFSM::State_Tracking()
//{
//}

void UHunterFSM::State_Attack()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if ( CurrentTime > AttackDelayTime )
	{
		PRINTLOG(TEXT("Attack"));
		CurrentTime = 0.0f;
		//Anim->bAttackPlay = true;
	}
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if ( distance > AttackRange )
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.f, randomPos);
		mState = EHUNTERSTATE::PATROL;
		//Anim->AnimState = mState;
	}
}

void UHunterFSM::State_Catch()
{
}

void UHunterFSM::State_Stun()
{
}

void UHunterFSM::State_Taunt()
{
}

bool UHunterFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

