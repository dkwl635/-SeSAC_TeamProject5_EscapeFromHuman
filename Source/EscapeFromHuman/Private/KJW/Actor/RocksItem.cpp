// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/RocksItem.h"
#include "NiagaraComponent.h"
#include "KJW/Game/EFHGameState.h"
#include "LJW/IPlayerInterAction.h"
#include "LJW/AnimalBase.h"
#include "Components/SphereComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ARocksItem::ARocksItem()
{
	NS_ThrowTail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_ThrowTail"));
	NS_ThrowTail->SetupAttachment(GetRootComponent());
	NS_ThrowTail->SetAutoActivate(false);

	ThrowType = EThrowType::STONE;
}

void ARocksItem::BeginPlay()
{
	Super::BeginPlay();


}

void ARocksItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!HasAuthority()) return;
	if (bHit) return;

	FVector LastPos = GetActorLocation();

	// P = P0 + vt
	FVector NewPos = LastPos + Velocity * DeltaSeconds;
	SetActorLocation(NewPos);

	FRotator NewRotation = Velocity.Rotation(); // 속도 방향의 회전값
	SetActorRotation(NewRotation);
}

void ARocksItem::OnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetOwner ()) 
	{
		AAnimalBase* animal = Cast<AAnimalBase> ( OtherActor );
		if (animal)
		{
			if (HasAuthority ())
			{
				AddItem ( animal );
			}

		}
		return;
	}

	if (GetOwner() == OtherActor) return;
	
	if (DetroyTimer.IsValid ())
	{
		GetWorldTimerManager ().ClearTimer ( DetroyTimer );
	}

	IIPlayerInterAction* IPlayerInterAction = Cast<IIPlayerInterAction>(OtherActor);
	if (IPlayerInterAction)
	{
		if (HasAuthority())
		{
			bHit = true;
			IPlayerInterAction->ThrowItem(ThrowType);
			
			GetWorldTimerManager().SetTimer(DetroyTimer, FTimerDelegate::CreateLambda([&]
				{
					HitDestoryEvent();
				}), DestroyTime, false);

			SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ServerRPC_Showtail(false);
			ServerRPC_ShowItem(false);
		}
			
		
	}


	
}

void ARocksItem::HitDestoryEvent()
{
	//MulticastRPC_Showtail(false);

	if (GS)
		GS->DestroyThrowItem(this);
}


void ARocksItem::ItemUse()
{
	ShowItem(true);
	SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
	Velocity = GetActorForwardVector() * ProjectileSpeed;
	//SetLifeSpan(LifeSpan);
	ServerRPC_Showtail(true);
	//FTimerHandle destroyTimerHandle;
	GetWorldTimerManager ().SetTimer ( DetroyTimer , FTimerDelegate::CreateLambda ( [this] {
		HitDestoryEvent();
		} ) , LifeSpan , false );
}

void ARocksItem::ServerRPC_Showtail_Implementation(bool bShow)
{
	MulticastRPC_Showtail(bShow);
}

void ARocksItem::MulticastRPC_Showtail_Implementation(bool bShow)
{
	if (bShow)
	{

		NS_ThrowTail->Activate(true);
		NS_ThrowTail->SetVisibility(true);
	}
	else
	{
		NS_ThrowTail->SetFloatParameter(TEXT(" LifeTimer"), 0.0f);
		//NS_ThrowTail->Deactivate();
		//NS_ThrowTail->SetVisibility(false);
	}

}

void ARocksItem::ServerRPC_SpawnHitEffect_Implementation()
{
	MulticastRPC_SpawnHitEffect();
}

void ARocksItem::MulticastRPC_SpawnHitEffect_Implementation()
{
	if (NS_HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NS_HitEffect,
			GetActorLocation()
		);
	}
}


