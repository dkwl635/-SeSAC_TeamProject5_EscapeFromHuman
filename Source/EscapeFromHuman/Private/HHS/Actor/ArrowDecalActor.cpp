// Fill out your copyright notice in the Description page of Project Settings.


#include "HHS/Actor/ArrowDecalActor.h"

#include "Components/DecalComponent.h"

// Sets default values
AArrowDecalActor::AArrowDecalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Script/Engine.Material'/Game/HHS/UI/M_Decal_Arrow.M_Decal_Arrow'"));
	if (Mat.Succeeded())
	{
		ArrowDecalMaterial = Mat.Object;
	}
}

// Called when the game starts or when spawned
void AArrowDecalActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority ())
		GetWorldTimerManager ().SetTimer ( DestroyDecalTimerHandle , this , &AArrowDecalActor::DestroyDecal , 1.5f , false );

	
}

// Called every frame
void AArrowDecalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrowDecalActor::SetDecalLength(int32 Count)
{
	if (HasAuthority ())
	{
		Multicast_SetDecalLength ( Count );
;	}
	//// 기존 데칼 제거
	//for (auto* Decal : ArrowDecals)
	//{
	//	if (Decal)
	//		Decal->DestroyComponent();
	//}
	//ArrowDecals.Empty();

	//// 새 데칼 생성
	//for (int32 i = 0; i < Count; ++i)
	//{
	//	UDecalComponent* NewDecal = NewObject<UDecalComponent>(this);
	//	NewDecal->RegisterComponent();
	//	NewDecal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//	NewDecal->SetRelativeLocation(FVector(DecalSpacing * i, 0, 5)); // Z축은 바닥 위 살짝
	//	NewDecal->SetRelativeRotation(FRotator(-90, 0, 0));
	//	NewDecal->DecalSize = DecalSize;
	//	NewDecal->SetDecalMaterial(ArrowDecalMaterial);
	//	NewDecal->SetVisibility(true);

	//	ArrowDecals.Add(NewDecal);
	//}
}

void AArrowDecalActor::SetArrowVisible(bool bVisible)
{
	for (auto* Decal : ArrowDecals)
	{
		if (Decal)
		{
			Decal->SetVisibility(bVisible);
		}
	}
}

void AArrowDecalActor::DestroyDecal()
{
	Destroy();
}

void AArrowDecalActor::Multicast_SetDecalLength_Implementation  ( int32 Count )
{
	// 기존 데칼 제거
	for (auto* Decal : ArrowDecals)
	{
		if (Decal)
			Decal->DestroyComponent ();
	}
	ArrowDecals.Empty ();

	// 새 데칼 생성
	for (int32 i = 0; i < Count; ++i)
	{
		UDecalComponent* NewDecal = NewObject<UDecalComponent> ( this );
		NewDecal->RegisterComponent ();
		NewDecal->AttachToComponent ( RootComponent , FAttachmentTransformRules::KeepRelativeTransform );
		NewDecal->SetRelativeLocation ( FVector ( DecalSpacing * i , 0 , 5 ) ); // Z축은 바닥 위 살짝
		NewDecal->SetRelativeRotation ( FRotator ( -90 , 0 , 0 ) );
		NewDecal->DecalSize = DecalSize;
		NewDecal->SetDecalMaterial ( ArrowDecalMaterial );
		NewDecal->SetVisibility ( true );

		ArrowDecals.Add ( NewDecal );
	}
}

