// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/AI/NetHunter.h"

#include "HHS/BaseAIController.h"
#include "HHS/Actor/LassoActor.h"
#include "Net/UnrealNetwork.h"

ANetHunter::ANetHunter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackRange = 150.f;
	DefaultAttackType = EAttackType::Net;
}

void ANetHunter::AttackStart()
{
	if (bIsAttacking) return;
	bIsAttacking = true;
	UE_LOG(LogTemp, Warning, TEXT("AttackStart called"));
	
	Super::AttackStart();
	
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
		Multicast_PlayAttackMontage(AttackMontage);
	}
	DisableMovement();
}

void ANetHunter::AttackEnd()
{
	Super::AttackEnd();
	bIsAttacking = false;

	EnableMovement();
}

float ANetHunter::GetAttackRange(EAttackType AttackType) const
{
	return Super::GetAttackRange(AttackType);
}

UAnimMontage* ANetHunter::GetMontage(EAttackType AttackType) const
{
	if (AttackType == EAttackType::Net)
	{
		return NetMontage;
	}
	return nullptr;
}

void ANetHunter::StartLassoSpin()
{
	if (!LassoClass) return;
	if (CurrentLasso)
	{
		CurrentLasso->Destroy();
		CurrentLasso = nullptr;
	}
	
	ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
	if (!AIController) return;

	AActor* Target = AIController->GetCurrentTarget();
	if (!Target) return;

	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Hand_R"));
	FRotator SpawnRotation = GetActorRotation();

	ALassoActor* Lasso = GetWorld()->SpawnActor<ALassoActor>(LassoClass, SpawnLocation, SpawnRotation);
	if (Lasso)
	{
		Lasso->OwnerAI = this;
		CurrentLasso = Lasso;
		Lasso->StartSpinning();

		UE_LOG(LogTemp, Warning, TEXT("StartLassoSpin: Lasso created and spinning"));
	}
}

void ANetHunter::ThrowNet()
{
	if (!LassoClass) return;
	if (CurrentLasso)
	{
		CurrentLasso->Destroy();
		CurrentLasso = nullptr;
	}

	ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
	if (!AIController) return;

	AActor* Target = AIController->GetCurrentTarget();
	if (!Target) return;

	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Hand_R"));
	FRotator SpawnRotation = GetActorRotation();
	
	FVector TargetHeadLocation = Target->GetActorLocation(); // fallback

	// 목표 방향 계산 (디버그용 시각화)
	if (AAnimalBase* AnimalTarget = Cast<AAnimalBase>(Target))
	{
		if (USkeletalMeshComponent* TargetMesh = AnimalTarget->GetMesh())
		{
			if (TargetMesh->DoesSocketExist(FName("Head")))
			{
					TargetHeadLocation = TargetMesh->GetSocketLocation(FName("Head"));
			}
		}
	}

	FVector Direction = (TargetHeadLocation - SpawnLocation).GetSafeNormal();
	SpawnRotation = Direction.Rotation();
	DrawDebugLine(GetWorld(), SpawnLocation, TargetHeadLocation, FColor::Cyan, false, 1.f, 0, 2.f);

	// 올가미 생성
	ALassoActor* Lasso = GetWorld()->SpawnActor<ALassoActor>(LassoClass, SpawnLocation, SpawnRotation);
	if (Lasso)
	{
		Lasso->OwnerAI = this;
		CurrentLasso = Lasso;
		Lasso->ThrowLasso(Target); // Target은 AActor* 이므로 바로 넘김

		// 플레이어에 NetHunter를 등록
		if (AAnimalBase* animal = Cast<AAnimalBase>(Target))
		{
			animal->GrabbedByHunter = this;
		}
		UE_LOG(LogTemp, Warning, TEXT("ThrowLasso: Lasso thrown toward %s"), *Target->GetActorLocation().ToString());
	}
}

void ANetHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldEndPull && NetMontage && GetMesh()->GetAnimInstance()->Montage_IsPlaying(NetMontage))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_SetNextSection(FName("PullLoop"), FName("PullEnd"), NetMontage);

		bShouldEndPull = false; // 리셋
	}
}

void ANetHunter::EndPull()
{
	Server_EndPull();
}

void ANetHunter::Server_EndPull_Implementation()
{
	if (CurrentLasso)
	{
		CurrentLasso->Destroy();
		CurrentLasso = nullptr;
	}
	
	bShouldEndPull = false; // 서버에서만 false로 설정
	Multicast_EndPull();
}

void ANetHunter::Multicast_EndPull_Implementation()
{
	if (NetMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance->Montage_IsPlaying(NetMontage))
		{
			// PullLoop 섹션에서 PullEnd 섹션으로 넘어가게 설정
			AnimInstance->Montage_SetNextSection(FName("Loop"), FName("End"), NetMontage);
			
			// 필요한 경우 PullEnd 재생을 확실하게 트리거
			AnimInstance->Montage_JumpToSection(FName("End"), NetMontage);
			
			UE_LOG(LogTemp, Warning, TEXT("Multicast_EndPull: End 섹션으로 전환"));
		}
	}
}

void ANetHunter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetHunter, bShouldEndPull);
}