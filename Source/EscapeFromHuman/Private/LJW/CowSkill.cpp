// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/CowSkill.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACowSkill::ACowSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>( TEXT( "boxcomp" ) );
	boxcomp->SetupAttachment( RootComponent );
	boxcomp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics);

	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "meshcomp" ) );
	meshcomp->SetupAttachment( boxcomp );
	meshcomp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void ACowSkill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACowSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

