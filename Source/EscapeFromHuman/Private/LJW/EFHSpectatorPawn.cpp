// Fill out your copyright notice in the Description page of Project Settings.


#include "LJW/EFHSpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"



AEFHSpectatorPawn::AEFHSpectatorPawn ()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent> ( TEXT ("SpringArmComp"));
	SpringArmComp->SetupAttachment ( RootComponent );
	SpringArmComp->SetRelativeLocation ( FVector ( 0 ) );
	SpringArmComp->bUsePawnControlRotation = true;

	CamComp = CreateDefaultSubobject<UCameraComponent> ( TEXT ("CamComp"));
	CamComp->SetupAttachment ( SpringArmComp );
	CamComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

}

void AEFHSpectatorPawn::BeginPlay ()
{
	Super::BeginPlay();

	CamComp->PostProcessSettings.ColorSaturation = FVector4 ( 0 , 0 , 0 , 1 );
}
