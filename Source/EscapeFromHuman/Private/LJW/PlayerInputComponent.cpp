#include "LJW/PlayerInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedPlayerInput.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "LJW/AnimalBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KJW/ActorComponent/ItemComponent.h"
#include "LJW/StatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "LJW/AnimalAnimInstance.h"
#include "LJW/AnimalSkillComponent.h"
#include "EscapeFromHuman.h"
#include "Engine/OverlapResult.h"
#include "HHS/BaseAIHunter.h"
#include "HHS/UI/AI_UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "KJW/Game/EFHPlayerState.h"
#include "Components/SphereComponent.h"


UPlayerInputComponent::UPlayerInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<AAnimalBase> ( GetOwner () );
	MoveComp = me->GetCharacterMovement ();
	MoveComp->MaxWalkSpeed = WalkSpeed;
}

void UPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (me->bCaughtByLasso) 
	{
		// 올가미 상태면 이동 안 함
		Direction = FVector::ZeroVector;
		return;
	}
	
	if (me->StatusComp->bFeared)
	{
		Direction = (me->CounterPlayer->GetActorLocation() - me->GetActorLocation())*(-1);
		me->AddMovementInput ( Direction.GetSafeNormal() );
	}
	else
	{
		Direction = FTransform ( me->GetControlRotation () ).TransformVector ( Direction );
		Direction.Z = 0.f;
		me->AddMovementInput ( Direction.GetSafeNormal() );
		Direction = FVector::ZeroVector;
	}
}

void UPlayerInputComponent::SetupInputBinding ( class UEnhancedInputComponent* input )
{
	input->BindAction ( IA_LookUp , ETriggerEvent::Triggered , this , &UPlayerInputComponent::LookUp );
	input->BindAction ( IA_Turn , ETriggerEvent::Triggered , this , &UPlayerInputComponent::Turn );
	input->BindAction ( IA_Move , ETriggerEvent::Triggered , this , &UPlayerInputComponent::Move );
	input->BindAction ( IA_Run , ETriggerEvent::Started , this , &UPlayerInputComponent::DashStart );
	input->BindAction ( IA_ItemInteraction , ETriggerEvent::Started , this , &UPlayerInputComponent::ItemInterActionStarted );
	input->BindAction ( IA_ItemInteraction , ETriggerEvent::Completed , this , &UPlayerInputComponent::ItemInterActionEnded );
	input->BindAction ( IA_Drop , ETriggerEvent::Started , this , &UPlayerInputComponent::Drop );
	input->BindAction ( IA_Ice , ETriggerEvent::Started , this , &UPlayerInputComponent::IceStart );
	input->BindAction ( IA_Ice , ETriggerEvent::Completed , this , &UPlayerInputComponent::IceEnd );
	input->BindAction ( IA_ItemChange , ETriggerEvent::Started , this , &UPlayerInputComponent::ItemChange );
	input->BindAction ( IA_Shouting , ETriggerEvent::Started , this , &UPlayerInputComponent::Shouting );
	input->BindAction ( IA_ReadyAction , ETriggerEvent::Started , this , &UPlayerInputComponent::GameReady );
}

void UPlayerInputComponent::Turn ( const struct FInputActionValue& InputValue )
{
	float value = InputValue.Get<float> ();
	me->AddControllerYawInput ( value * 0.5f);
}

void UPlayerInputComponent::LookUp ( const struct FInputActionValue& InputValue )
{
	float value = InputValue.Get<float> ();
	me->AddControllerPitchInput ( value * 0.5f );
}

void UPlayerInputComponent::Move ( const struct FInputActionValue& InputValue )
{
	if (me->bCaughtByLasso || me->StatusComp->IsIce || me->StatusComp->IsTrapped)
	{
		// 이동 입력 무시
		return;
	}
	
	if (!me->StatusComp->IsIce && !me->StatusComp->IsTrapped)
	{
		FVector2D value = InputValue.Get<FVector2D> ();
		Direction.X = value.X;
		Direction.Y = value.Y; 
	}
}

// 아이템 줍기, 사용 (좌클릭)
void UPlayerInputComponent::ItemInterActionStarted ( const struct FInputActionInstance& InputValue )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "InterActionStarted" ) );
	if (!me->StatusComp->IsIce)
	{
		me->StatusComp->IsUsingItem = true;
		if (me->ItemComp->GetCurrentItem())
		{
			me->GetCharacterMovement()->bOrientRotationToMovement = false;
			me->bUseControllerRotationYaw = true;
			me->ItemComp->ItemStarted();
		}
		else
		{
			me->OnInteractionEvent.Broadcast (me);
		}
	}
}

void UPlayerInputComponent::ItemInterActionEnded ( const struct FInputActionInstance& InputValue )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "InterActionEnd" ) );
	if (me->ItemComp->GetCurrentItem ())
	{
		me->GetCharacterMovement()->bOrientRotationToMovement = true;
		me->bUseControllerRotationYaw = false;
		me->StatusComp->IsUsingItem = false;
		me->ItemComp->ItemCompleted();
	}
}

// 아이템 버리기 (G키)
void UPlayerInputComponent::Drop ( const struct FInputActionInstance& InputValue )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Drop" ) );
	me->ItemComp->DropItem ();
}

// 얼음 스킬 (E키)
void UPlayerInputComponent::IceStart ( const struct FInputActionInstance& InputValue )
{
	ServerRPC_IceStart();
}

void UPlayerInputComponent::ServerRPC_IceStart_Implementation ()
{
	MulticastRPC_IceStart ();
	
}

void UPlayerInputComponent::MulticastRPC_IceStart_Implementation ()
{
	if (me->StatusComp->bCanIce)
	{
		if (me->StatusComp->IsInvisible && !me->IsLocallyControlled ())
		{
			me->meshcomp->SetVisibility ( false );
		}
		else
		{
			me->meshcomp->SetVisibility ( true );
		}
		me->StatusComp->IsIce = true;
		me->bUseControllerRotationYaw = false;
		me->StatusComp->bCanIce = false;
		GetWorld()->GetTimerManager().SetTimer(IceCoolTimer,me->StatusComp, &UStatusComponent::ResetIce, IceCooldown, false);

		// AIHunter들에게 얼음 상태 알림
		TArray<FOverlapResult> Overlaps;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(me);

		bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps,me->GetActorLocation(),FQuat::Identity,ECC_Pawn, // Pawn 채널로 감지
			FCollisionShape::MakeSphere(500.f), // 감지 반경 설정
			QueryParams
		);

		if (bHit)
		{
			for (auto& Result : Overlaps)
			{
				ABaseAIHunter* Hunter = Cast<ABaseAIHunter>(Result.GetActor());
				if (Hunter)
				{
					Hunter->SetAlertType(EAlertType::Question); // 물음표 UI 표시
				}
			}
		}
	}
}

void UPlayerInputComponent::IceEnd ( const struct FInputActionInstance& InputValue )
{
	ServerRPC_IceEnd ();
}

void UPlayerInputComponent::ServerRPC_IceEnd_Implementation ()
{
	MulticastRPC_IceEnd ();
}

void UPlayerInputComponent::MulticastRPC_IceEnd_Implementation ()
{
	me->StatusComp->IsIce = false;
	me->meshcomp->SetVisibility ( false );
}

// 아이템 변경 (R키)
void UPlayerInputComponent::ItemChange ( const struct FInputActionInstance& InputValue )
{
	UE_LOG ( LogTemp , Warning , TEXT ( "ItemChange" ) );
	me->ItemComp->ChangeItemSlot ();
}

// 소리치기 스킬 (Q키)
void UPlayerInputComponent::Shouting ( const struct FInputActionInstance& InputValue )
{
	if (me->StatusComp->bSkill)
	{
		ServerRPC_Shouting ();
	}
}

void UPlayerInputComponent::GameReady ( const FInputActionInstance& InputValue )
{
	if (me)
	{
		AEFHPlayerState* PS =  me->GetPlayerState<AEFHPlayerState> ();
		if (PS)
		{
			PS->SetRaedy ( true );
		}
	}
}

void UPlayerInputComponent::ServerRPC_Shouting_Implementation ()
{
	MulticastRPC_Shouting ();
}

void UPlayerInputComponent::MulticastRPC_Shouting_Implementation ()
{
	if (me->StatusComp && me->StatusComp->bSkill && !me->StatusComp->IsIce)
	{
		if (me->animaltype == 1)
		{
			if (ChickenSound)
			{
				 UGameplayStatics::PlaySound2D(this, ChickenSound,0.1f);
			}
			if (me->StatusComp->bCanSkill)
			{
				me->SkillComp->ChickenSkill ();
			}
		}
		else if (me->animaltype == 2)
		{
			if (CowSound)
			{
				UGameplayStatics::PlaySound2D(this, CowSound,0.1f);
			}
			me->SkillComp->CowSkill ();
		}
		else if (me->animaltype == 3)
		{
			if (SheepSound)
			{
				 UGameplayStatics::PlaySound2D(this, SheepSound);
			}

			if (me->StatusComp->bCanSkill)
			{
				me->SkillComp->SheepSkill ();
			}
			
		}

		// 쿨타임 적용
		me->StatusComp->bSkill = false;
		GetWorld()->GetTimerManager().SetTimer(ShoutCooldownHandle, me->StatusComp, &UStatusComponent::ResetShout, me->StatusComp->ShoutingCoolDown, false);
	}
}
//====================================================================================================

// 대쉬 (Space 키)
void UPlayerInputComponent::DashStart ( const struct FInputActionInstance& InputValue )
{
	AAnimalBase* AnimalOwner = Cast<AAnimalBase>(GetOwner());
	if (AnimalOwner)
	{
		AnimalOwner->HandleDashStart();
	}
	ServerRPC_DashStart();
}

void UPlayerInputComponent::DashEnd ()
{
	ServerRPC_DashEnd ();
}

void UPlayerInputComponent::ServerRPC_DashStart_Implementation ()
{
	MulticastRPC_DashStart ();
}

void UPlayerInputComponent::MulticastRPC_DashStart_Implementation ()
{
	if (!me->StatusComp->IsDead && me->StatusComp->bCanDash && !me->StatusComp->IsIce)
	{
		me->StatusComp->IsDash = true;
		FVector DashDirection = me->GetActorForwardVector ();
		me->LaunchCharacter ( DashDirection * 2500.0f , true , true );
		me->dashrange->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
		me->dashrange->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
		if (me->StatusComp->IsInvisible && !me->IsLocallyControlled ())
		{
			me->DashNiagaraComp->SetVisibility ( false );
			me->DashNiagaraComp->Activate ( false );
		}
		else
		{
			me->DashNiagaraComp->SetVisibility ( true );
			me->DashNiagaraComp->Activate ( true );
		}

		GetWorld ()->GetTimerManager ().SetTimer ( DashTimer , this , &UPlayerInputComponent::DashEnd , DashDuration , false );
		me->StatusComp->bCanDash = false;
		GetWorld ()->GetTimerManager ().SetTimer ( DashCoolTimer , me->StatusComp , &UStatusComponent::ResetDash , DashCooldown , false );
	}
}

void UPlayerInputComponent::ServerRPC_DashEnd_Implementation ()
{
	MulticastRPC_DashEnd ();
}

void UPlayerInputComponent::MulticastRPC_DashEnd_Implementation ()
{
	if (me->StatusComp->IsDash)
	{
		me->DashNiagaraComp->SetVisibility ( false );
		me->DashNiagaraComp->Activate ( false );
		me->StatusComp->IsDash = false;
		me->GetCharacterMovement()->StopMovementImmediately();
	}
}
//====================================================================================================
