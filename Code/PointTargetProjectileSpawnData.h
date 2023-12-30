// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ProjectileSpawnData.h"
#include "PointTargetProjectileSpawnData.generated.h"

/**
 * 
 */
UCLASS()
class TEAM09_GP4_API UPointTargetProjectileSpawnData : public UProjectileSpawnData
{
	GENERATED_BODY()

public:
	virtual void Shoot(UWorld* world, ACharacter* ShootingCharacter, AWeapon* weapon, FVector aimDirection, float AimDistance, float buttonHeldTime) override;

private:
	FVector FirstHitPointUnderTargetPoint(UWorld* world, FVector targetPoint);

	FVector GetMouseAimPosition(UWorld* world, FVector& hitNormal);
};
