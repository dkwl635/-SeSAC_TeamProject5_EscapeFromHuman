// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HHS/BaseAIHunter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "LassoActor.generated.h"

UCLASS()
class ESCAPEFROMHUMAN_API ALassoActor : public AActor
{
	GENERATED_BODY()
public:	
	ALassoActor();

	/** 소유자 AI */
	UPROPERTY()
	AActor* OwnerAI;

	/** 타겟으로 끌려오는 액터 */
	UPROPERTY()
	AAnimalBase* TargetActor;

	// 상태 제어 함수
	void StartSpinning();
	void ThrowLasso(AActor* Target);
	void StopLasso();

	// 상태 변수 접근용 Getter (읽기 전용)
	bool IsSpinning() const { return bIsSpinning; }
	bool IsThrown() const { return bIsThrown; }
	bool IsPullingTarget() const { return bIsPullingTarget; }

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionSphere;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
								 AActor* OtherActor,
								 UPrimitiveComponent* OtherComp,
								 int32 OtherBodyIndex,
								 bool bFromSweep,
								 const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 컴포넌트
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* StartPoint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndPoint;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere)
	TArray<USplineMeshComponent*> SplineMeshes;

	// 상태
	bool bIsSpinning;
	bool bIsThrown;
	bool bIsPullingTarget;

	float SpinAngle;
	FVector ThrowDirection;
	float CurrentThrowDistance;

	UPROPERTY(EditAnywhere)
	float SpinRadius = 100.f;

	UPROPERTY(EditAnywhere)
	float SpinSpeed = 180.f;

	UPROPERTY(EditAnywhere)
	float ThrowSpeed = 1000.f;

	UPROPERTY(EditAnywhere)
	float PullSpeed = 100.f;

	UPROPERTY(EditAnywhere)
	int32 SplinePointCount = 10;

	UPROPERTY(EditAnywhere)
	UStaticMesh* RopeSegmentMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* RopeMaterial;

	// 내부 함수
	void UpdateSpline();
	void OnLassoHit(AActor* HitActor);

};