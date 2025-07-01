// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/AnimalSkillComponent.h"
#include "LJW/AnimalBase.h"
#include "EscapeFromHuman.h"
#include "Components/BoxComponent.h"
#include "LJW/StatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LJW/AnimalDataAsset.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

UAnimalSkillComponent::UAnimalSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAnimalSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<AAnimalBase> ( GetOwner () );
	// ...
	
}

void UAnimalSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAnimalSkillComponent::ChickenSkill ()
{
	if (me->CounterPlayer)
	{
		if (me->CounterPlayer->StatusComp->bCanSkilled)
		{
			ChickenEffectOn ();
			StartFeared ();
			GetWorld()->GetTimerManager().SetTimer(FearTimer, this, &UAnimalSkillComponent::EndFeared, 1.5f, false);

			FTimerHandle TempHandle;
			GetWorld()->GetTimerManager().SetTimer(
			TempHandle,
			this,
			&UAnimalSkillComponent::ChickenEffectOff,
			1.f,
			false
			);
		}
	}
}

void UAnimalSkillComponent::CowSkill ()
{
	SpawnObject ();
}

void UAnimalSkillComponent::SheepSkill ()
{
	if (me->CounterPlayer)
	{
		if (me->CounterPlayer->StatusComp->bCanSkilled)
		{
			SheepEffectOn ();

			FTimerHandle TempHandle;
			GetWorld ()->GetTimerManager ().SetTimer (
			TempHandle ,
			this ,
			&UAnimalSkillComponent::SheepEffectOff ,
			1.f ,
			false
			);
		
			if (!me->StatusComp->GameStart) return;

			if (!me->IsLocallyControlled ())
			{
				ShowFluffEffectStart ();
				GetWorld()->GetTimerManager().SetTimer(FluffTimer, this, &UAnimalSkillComponent::ShowFluffEffectEnd, 3.f, false);

			
			}
		}
	}
}

float  UAnimalSkillComponent::EaseOutBounce( float x )
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1 / d1)
	{
		return n1 * x * x;
	}
	else if (x < 2 / d1)
	{
		x -= 1.5f / d1;
		return n1 * x * x + 0.98f;
	}
	else if (x < 2.5f / d1)
	{
		x -= 2.25f / d1;
		return n1 * x * x + 0.9875f;
	}
	else
	{
		x -= 2.625f / d1;
		return n1 * x * x + 0.984375f;
	}
}

float  UAnimalSkillComponent::FlippedEaseOutBounce( float x )
{
	return 1.0f - EaseOutBounce( x );
}

void  UAnimalSkillComponent::ApplyBouncingEffect( AActor* TargetActor , FVector Location , float BounceHeight , float Duration )
{
	if (!TargetActor) return;

	float StartTime = GetWorld()->GetTimeSeconds();

	GetWorld()->GetTimerManager().SetTimer( TimerHandle , [this , TargetActor , StartTime , Duration , Location , BounceHeight]()
	{
		float Elapsed = GetWorld()->GetTimeSeconds() - StartTime;
		float t = Elapsed / Duration; 

		if (t > 1.0f)
		{
			GetWorld()->GetTimerManager().ClearTimer( TimerHandle );
			return;
		}

		float BounceOffset = FlippedEaseOutBounce( t ) * BounceHeight;

		FVector NewLocation = Location + FVector( 0 , 0 , BounceOffset );
		TargetActor->SetActorLocation( NewLocation );

	} , 0.016f , true );
}

void UAnimalSkillComponent::SpawnObject()
{
	FVector ActorLocation = me->GetActorLocation() - FVector(0.f, 0.f, 60.f);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ActorLocation);

	ACowSkill* SpawnedActor = GetWorld()->SpawnActor<ACowSkill>(me->CowSkillFactory, SpawnTransform);
	if (me->type == 0)
	{
		SpawnedActor->boxcomp->SetCollisionObjectType ( ECC_GameTraceChannel4 );
		SpawnedActor->boxcomp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Block);
		SpawnedActor->meshcomp->SetCollisionObjectType ( ECC_GameTraceChannel4 );
		SpawnedActor->meshcomp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Block);
	}
	else if (me->type == 1)
	{
		SpawnedActor->boxcomp->SetCollisionObjectType ( ECC_GameTraceChannel5 );
		SpawnedActor->boxcomp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
		SpawnedActor->meshcomp->SetCollisionObjectType ( ECC_GameTraceChannel5 );
		SpawnedActor->meshcomp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
	}

	if (SpawnedActor)
	{
		ApplyBouncingEffect(SpawnedActor, ActorLocation, 400.0f, 0.3f);
		SkillQueue.Enqueue(SpawnedActor);

		FTimerHandle TempHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TempHandle,
			this,
			&UAnimalSkillComponent::DestroyOldestActor,
			5.0f,
			false
		);
	}
}

void UAnimalSkillComponent::DestroyOldestActor()
{
	ACowSkill* Oldest = nullptr;
	if (SkillQueue.Dequeue(Oldest) && IsValid(Oldest))
	{
		Oldest->Destroy();
	}
}

void UAnimalSkillComponent::StartFeared ()
{
	auto PC = Cast<APlayerController> ( me->CounterPlayer->GetController () );
	me->CounterPlayer->DisableInput (PC);
	me->CounterPlayer->StatusComp->bFeared = true;
	me->CounterPlayer->GetCharacterMovement ()->MaxWalkSpeed = 100;
}

void UAnimalSkillComponent::EndFeared ()
{
	auto PC = Cast<APlayerController> ( me->CounterPlayer->GetController () );
	me->CounterPlayer->EnableInput (PC);
	me->CounterPlayer->GetCharacterMovement()->MaxWalkSpeed = me->CounterPlayer->AnimalData->AnimalStats.AnimalWalkSpeed;
	me->CounterPlayer->StatusComp->bFeared = false;
}

void UAnimalSkillComponent::ShowFluffEffectStart ()
{
	if (FluffWidgetClass)
    {
        FluffWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), FluffWidgetClass);
        if (FluffWidgetInstance)
        {
			FluffWidgetInstance->AddToViewport();
        }
    }
}

void UAnimalSkillComponent::ShowFluffEffectEnd ()
{
	if (FluffWidgetClass)
    {
        if (FluffWidgetInstance)
        {
			FluffWidgetInstance->RemoveFromParent();
        }
    }
}

void UAnimalSkillComponent::ChickenEffectOn ()
{
	me->ChickenNiagaraComp->SetVisibility ( true );
	me->ChickenNiagaraComp->Activate ( true );
}

void UAnimalSkillComponent::ChickenEffectOff ()
{

	me->ChickenNiagaraComp->SetVisibility ( false );
	me->ChickenNiagaraComp->Activate ( false );
}

void UAnimalSkillComponent::SheepEffectOn ()
{
	me->SheepNiagaraComp->SetVisibility ( true );
	me->SheepNiagaraComp->Activate ( true );
}

void UAnimalSkillComponent::SheepEffectOff ()
{
	me->SheepNiagaraComp->SetVisibility ( false );
	me->SheepNiagaraComp->Activate ( false );
}

