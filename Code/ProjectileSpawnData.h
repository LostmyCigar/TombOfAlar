// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActorClasses/Projectile.h"
#include "ProjectileSpawnData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootSignature);


class AWeapon;
class UProjectileData;
/**
 * This class is way to big right now. It should probly be built up using components instead
 * We should for example have a constraint class and just let this spawndata check through a list of contraints when trying to fire instead of checking every condition itself
 * 
 * We could ame
 * 
 * At the start of this project i as not aware that you could make UObjects blueprintable and that would have been very usefull
 * In a larger project we would do a full rewrite of this
 */
UCLASS(BlueprintType)
class TEAM09_GP4_API UProjectileSpawnData : public UDataAsset
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> Bullet;

	UPROPERTY(EditAnywhere, Category = "Weapon|Reload")
	float ReloadTime;
	UPROPERTY(EditAnywhere, Category = "Weapon|Reload")
	int MagazineSize;

	UPROPERTY(EditAnywhere, Category = "Weapon|Overheat")
	float OvearheatLimit;
	UPROPERTY(EditAnywhere, Category = "Weapon|Overheat")
	float OvearheatChargePerShot;

	UPROPERTY(EditAnywhere, Category = "Weapon|Overheat")
	float OverheatRechargeWaitTime;

	UPROPERTY(EditAnywhere, Category = "Weapon|Overheat")
	float RechargeRate;
	UPROPERTY(EditAnywhere, Category = "Weapon|Overheat")
	float OverheatedRechargeRate;

	UPROPERTY(EditAnywhere, Category = "Weapon|FireRate")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Weapon|Combos")
	bool OnlyUseComboData;
	UPROPERTY(EditAnywhere, Category = "Weapon|Combos")
	bool ResetComboCounterOnReleaseButton;
	UPROPERTY(EditAnywhere, Category = "Weapon|Combos|SpawnDataAtShootCount")
	TMap<TObjectPtr<UProjectileSpawnData>, int> NewSpawnDataAfterShoots;


	UPROPERTY(EditAnywhere, Category = "Weapon|FireInput")
	bool TryShootOnButtonPressed;
	UPROPERTY(EditAnywhere, Category = "Weapon|FireInput")
	bool TryShootOnButtonHeld;
	UPROPERTY(EditAnywhere, Category = "Weapon|FireInput")
	bool TryShootOnButtonReleased;
	UPROPERTY(EditAnywhere, Category = "Weapon|FireInput")
	float MinHoldTimeToFire;


	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn")
	int ProjectileAmount;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Spawnpoint Offset")
	bool bUseRandomSpawnOffset;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Spawnpoint Offset")
	float BulletSpawnOffsetMin;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Spawnpoint Offset")
	float BulletSpawnOffsetMax;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Angles")
	bool bUseRandomAngles;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Angles")
	TArray<float> ProjectileAnglesForward;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Angles")
	TArray<float> ProjectileAnglesUp;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Angles|Offset")
	bool bUseRandomAngleOffset;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Angles|Offset")
	float BulletAngleOffsetMin;
	UPROPERTY(EditAnywhere, Category = "Weapon|Bullet Spawn|Angles|Offset")
	float BulletAngleOffsetMax;

	UPROPERTY(EditAnywhere, Category = "Bullet Data")
	TObjectPtr<UProjectileData> ProjectileData;

	UPROPERTY(EditAnywhere, Category = "Character|Movement")
	float RecoilForce;


	UPROPERTY(EditAnywhere, Category = "Other|Time Modification")
	float TimeScaleOnFire;
	UPROPERTY(EditAnywhere, Category = "Other|Time Modification")
	float TimeScaleOnFireTime;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug|Ovearheat")
	float OverheatCharge;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug|Ovearheat")
	bool bIsOverheated;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug|Ovearheat")
	float OverheatRechargeWaitTimer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug")
	float FireRateTimer;

	UPROPERTY(EditAnywhere, Category = "Debug|Time Modification")
	bool TimeIsSlowed;
	UPROPERTY(EditAnywhere, Category = "Debug|Time Modification")
	float TimeScaleTimer;

private:
	
	//bool TimeIsSlowed;
	//float TimeScaleTimer;

	int ShootCount;
	bool Reloading;
	float ReloadTimer;
	int Magazine;


	UWorld* cachedWorld;

public:
	UFUNCTION()
	virtual void OnFireButtonPressed(UWorld* world, FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, AWeapon* weapon);
	UFUNCTION()
	virtual void OnFireButtonHeld(UWorld* world, FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, AWeapon* weapon, float primaryHeldTime);
	UFUNCTION()
	virtual void OnFireButtonReleased(UWorld* world, FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, AWeapon* weapon, float releaseTime);

	UFUNCTION()
	virtual void Shoot(UWorld* world, ACharacter* ShootingCharacter, AWeapon* weapon, FVector aimDirection, float AimDistance, float buttonHeldTime);
	UFUNCTION()
	virtual void BulletExplosionSpawn(UWorld* world, AActor* SpawnActor, FVector aimDirection);
	UFUNCTION()
	virtual bool CheckComboShoot(UWorld* world, ACharacter* ShootingCharacter, AWeapon* weapon, FVector aimDirection, float AimDistance, float buttonHeldTime);

	
	UFUNCTION()
	virtual void OnShoot();
	UFUNCTION()
	virtual bool CanShoot(float buttonHeldTime);

protected:
	TArray<AProjectile*> SpawnBullets(UWorld* world, FTransform SpawnPoint, FVector SpawnPointRightVector);
	TArray<AProjectile*> SpawnBullets(UWorld* world, FVector SpawnPoint, FRotator SpawnRotation);
	void SetBulletStats(TArray<AProjectile*> bullets);
	void AimBullets(TArray<AProjectile*> bullets, FVector aimDirection);
	void ApplyRecoil(ACharacter* ShootingCharacter);
	void ApplyTimeSlow(float deltaTime);
	void CheckOverheat(float deltaTime);

public:
	UFUNCTION()
	virtual void StartSpawnData();
	UFUNCTION()
	virtual void UpdateSpawnData(float deltaTime);

#pragma region EinarRegion
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnShootSignature OnShootSignature;
#pragma endregion 
};
