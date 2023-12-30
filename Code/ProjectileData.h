// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssets/PointTargetProjectileSpawnData.h"
#include "Interfaces/Damageable.h"
#include "ProjectileData.generated.h"

/**
 * 
 */
UCLASS()
class TEAM09_GP4_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()
	
public:


	UPROPERTY(EditAnywhere, Category = "Bullet|Damage")
	float Damage;
	UPROPERTY(EditAnywhere, Category = "Bullet|Damage")
	GameDamageType DamageType;

	UPROPERTY(EditAnywhere, Category = "Bullet|Movement|Speed")
	float Speed;
	UPROPERTY(EditAnywhere, Category = "Bullet|Movement|Speed|LinearSpeedChange")
	bool LinearChangeSpeedOverTime;
	UPROPERTY(EditAnywhere, Category = "Bullet|Movement|Speed|LinearSpeedChange")
	float LinearSpeedChangeValue;
	UPROPERTY(EditAnywhere, Category = "Bullet|Movement|Speed|ExponentialSpeedChange")
	bool ExponentialChangeSpeedOverTime;
	UPROPERTY(EditAnywhere, Category = "Bullet|Movement|Speed|ExponentialSpeedChange")
	float ExponentialSpeedChangeMultiplier;

	UPROPERTY(EditAnywhere, Category = "Bullet|Movement|Gravity")
	bool bUseGravity;
	UPROPERTY(EditAnywhere, Category = "Bullet|Movement|Gravity")
	float GravityForce;


	UPROPERTY(EditAnywhere, Category = "Bullet|Knockback")
	float KnockbackForce;


	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction")
	bool DontUseDestroyTimer;
	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction")
	float DestroyTimer;

	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction|Time Offset")
	bool bRandomDestroyTimeOffset;
	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction|Time Offset")
	float DestroyTimeOffsetMin;
	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction|Time Offset")
	float DestroyTimeOffsetMax;


	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction|On Hit")
	bool DestroyOnHitDamagable;
	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction|On Hit")
	bool DestroyOnHitAnything;

	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction|New Bullet")
	bool bSpawnNewBulletOnDestroy;
	//DO NOT USE A SPAWNDATA THAT SPAWNS AN IDENTICAL BULLET HERE. Will cause the game to crash.
	UPROPERTY(EditAnywhere, Category = "Bullet|Destruction|New Bullet")
	UProjectileSpawnData* NewBulletOnDestroySpawnData;

	UPROPERTY(EditAnywhere, Category = "Bullet|Targeting")
	bool bAimOnTarget;
	UPROPERTY(EditAnywhere, Category = "Bullet|Targeting")
	float TargetLockTurnPower;

	UPROPERTY(EditAnywhere, Category = "Bullet|Data|On Apply")
	bool ChangeAimDirWhenApplied;
	UPROPERTY(EditAnywhere, Category = "Bullet|Data|On Apply")
	FVector AimDirChange;
	UPROPERTY(EditAnywhere, Category = "Bullet|Data")
	bool ChangeBulletDataAfterTime;
	UPROPERTY(EditAnywhere, Category = "Bullet|Data")
	TMap<UProjectileData*, float> NewBulletDataAfterTime;


	UPROPERTY(EditAnywhere, Category = "Bullet|Time Modification")
	float TimeScaleOnHit;
	UPROPERTY(EditAnywhere, Category = "Bullet|Time Modification")
	float TimeScaleOnHitTime;
};
