// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UProjectileData;
class UProjectileSpawnData;


//ToDo: Let projectiles use Values from projectile data instead of copying them over.
//The current solution is outdated
UCLASS(Blueprintable)
class TEAM09_GP4_API AProjectile : public AActor
{
	GENERATED_BODY()


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

public:

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UProjectileData> ProjectileData;

	UPROPERTY(BlueprintReadWrite)
	float Damage;
	GameDamageType DamageType;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForce;

	UPROPERTY(BlueprintReadWrite)
	float Speed;
	UPROPERTY(BlueprintReadWrite)
	bool LinearChangeSpeedOverTime;
	UPROPERTY(BlueprintReadWrite)
	float LinearSpeedChangeValue;
	UPROPERTY(BlueprintReadWrite)
	bool ExponentialChangeSpeedOverTime;
	UPROPERTY(BlueprintReadWrite)
	float ExponentialSpeedChangeMultiplier;

	bool bUseGravity;
	float GravityForce;

	UPROPERTY(BlueprintReadWrite)
	bool DestroyOnHitDamagable;
	UPROPERTY(BlueprintReadWrite)
	bool DestroyOnHitAnything;
	UPROPERTY(BlueprintReadWrite)
	bool DontUseDestroyTimer;
	UPROPERTY(BlueprintReadWrite)
	float DestroyTimer = 5;

	UPROPERTY(BlueprintReadWrite)
	bool SpawnNewBulletOnDestroy;
	UPROPERTY(BlueprintReadWrite)
	UProjectileSpawnData* NewBulletSpawnData;

	UPROPERTY(BlueprintReadWrite)
	bool bAimOnTarget;
	UPROPERTY(BlueprintReadWrite)
	float TargetLockTurnPower;
	UPROPERTY(BlueprintReadWrite)
	FVector TargetPoint;

	UPROPERTY(BlueprintReadWrite)
	bool ChangeBulletDataAfterTime;

	//This should really be a TPair but I cant get it to work with UPROPERTY
	UPROPERTY(BlueprintReadWrite)
	TMap<UProjectileData*, float> NewBulletDataAfterTime;

	FVector AimDirection;
	float TimeSinceBulletDataApplied;
	
public:	
	AProjectile();

	virtual void ApplyVelocity();

	void ApplyProjectileData(TObjectPtr<UProjectileData> projectileData);
	UFUNCTION(BlueprintCallable)
	void OnHitActor(AActor* hitActor, FVector HitWorldLocation);
	UFUNCTION(BlueprintCallable)
	void DealDamage(AActor* damagableActor, FVector HitWorldLocation);
	UFUNCTION(BlueprintCallable)
	void ApplyKnockback(AActor* knockbackableActor, FVector HitWorldLocation);
	UFUNCTION(BlueprintCallable)
	void DestroyBullet();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyBullet();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHitDamagable(AActor* damagableActor, FVector HitWorldLocation);
	UFUNCTION(BlueprintImplementableEvent)
	void OnHitAnything(AActor* damagableActor, FVector HitWorldLocation);

private:
	void GetBulletDestruction();
	void AimOnTarget(float deltaTime);
	void ApplySpeedChangeOverTime(float deltaTime);
	void ApplyNewBulletDataAfterTime(float timeSinceLastAppliedBulletData);
	void ChangeAimDir(FVector aimDirAdjustment);
	void ApplyGravity(float deltaTime);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
