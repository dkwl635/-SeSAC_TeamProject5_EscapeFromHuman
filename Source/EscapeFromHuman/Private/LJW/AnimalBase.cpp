// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/AnimalBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "LJW/PlayerInputComponent.h"
#include "KJW/ActorComponent/ItemComponent.h"
#include "Components/ArrowComponent.h"
#include "LJW/StatusComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "EscapeFromHuman.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "KJW/Game/EFHPlayerController.h"
#include "GameFramework/SpectatorPawn.h"
#include "LJW/AnimalDataAsset.h"
#include "LJW/AnimalAnimInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "KJW/Game/EFHPlayerState.h"
#include "Components/MeshComponent.h"
#include "KismetTraceUtils.h"
#include "Components/SphereComponent.h"
#include "LJW/AnimalSkillComponent.h"
#include "Components/BoxComponent.h"
#include "Internationalization/Text.h"
#include "KJW/Game/EFHGameState.h"

AAnimalBase::AAnimalBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent> ( TEXT ("SpringArmComp"));
	SpringArmComp->SetupAttachment ( RootComponent );
	SpringArmComp->SetRelativeLocation ( FVector ( 0 ) );
	SpringArmComp->bUsePawnControlRotation = true;

	CamComp = CreateDefaultSubobject<UCameraComponent> ( TEXT ("CamComp"));
	CamComp->SetupAttachment ( SpringArmComp );
	CamComp->bUsePawnControlRotation = false;
	CamComp->PostProcessBlendWeight = 0.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InputComp = CreateDefaultSubobject<UPlayerInputComponent> ( TEXT ("InputComp"));
	ItemComp = CreateDefaultSubobject<UItemComponent> ( TEXT ("ItemComp"));
	StatusComp = CreateDefaultSubobject<UStatusComponent> ( TEXT ( "StatusComp" ) );
	SkillComp = CreateDefaultSubobject<UAnimalSkillComponent> ( TEXT ( "SkillComp" ));

	ArrowComp = CreateDefaultSubobject<UArrowComponent> ( TEXT ( "ArrowComp" ) );
	ArrowComp->SetupAttachment ( RootComponent );

	ConstructorHelpers::FObjectFinder <UMaterial> Temp_Material( TEXT( "/Script/Engine.Material'/Game/LJW/Blueprint/BasicMaterial.BasicMaterial'" ) );
	if (Temp_Material.Succeeded())
	{
		GetMesh()->SetMaterial (0 , Temp_Material.Object);
	}

	ProjectileNiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "ProjectileNiagaraComp" ) );
	ProjectileNiagaraComp->SetupAttachment ( ArrowComp );
	ProjectileNiagaraComp->SetVisibility ( false );

	RingNiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "RingNiagaraComp" ) );
	RingNiagaraComp->SetupAttachment ( GetRootComponent() );
	RingNiagaraComp->SetVisibility ( false );

	StateNiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "StateNiagaraComp" ) );
	StateNiagaraComp->SetupAttachment ( GetRootComponent () );
	StateNiagaraComp->SetVisibility ( false );

	HasteNiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "HasteNiagaraComp" ) );
	HasteNiagaraComp->SetupAttachment ( GetRootComponent () );
	HasteNiagaraComp->SetVisibility ( false );

	DashNiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ("DashNiagaraComp"));
	DashNiagaraComp->SetupAttachment ( GetRootComponent () );
	DashNiagaraComp->SetVisibility ( false );

	ChickenNiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "ChickenNiagaraComp" ));
	ChickenNiagaraComp->SetupAttachment ( GetRootComponent () );
	ChickenNiagaraComp->SetVisibility ( false );

	SheepNiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ("SheepNiagaraComp"));
	SheepNiagaraComp->SetupAttachment ( GetRootComponent () );
	SheepNiagaraComp->SetVisibility ( false );

	RingComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "RingComp" ) );
	RingComp->SetupAttachment ( GetCapsuleComponent());
	RingComp->SetVisibility ( false );

	SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	SphereComp->SetupAttachment ( RootComponent );
	SphereComp->SetSphereRadius (50.f);
	SphereComp->SetRelativeLocation ( FVector (0.f,0.f,-70.f ) );
	SphereComp->SetCollisionProfileName ( FName ( "sphere" ) );

	dashrange = CreateDefaultSubobject<USphereComponent> ( TEXT ( "dashrange" ) );
	dashrange->SetupAttachment ( RootComponent );
	dashrange->SetSphereRadius (50.f);
	dashrange->SetRelativeLocation ( FVector (0.f,0.f,-70.f ) );
	dashrange->SetCollisionProfileName ( FName ( "dashrange" ) );
	
	SkillRange = CreateDefaultSubobject<USphereComponent> ( TEXT ( "ChickenSkillRange" ) );
	SkillRange->SetupAttachment ( RootComponent );
	SkillRange->SetSphereRadius ( 300.f );
	SkillRange->SetRelativeLocation ( FVector (0.f,0.f,-70.f ) );
	

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "meshcomp" ) );
	meshcomp->SetupAttachment ( RootComponent );
	meshcomp->SetVisibility ( false );
	meshcomp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	meshcomp->SetRelativeLocation ( FVector (0.f,0.f,-70.f));
	
	Scenecomp = CreateDefaultSubobject<USceneComponent> ( TEXT ("Scenecomp"));
	Scenecomp->SetupAttachment ( GetMesh() );

	Tags.Add(FName("Player"));

	bAlwaysRelevant = true;
}

void AAnimalBase::BeginPlay()
{
	Super::BeginPlay();
	dashrange->OnComponentBeginOverlap.AddDynamic (this, &AAnimalBase::OnAnimalBeginOverlap);
	SkillRange->OnComponentBeginOverlap.AddDynamic (this, &AAnimalBase::OnSkillRangeBeginOverlap);
	SkillRange->OnComponentEndOverlap.AddDynamic ( this , &AAnimalBase::OnSkillRangeEndOverlap );
	SphereComp->SetCollisionProfileName ( FName ( "sphere" ) );
	SkillRange->SetCollisionProfileName ( FName ( "sphere" ) );
	StatusComp->bCanSkill = false;
	StatusComp->bCanSkilled = false;
	GS = Cast<AEFHGameState> ( GetWorld ()->GetGameState () );
}

void AAnimalBase::PossessedBy ( AController* NewController )
{
	Super::PossessedBy ( NewController );
	InitControl ();
}

void AAnimalBase::OnRep_Controller ()
{
	Super::OnRep_Controller ();

	InitControl ();
}

void AAnimalBase::InitControl ()
{
	if (APlayerController* pc = Cast<APlayerController> ( GetController() ))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( pc->GetLocalPlayer () ))
			subSys->AddMappingContext ( IMC_Animals , 0 );


		if (HasAuthority ())
		{
			AEFHPlayerState* PS = pc->GetPlayerState<AEFHPlayerState> ();
			if (PS)
			{
				MulticastRPC_SetRingColor ( (PS->GameTeam == EGameTeam::A ? 0 : 1) );
				MulticastRPC_ApplyAnimalData ( (PS->GameTeam == EGameTeam::A ? 0 : 1) );
				MulticastRPC_SetPlayerType ( (PS->GameTeam == EGameTeam::A ? 0 : 1) );
			}

		}

	}
}

void AAnimalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsLocallyControlled()) return;
 
	FVector startPos = CamComp->GetComponentLocation ();
	FVector endPos = GetActorLocation();
	TArray<FHitResult> HitResults;
	FCollisionQueryParams params;
	params.AddIgnoredActor ( this );
	bool bHit = GetWorld ()->LineTraceMultiByChannel ( HitResults , startPos , endPos , ECC_Visibility , params );
	TSet<AActor*> HitActors;

	if (bHit)
	{
	
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != this)
			{
				HitActors.Add(HitActor); // 중복 방지
				SetActorMeshVisibility(HitActor, false);
				TransparentActors.Add(HitActor);
			}
		}
	}

	else
	{
		for (AActor* Prev : TransparentActors)
		{
			SetActorMeshVisibility(Prev, true);
		}
		TransparentActors.Empty();
	}

}

void AAnimalBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto playerInput = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	if (playerInput)
	{
		InputComp->SetupInputBinding ( playerInput );
	}
}

void AAnimalBase::ThrowItem ( EThrowType ThrowType )
{
	switch (ThrowType)
    {
        case EThrowType::STONE:
			HitStone ();
            break;
        case EThrowType::NET:
            break;
        case EThrowType::Trap:
			HitTrap ();
            break;
        default:
            break;
    }
}

void AAnimalBase::BuffItem ( EUSINGType BuffType )
{
	switch (BuffType)
    {
        case EUSINGType::HASTE:
			Haste ();
            break;
        case EUSINGType::INVISIBLE:
			Invisible ();
            break;
        case EUSINGType::CLAIRVOYANCE:
            break;
        case EUSINGType::MAX:
            break;
        default:
            break;
    }
}

void AAnimalBase::MulticastRPC_SetPlayerType_Implementation ( int32 PlayerType )
{
	if (PlayerType == 0)
	{
		type = 0;
		GetCapsuleComponent()->SetCollisionResponseToChannel ( ECC_GameTraceChannel5 , ECR_Block );
	}

	else if(PlayerType == 1)
	{
		type = 1;
		GetCapsuleComponent()->SetCollisionResponseToChannel ( ECC_GameTraceChannel4 , ECR_Block );
	}
}

void AAnimalBase::Haste ()
{
	HasteOn ();
	GetWorld ()->GetTimerManager ().SetTimer ( HasteTimer , this , &AAnimalBase::HasteOff , 3.f, false);
}

void AAnimalBase::HasteOn ()
{
	GetCharacterMovement ()->MaxWalkSpeed += 200.f;
	MulticastRPC_HasteEffectOn ();
	if (AnimalAnim)
	{
		AnimalAnim->SpeedMultiplier = 1.66f;
	}
}

void AAnimalBase::HasteOff ()
{
	GetCharacterMovement ()->MaxWalkSpeed -= 200.f;
	MulticastRPC_HasteEffectOff ();
	if (AnimalAnim)
	{
		AnimalAnim->SpeedMultiplier = 1.f;
	}
}

void AAnimalBase::Invisible ()
{
	ServerRPC_Invisible ();
}

void AAnimalBase::InvisibleOn ()
{
	if (IsLocallyControlled ())
	{
		UMaterial* Mat = LoadObject<UMaterial>( nullptr , TEXT( "/Script/Engine.Material'/Game/3D_LOW_POLY_FarmPack/Materials/M_FarmAnimals_Invisible.M_FarmAnimals_Invisible'" ) );
		GetMesh ()->SetMaterial ( 0,Mat );
	}
	else
	{
		meshcomp->SetVisibility ( false );
		GetMesh ()->SetVisibility ( false );
		DashNiagaraComp->SetVisibility ( false );
	}
	OnInvisible.Broadcast (true);
	StatusComp->IsInvisible = true;
}

void AAnimalBase::InvisibleOff ()
{
	if (IsLocallyControlled ())
	{
		UMaterial* Mat = LoadObject<UMaterial>( nullptr , TEXT( "/Script/Engine.Material'/Game/3D_LOW_POLY_FarmPack/Materials/M_FarmAnimals.M_FarmAnimals'" ) );
		GetMesh ()->SetMaterial ( 0,Mat );
	}
	else
	{
		GetMesh ()->SetVisibility ( true );
		if (StatusComp->IsIce)
		{
			meshcomp->SetVisibility ( true );
		}
	}
	OnInvisible.Broadcast (false);
	StatusComp->IsInvisible = false;
}

void AAnimalBase::ServerRPC_Invisible_Implementation ()
{
	MulticastRPC_Invisible ();
}

void AAnimalBase::MulticastRPC_Invisible_Implementation ()
{
	InvisibleOn ();
	GetWorld ()->GetTimerManager ().SetTimer ( InvisibleTimer , this , &AAnimalBase::InvisibleOff , 3.f, false);
}

void AAnimalBase::HitStone ()
{
	ServerRPC_HitStone ();
}

void AAnimalBase::ServerRPC_HitStone_Implementation ()
{
	MulticastRPC_HitStone ();
}

void AAnimalBase::MulticastRPC_HitStone_Implementation ()
{
	if (StatusComp->IsIce)
	{
		StatusComp->IsIce = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		meshcomp->SetVisibility ( false );
		GetWorld ()->GetTimerManager ().ClearTimer ( InputComp->IceTimer );
	}
	else
	{
		ApplyDamage ( 1 );
		TrapOn ();
		OnHitEvent.Broadcast ();
		GetWorld ()->GetTimerManager ().SetTimer ( HitTrapTimer , this , &AAnimalBase::TrapOff , 2.f, false);
		PlayAnimMontage (hitmontage);
		OnHitEvent.Broadcast ();
		OnStunEvent.Broadcast (1);
	}
}
//=============================================================================================
void AAnimalBase::HitGun ()
{
	ServerRPC_HitGun ();
}

void AAnimalBase::GunSlowOn ()
{
	GetCharacterMovement ()->MaxWalkSpeed -= 200.f;
}

void AAnimalBase::GunSlowOff ()
{
	GetCharacterMovement ()->MaxWalkSpeed += 200.f;
	StatusComp->bHitByGun = false;
}


void AAnimalBase::ApplyDamage ( int damage )
{
	if (!StatusComp->GameStart) return;
	StatusComp->hp -= damage;

	if (StatusComp->hp <= 0)
	{
		Die ();
	}
}

void AAnimalBase::DashAttack (AAnimalBase* player1, AAnimalBase* player2)
{
	ServerRPC_DashAttack(player1, player2);
}

void AAnimalBase::ServerRPC_HitGun_Implementation ()
{
	MulticastRPC_HitGun ();
}

void AAnimalBase::MulticastRPC_HitGun_Implementation ()
{
	if (StatusComp->bHitByGun)
	{
		if (StatusComp->IsIce)
		{
			StatusComp->IsIce = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			meshcomp->SetVisibility ( false );
		}
		else
		{
			Die ();
		}
	}
	else
	{
		if (StatusComp->IsIce)
		{
			StatusComp->IsIce = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			meshcomp->SetVisibility ( false );
		}
		else
		{
			StatusComp->bHitByGun = true;
			GunSlowOn ();
			GetWorld ()->GetTimerManager ().SetTimer ( HitGunTimer , this , &AAnimalBase::GunSlowOff , 5.f , false );
		}
	}
}

void AAnimalBase::ServerRPC_DashAttack_Implementation (AAnimalBase* player1, AAnimalBase* player2)
{
	MultiCast_DashAttack(player1, player2);
}

void AAnimalBase::MultiCast_DashAttack_Implementation (AAnimalBase* player1, AAnimalBase* player2)
{
	if (player2)
	{
		PRINTLOG ( TEXT ("test1"));
		if (player2->StatusComp->IsDash)
		{
			PRINTLOG ( TEXT ("test2"));
			if (player1->StatusComp->IsIce)
			{
				PRINTLOG ( TEXT ("test3"));
				player1->StatusComp->IsIce = false;
				player1->GetCharacterMovement ()->bOrientRotationToMovement = true;
				player1->meshcomp->SetVisibility ( false );
			}
			else
			{
				PRINTLOG ( TEXT ("test4"));
				player1->TrapOn ();
				player1->OnHitEvent.Broadcast ();
				player1->GetWorld ()->GetTimerManager ().SetTimer ( HitTrapTimer , this , &AAnimalBase::TrapOff , 2.f, false);
				player1->PlayAnimMontage (hitmontage);
				player1->OnHitEvent.Broadcast ();
				player1->OnStunEvent.Broadcast (3);
				player1->ApplyDamage ( 1 );
			}
		}
	}
}

void AAnimalBase::MulticastRPC_HasteEffectOn_Implementation ()
{
	PRINTLOG ( TEXT ( "헤이스트 시작" ) );
	HasteNiagaraComp->SetVisibility ( true );
	HasteNiagaraComp->Activate ( true );
}

void AAnimalBase::MulticastRPC_HasteEffectOff_Implementation ()
{
	HasteNiagaraComp->SetVisibility ( false );
	HasteNiagaraComp->Activate ( false );
}

//=============================================================================================

// 트랩 처리
void AAnimalBase::HitTrap ()
{
	ServerRPC_HitTrap ();
}

// 트랩 상태
void AAnimalBase::TrapOn ()
{
	StatusComp->IsTrapped = true;
	StateNiagaraComp->SetVisibility ( true );
	StateNiagaraComp->Activate ( true );
	APlayerController* pc = Cast<APlayerController> ( GetController () );
	DisableInput (pc);
	UMaterial* Mat = LoadObject<UMaterial>( nullptr , TEXT( "/Script/Engine.Material'/Game/3D_LOW_POLY_FarmPack/Materials/M_FarmAnimals_Hit.M_FarmAnimals_Hit'" ) );
	GetMesh ()->SetMaterial ( 0,Mat );
}

// 트랩 상태 해제
void AAnimalBase::TrapOff ()
{
	StatusComp->IsTrapped = false;
	StateNiagaraComp->SetVisibility ( false );
	StateNiagaraComp->Activate ( false );
	APlayerController* pc = Cast<APlayerController> ( GetController () );
	EnableInput (pc);
	UMaterial* Mat = LoadObject<UMaterial>( nullptr , TEXT( "/Script/Engine.Material'/Game/3D_LOW_POLY_FarmPack/Materials/M_FarmAnimals.M_FarmAnimals'" ) );
	GetMesh ()->SetMaterial ( 0,Mat );
}

// 트랩 멀티캐스트 호출
void AAnimalBase::ServerRPC_HitTrap_Implementation ()
{
	MulticastRPC_HitTrap ();
}

// 트랩 구현 => 발동 후 1.5초 뒤 트랩 상태 해제
void AAnimalBase::MulticastRPC_HitTrap_Implementation ()
{
	TrapOn ();
	OnHitEvent.Broadcast ();
	OnStunEvent.Broadcast (2);
	GetWorld ()->GetTimerManager ().SetTimer ( HitTrapTimer , this , &AAnimalBase::TrapOff , 2.f, false);
}

void AAnimalBase::Die ()
{
	StatusComp->IsDead = true;
	DeadEvent ( this );
	// 관전자 모드로 전환
	if (HasAuthority())
	{
		PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->UnPossess();

			FActorSpawnParameters Params;
			//Params.Owner = PC;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector SpawnLoc = GetActorLocation();
			ASpectatorPawn* Spectator = GetWorld()->SpawnActor<ASpectatorPawn>(GetWorld()->GetAuthGameMode()->SpectatorClass, GetActorLocation(), GetActorRotation(), Params);
			
			if (Spectator)
			{
				PC->Possess(Spectator);
				Spectator->SetActorEnableCollision ( ECollisionEnabled::NoCollision );
			}
		}
	}

	// 사망처리(충돌체, Input 제거, visible false 처리)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetVisibility(true);
	StatusComp->IsIce = true;

	OnHitEvent.Broadcast ();
	// 5초 후 리스폰
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AAnimalBase::Respawn, 5.f, false);
}

void AAnimalBase::Respawn ()
{
	if (HasAuthority())
	{
		if (!PC) PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			// 관전자 제거
			if (ASpectatorPawn* Spectator = Cast<ASpectatorPawn>(PC->GetPawn()))
			{
				PRINTLOG ( TEXT ( "Spectator " ) );
				PC->UnPossess();
				Spectator->Destroy();
			}

			// 다시 플레이어 캐릭터 Possess
			PRINTLOG ( TEXT ( "Possess" ) );
			PC->Possess(this);
		}
	}

	AEFHPlayerController* pc = Cast<AEFHPlayerController> ( PC );
	if (pc)
	{
		// 진영에 따른 리스폰 위치 설정
		ASite = GS->SpawnPoint[0]->GetActorLocation ();
		BSite = GS->SpawnPoint[1]->GetActorLocation ();

		SetActorLocation ( pc->PlayerIndex == 0 ? ASite : BSite );
	}



	// 충돌체 설정, Input, visible 상태 복구
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->SetVisibility(true);
	StatusComp->IsIce = false;
	StatusComp->IsDead = false;
	StatusComp->DashStamina = StatusComp->MaxDashStamina;
	StatusComp->IceStamina = StatusComp->MaxIceStamina;
	InvisibleOff ();
	meshcomp->SetVisibility ( false );
	StatusComp->hp = StatusComp->maxhp;
}

void AAnimalBase::MulticastRPC_ApplyAnimalData_Implementation ( int32 playertype )
{
	if (AnimalData)
	{
		AnimalData->LoadSkillCool ();
		// 스켈레탈 메쉬 설정
		if (playertype == 0)
		{
			if (AnimalData->AnimalStats.AnimalMesh1)
			{
				GetMesh ()->SetSkeletalMesh ( AnimalData->AnimalStats.AnimalMesh1 );
				GetMesh ()->SetRelativeLocation ( FVector ( 0.f , 0.f , -90.f ) );
				GetMesh ()->SetRelativeRotation ( FRotator ( 0.f , -90.f , 0.f ) );
				GetMesh ()->SetRelativeScale3D ( AnimalData->AnimalStats.MeshScale );
			}
		}

		else if (playertype == 1)
		{
			if (AnimalData->AnimalStats.AnimalMesh2)
			{
				GetMesh ()->SetSkeletalMesh ( AnimalData->AnimalStats.AnimalMesh2 );
				GetMesh ()->SetRelativeLocation ( FVector ( 0.f , 0.f , -90.f ) );
				GetMesh ()->SetRelativeRotation ( FRotator ( 0.f , -90.f , 0.f ) );
				GetMesh ()->SetRelativeScale3D ( AnimalData->AnimalStats.MeshScale );
			}
		}

		if (AnimalData->AnimalStats.AnimalSkillCoolTime)
			StatusComp->ShoutingCoolDown = AnimalData->AnimalStats.AnimalSkillCoolTime;

		if (AnimalData->AnimalStats.AnimalWalkSpeed)
			GetCharacterMovement ()->MaxWalkSpeed = AnimalData->AnimalStats.AnimalWalkSpeed;

		// 애니메이션 블루프린트 설정
		if (AnimalData->AnimalStats.AnimBlueprint)
		{
			GetMesh ()->SetAnimInstanceClass ( AnimalData->AnimalStats.AnimBlueprint );
			AnimalAnim = Cast<UAnimalAnimInstance> ( GetMesh ()->GetAnimInstance () );
		}

		if (AnimalData->AnimalStats.HitMontage)
			hitmontage = AnimalData->AnimalStats.HitMontage;

		if (AnimalData->AnimalStats.AttackMontage)
			attackmontage = AnimalData->AnimalStats.AttackMontage;

		if (AnimalData->AnimalStats.animaltype)
			animaltype = AnimalData->AnimalStats.animaltype;

		if (AnimalData->AnimalStats.animaltype)
			icesize = AnimalData->AnimalStats.IceScale;

		if (AnimalData->AnimalStats.IceMesh)
		{
			meshcomp->SetStaticMesh ( AnimalData->AnimalStats.IceMesh );
			meshcomp->SetRelativeLocation ( FVector (0.f,0.f,-60.f));
			meshcomp->SetRelativeScale3D ( icesize );
		}

		if (AnimalData->AnimalStats.Skillicon)
		{
			skillicon = AnimalData->AnimalStats.Skillicon;
		}
	}
}
void AAnimalBase::MulticastRPC_SetRingColor_Implementation ( int32 NewColor )
{
	FLinearColor ringColor;
	if (NewColor == 0)
	{
		ringColor = FColor::Purple;
	}
	else if (NewColor == 1)
	{
		ringColor = FColor::Red;
	}


	int32 materialCount = RingComp->GetNumMaterials ();
	for (int32 i = 0; i < materialCount; ++i)
	{
		// 기존 머티리얼을 가져옴
		UMaterialInterface* Material = RingComp->GetMaterial ( i );

		UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic> ( Material );
		if (!DynMaterial)
		{
			// 새 동적 인스턴스를 생성
			DynMaterial = UMaterialInstanceDynamic::Create ( Material , this );
			// 해당 슬롯에 새 인스턴스 할당
			RingComp->SetMaterial ( i , DynMaterial );
		}

		DynMaterial->SetVectorParameterValue ( FName ( "Color" ) , ringColor );
	}
	
	RingComp->SetVisibility ( true );
}

//✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅
void AAnimalBase::HandleDashStart()
{
	if (!bIsGrabbedByHunter || !bCanTryEscapeLasso || bShouldEndPull)
	{
		return; // 탈출 불가한 상태
	}

	LassoEscapePressCount++;

	UE_LOG(LogTemp, Warning, TEXT("SpaceBar Pressed! Count: %d, bShouldEndPull: %s"),
		LassoEscapePressCount,
		bShouldEndPull ? TEXT("true") : TEXT("false"));

	if (LassoEscapePressCount >= MaxSpaceBarPress)
	{
		bShouldEndPull = true;

		UE_LOG(LogTemp, Warning, TEXT("Escape attempt succeeded! bShouldEndPull set to true."));

		EscapeLasso();  // 탈출 처리
	}
}

void AAnimalBase::HitLasso()
{
	UE_LOG(LogTemp, Warning, TEXT("HitLasso() called"));

	ServerRPC_HitLasso();
}

void AAnimalBase::ServerRPC_HitLasso_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerRPC_HitLasso_Implementation() called"));

	MulticastRPC_HitLasso();
}

void AAnimalBase::MulticastRPC_HitLasso_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Lasso caught! Disabling movement."));
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetCharacterMovement() returned nullptr!"));
	}

	// 올가미 상태 진입
	bCaughtByLasso = true;
	bCanTryEscapeLasso = true;
	bIsGrabbedByHunter = true;
	LassoEscapePressCount = 0;

	// 이펙트 또는 애니메이션 재생도 여기서 가능
	PlayAnimMontage(LassoCaughtMontage);
}

void AAnimalBase::EscapeLasso()
{
	ServerRPC_EscapeLasso();
}

void AAnimalBase::ServerRPC_EscapeLasso_Implementation()
{
	MulticastRPC_EscapeLasso();
}

void AAnimalBase::MulticastRPC_EscapeLasso_Implementation()
{
	bCaughtByLasso = false;
	bCanTryEscapeLasso = false;
	
	LassoEscapePressCount = 0;

	// 이동 다시 활성화
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 애니메이션 종료
	StopAnimMontage(LassoCaughtMontage);

	// 끌려오는 중이었다면 종료 신호
	bShouldEndPull = true;
	
}

//✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅✅

void AAnimalBase::SetActorMeshVisibility ( AActor* TargetActor , bool bVisible )
{
	if (!TargetActor) return;

	TArray<UMeshComponent*> MeshComponents;
	TargetActor->GetComponents<UMeshComponent>(MeshComponents);

	for (UMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetVisibility(bVisible, true);
		}
	}
}

void AAnimalBase::OnAnimalBeginOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if (!HasAuthority ()) return;
	auto player = Cast<AAnimalBase> ( OtherActor );
	if (player)
	{
		DashAttack ( this , player );
	}
}

void AAnimalBase::OnSkillRangeBeginOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	CounterPlayer = Cast<AAnimalBase> ( OtherActor );
	if (CounterPlayer)
	{
		StatusComp->bCanSkill = true;
		CounterPlayer->StatusComp->bCanSkilled = true;
	}
}

void AAnimalBase::OnSkillRangeEndOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex )
{
	CounterPlayer = Cast<AAnimalBase> ( OtherActor );
	if (CounterPlayer)
	{
		StatusComp->bCanSkill = false;
		CounterPlayer->StatusComp->bCanSkilled = false;
	}
}

void AAnimalBase::DeadEvent (AAnimalBase* player)
{
	ServerRPC_DeadEvent (player);
}

void AAnimalBase::ServerRPC_DeadEvent_Implementation (AAnimalBase* player)
{
	MulticastRPC_DeadEvent(player);
}

void AAnimalBase::MulticastRPC_DeadEvent_Implementation (AAnimalBase* player)
{
	OnStunEvent.Broadcast ( 4 );
	player->meshcomp->SetVisibility ( false );
	player->HasteNiagaraComp->SetVisibility ( false );
	player->HasteNiagaraComp->Activate ( false );
	player->StateNiagaraComp->SetVisibility ( false );
	player->StateNiagaraComp->Activate ( false );
}
