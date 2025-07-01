// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/BaseAIController.h"

#include "EscapeFromHuman.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HHS/BaseAIHunter.h"
#include "LJW/AnimalBase.h"
#include "LJW/StatusComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


ABaseAIController::ABaseAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//if (!HasAuthority()) return;
	
	if (ABaseAIHunter* const hunter = Cast<ABaseAIHunter>(InPawn))
	{
		if (UBehaviorTree* const behaviorTree = hunter->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(behaviorTree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(behaviorTree);
		}
	}
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	ABaseAIHunter* AIHunter = Cast<ABaseAIHunter>(GetPawn());
	if (AIHunter)
	{
		TeamHunter = AIHunter;
		TeamId = FGenericTeamId(TeamHunter->ID);
	}
}

ETeamAttitude::Type ABaseAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (OtherPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	}

	auto PlayerTI = Cast<IGenericTeamAgentInterface>(&Other);
	class IGenericTeamAgentInterface* BotTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (BotTI == nullptr && PlayerTI == nullptr)
	{
		return ETeamAttitude::Neutral;
	}

	FGenericTeamId OtherActorTeamId = NULL;
	if (BotTI != nullptr)
	{
		OtherActorTeamId = BotTI->GetGenericTeamId();
	}
	else if (PlayerTI != nullptr)
	{
		OtherActorTeamId = PlayerTI->GetGenericTeamId();
	}

	FGenericTeamId ThisId = GetGenericTeamId();
	if (OtherActorTeamId == 8)				// TeamId가 8이면 중립.
	{
		return ETeamAttitude::Neutral;
	}
	else if (OtherActorTeamId == ThisId)		// TeamId가 같으면 같은 팀.
	{
		return ETeamAttitude::Friendly;
	}
	else										// TeamId가 다르면 적.
	{
		return ETeamAttitude::Hostile;
	}
}

void ABaseAIController::SetupPerceptionSystem()
{
	UAIPerceptionComponent* PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	SetPerceptionComponent(*PerceptionComp);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 500.f;									// 시야 반경
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;		// 시야 잃는 범위
		SightConfig->PeripheralVisionAngleDegrees = 90.f;					// 시야각
		SightConfig->SetMaxAge(5.f);										// 자극이 잊히기 까지의 시간 (0이면 잊지않음)
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;			// 위치 탐지 성공하고 시야 범위를 벗어난 상태에서도 객체를 설정한 거리 이내에서 추적할 수 있음
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;			// 적에 대한 감지 허용
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;		// 아군에 대한 감지 허용
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;		// 중립에 대한 감지 허용

		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 500.f;
		//HearingConfig->LoSHearingRange = 800.f;
		HearingConfig->SetMaxAge(5.f);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;

		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetDetected);
}

void ABaseAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	ServerRPC_OnTargetDetected(Actor, Stimulus);
}

AActor* ABaseAIController::GetCurrentTarget() const
{
	if (!Blackboard) return nullptr;

	UObject* TargetObject = Blackboard->GetValueAsObject("TargetActor");
	return Cast<AActor>(TargetObject);
}


void ABaseAIController::ServerRPC_OnTargetDetected_Implementation(AActor* Actor, FAIStimulus const Stimulus)
{
	if (!Actor || !Actor->ActorHasTag(FName("Player")))
	{
		return;
	}
	DetectedAnimal = Cast<AAnimalBase>(Actor);
	Hunter = Cast<ABaseAIHunter>(GetPawn());
	BB = GetBlackboardComponent();
	if (!DetectedAnimal || !Hunter || !BB) return;

	// 얼음 상태 확인
	bool bIsFrozen = false;
	if (DetectedAnimal->StatusComp)
	{
		bIsFrozen = DetectedAnimal->StatusComp->IsIce;
		BB->SetValueAsBool("TargetIsFrozen", bIsFrozen);
	}

	// 얼음 상태면 물음표 UI 띄우고 감지 처리 중단
	if (bIsFrozen)
	{
		Hunter->SetAlertType(EAlertType::Question);
		BB->SetValueAsVector("TargetLocation", DetectedAnimal->GetActorLocation());

		// 바로 TargetActor 지우지 않고 일정 시간 후 지우기
		UBlackboardComponent* LocalBB = BB;
		FTimerHandle LostTargetTimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(LostTargetTimerHandle, [LocalBB]()
		{
			if (LocalBB)
			{
				LocalBB->ClearValue("TargetActor");
				LocalBB->SetValueAsBool("bShouldRotateToTarget", true);
			}
		}, 2.0f, false); // 2초 후 TargetActor 제거 및 회전 가능하게 설정
		return;
	}

	// 얼음이 아닐 경우 타겟 설정
	BB->SetValueAsObject("TargetActor", DetectedAnimal);

	// Stimulus 처리
	const FAISenseID SightID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	const FAISenseID HearingID = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

	const bool bSensed = Stimulus.WasSuccessfullySensed();
	if (Stimulus.Type == SightID)
	{
		BB->SetValueAsBool("CanSeePlayer", bSensed);
		Hunter->bCanSeePlayer = bSensed;

		if (bSensed)
		{
			BB->SetValueAsObject("TargetActor", Actor);
			Hunter->SetAlertType(EAlertType::Exclamation);
		}
		else
		{
			BB->ClearValue("TargetActor");
			Hunter->SetAlertType(EAlertType::None);
		}
	}
	else if (Stimulus.Type == HearingID)
	{
		BB->SetValueAsBool("HeardNoise", bSensed);
		BB->SetValueAsVector("HeardLocation", Stimulus.StimulusLocation);
		Hunter->bHeardNoise = bSensed;

		if (bSensed)
		{
			BB->SetValueAsObject("TargetActor", Actor);
		}
	}
}

void ABaseAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseAIController, DetectedAnimal);
	DOREPLIFETIME(ABaseAIController, Hunter);
	DOREPLIFETIME(ABaseAIController, BB);

}