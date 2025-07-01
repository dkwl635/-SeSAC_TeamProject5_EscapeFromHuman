// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BaseAIHunter.h"

#include "EscapeFromHuman.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "HHS/EAttackType.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HHS/BaseAIController.h"
#include "HHS/AI/GunHunter.h"
#include "HHS/AI/MeleeHunter.h"
#include "HHS/AI/NetHunter.h"
#include "HHS/UI/AI_UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LJW/AnimalBase.h"


ABaseAIHunter::ABaseAIHunter()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshTemp(TEXT("/Script/Engine.SkeletalMesh'/Game/HHS/FarmerPack/Character/Farmer/SKM_Farmer_male.SKM_Farmer_male'"));
	if (MeshTemp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTemp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f),FRotator(0.0f, 0.0f, -90.0f));
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bReplicates = true;
	bAlwaysRelevant = true;

	Tags.Add(FName("AI"));

	AlertWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AlertWidgetComponent"));
	AlertWidgetComponent->SetupAttachment(RootComponent);
	AlertWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	AlertWidgetComponent->SetDrawSize(FVector2D(150, 150));
	AlertWidgetComponent->SetRelativeLocation(WidgetOffset);
	AlertWidgetComponent->SetWidgetClass(AlertWidgetClass);
}

void ABaseAIHunter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AlertWidgetComponent && AlertWidgetClass)
	{
		AlertWidgetComponent->SetWidgetClass(AlertWidgetClass);
	}
}

void ABaseAIHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AlertWidgetComponent)
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			FVector CameraLocation = CameraManager->GetCameraLocation();
			FVector WidgetLocation = AlertWidgetComponent->GetComponentLocation();

			FRotator LookAtRotation = (CameraLocation - WidgetLocation).Rotation();
			LookAtRotation.Pitch = 0.f;
			LookAtRotation.Roll = 0.f;

			AlertWidgetComponent->SetWorldRotation(LookAtRotation);
		}
	}

}

void ABaseAIHunter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ABaseAIHunter::GetBehaviorTree() const
{
	return BehaviorTree;
}

APatrolPath* ABaseAIHunter::GetPatrolPath() const
{
	return PatrolPath;
}

bool ABaseAIHunter::Attack_Implementation(EAttackType AttackType)
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack_Implementation: %s"), *UEnum::GetValueAsString(AttackType));
	CurrentAttackType = AttackType;
	if (!HasAuthority()) return false;

	if (IsAttackOnCooldown(AttackType))	return false;

	UAnimMontage* MontageToPlay  = nullptr;

	switch (AttackType)
	{
	case EAttackType::Melee:
		MontageToPlay = MeleeMontage;
		StartCooldown(AttackType, 2.0f);
		break;

	case EAttackType::Gun:
		MontageToPlay = GunMontage;
		StartCooldown(AttackType, 2.5f);
		break;

	case EAttackType::Net:
		MontageToPlay = NetMontage;
		StartCooldown(AttackType, 4.0f);
		break;

	default:
		return false;
	}

	if (MontageToPlay)
	{
		DisableMovement();

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			Multicast_PlayAttackMontage(MontageToPlay);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ABaseAIHunter::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
		}
	}
	return true;
}


bool ABaseAIHunter::IsAttackOnCooldown(EAttackType Type) const
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (AttackCooldowns.Contains(Type))
	{
		float CooldownEnd = AttackCooldowns[Type];
		return CurrentTime < CooldownEnd;
	}
	return false;
}

void ABaseAIHunter::StartCooldown(EAttackType Type, float Cooldown)
{
	AttackCooldowns.Add(Type, GetWorld()->GetTimeSeconds() + Cooldown);
}

void ABaseAIHunter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EnableMovement(); // 어떤 공격이든 끝나면 이동 가능
}

UAnimMontage* ABaseAIHunter::GetMontage(EAttackType AttackType) const
{
	return nullptr;	
}

AActor* ABaseAIHunter::GetTargetActor() const
{
	if (Controller)
	{
		ABaseAIController* AIController = Cast<ABaseAIController>(Controller);
		if (AIController && AIController->GetBlackboardComponent())
		{
			return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		}
	}
	return nullptr;
}

void ABaseAIHunter::DisableMovement()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(MOVE_None);
		//Movement->DisableMovement();	
		//bUseControllerRotationYaw = false; // 회전
		Movement->bUseControllerDesiredRotation = false;
		Movement->bOrientRotationToMovement = false;
	}
}

void ABaseAIHunter::EnableMovement()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(MOVE_Walking);
		//bUseControllerRotationYaw = false;
		Movement->bUseControllerDesiredRotation = true;
		Movement->bOrientRotationToMovement = true;
	}
}


void ABaseAIHunter::AttackStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("Base AttackStart"));
}

void ABaseAIHunter::AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Base AttackEnd"));
}

float ABaseAIHunter::GetAttackRange(EAttackType AttackType) const
{
	switch (AttackType)
	{
	case EAttackType::Melee:
			if (const AMeleeHunter* MeleeHunter = Cast<AMeleeHunter>(this))
			{
				return MeleeHunter->MeleeAttackRange;
			}
		return 0.f;

	case EAttackType::Gun:
		if (const AGunHunter* GunHunter = Cast<AGunHunter>(this))
		{
			return GunHunter->GunAttackRange;
		}
			return 0.f;

	case EAttackType::Net:
		if (const ANetHunter* NetHunter = Cast<ANetHunter>(this))
		{
			return NetHunter->NetAttackRange;
		}
		return 0.f;

	default:
		return 0.f;
	}
}


void ABaseAIHunter::Multicast_PlayAttackMontage_Implementation(UAnimMontage* AnimMontage)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(AnimMontage);
	}
}


void ABaseAIHunter::OnRep_CanSeePlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_CanSeePlayer: %s"), bCanSeePlayer ? TEXT("TRUE") : TEXT("FALSE"));
}

void ABaseAIHunter::OnRep_HeardNoise()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_HeardNoise: %s"), bHeardNoise ? TEXT("TRUE") : TEXT("FALSE"));
}


void ABaseAIHunter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABaseAIHunter, bCanSeePlayer);
	DOREPLIFETIME(ABaseAIHunter, bHeardNoise);
	DOREPLIFETIME(ABaseAIHunter, HeardLocation);
}


void ABaseAIHunter::SetAlertType(EAlertType NewType)
{
	ServerRPC_SetAlertType(NewType);
}

void ABaseAIHunter::ServerRPC_SetAlertType_Implementation(EAlertType NewType)
{
	MulticastRPC_SetAlertType(NewType);
}

void ABaseAIHunter::MulticastRPC_SetAlertType_Implementation(EAlertType NewType)
{
	// 위젯 컴포넌트 존재 확인
	if (!AlertWidgetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AlertWidgetComponent is null"));
		return;
	}

	// 위젯 캐스팅
	UAI_UserWidget* Widget = Cast<UAI_UserWidget>(AlertWidgetComponent->GetUserWidgetObject());
	if (!Widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("AlertWidget is null or wrong class"));
		return;
	}

	// UI 아이콘 무조건 강제 갱신
	Widget->UpdateIcon(NewType);

	if (NewType == EAlertType::Question && QuestionSound.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, QuestionSound.Num() - 1);
		USoundBase* SelectedSound = QuestionSound[Index];
	
		if (SelectedSound)
		{
			// 기존 사운드가 재생 중이면 끄기
			if (CurrentAudio && CurrentAudio->IsPlaying())
			{
				CurrentAudio->Stop();
			}

			// 새 사운드 재생
			CurrentAudio = UGameplayStatics::SpawnSoundAtLocation(this, SelectedSound, GetActorLocation());
		}
	}

	
	// 기존 타이머 제거
	GetWorldTimerManager().ClearTimer(AlertHideTimerHandle);

	// None이 아니라면 일정 시간 뒤에 아이콘 숨기기
	if (NewType != EAlertType::None)
	{
		GetWorldTimerManager().SetTimer(AlertHideTimerHandle, this, &ABaseAIHunter::HideAlertIcon, 2.5f, false);
	}

}

void ABaseAIHunter::HideAlertIcon()
{
	SetAlertType(EAlertType::None);
}
