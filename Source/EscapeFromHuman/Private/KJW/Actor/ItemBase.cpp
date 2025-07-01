// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/ItemBase.h"

AItemBase::AItemBase ()
{
	bReplicates = true;
	SetReplicateMovement ( true );
}

void AItemBase::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

}

void AItemBase::SetCollision ( bool bShow )
{	
	ServerRPC_SetCollision ( bShow );
}

void AItemBase::ShowItem ( bool bShow )
{
	ServerRPC_ShowItem ( bShow );
}

void AItemBase::DropItem ( FVector  DropPos )
{
	bDrop = true;
	SetCollision ( true );
	ShowItem ( true );

	this->SetActorLocation ( DropPos );
}


#pragma region  NetWork
void AItemBase::ServerRPC_SetCollision_Implementation ( bool bShow )
{
	MulticastRPC_Shouting ( bShow );
}

void AItemBase::MulticastRPC_Shouting_Implementation ( bool bShow )
{
	SetCollision_Internal ( bShow );
}

void AItemBase::ServerRPC_ShowItem_Implementation ( bool bShow )
{
	MulticastRPC_ShowItem ( bShow );
}

void AItemBase::MulticastRPC_ShowItem_Implementation ( bool bShow )
{
	ShowItem_Internal ( bShow );
}

#pragma endregion

