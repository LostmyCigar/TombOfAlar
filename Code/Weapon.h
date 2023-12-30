// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/ProjectileSpawnData.h"
#include "Weapon.generated.h"

class UArrowComponent;
class UStaticMeshComponent;


//Should rewrite

UCLASS(Blueprintable)
class TEAM09_GP4_API AWeapon : public AActor	
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> ShootPoint;
	
public:	
	AWeapon();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProjectileSpawner | Primary")
	TArray<TObjectPtr<UProjectileSpawnData>> PrimarySpawnData;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProjectileSpawner | Secondary")
	TArray<TObjectPtr<UProjectileSpawnData>> SecondarySpawnData;


public:
	void BeginPlay() override;
	void Tick(float deltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void OnPrimaryPressed(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter);
	UFUNCTION(BlueprintCallable)
	void OnPrimaryHeld(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float holdTime);
	UFUNCTION(BlueprintCallable)
	void OnPrimaryReleased(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float releaseTime);

	UFUNCTION(BlueprintCallable)
	void OnSecondaryPressed(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter);
	UFUNCTION(BlueprintCallable)
	void OnSecondaryHeld(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float holdTime);
	UFUNCTION(BlueprintCallable)
	void OnSecondaryReleased(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float releaseTime);

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnPrimaryPressedBP(FVector AimDir, ACharacter* ShootingCharacter);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnPrimaryHeldBP(FVector AimDir, ACharacter* ShootingCharacter, float holdTime);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnPrimaryReleasedBP(FVector AimDir, ACharacter* ShootingCharacter, float releaseTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnSecondaryPressedBP(FVector AimDir, ACharacter* ShootingCharacter);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnSecondaryHeldBP(FVector AimDir, ACharacter* ShootingCharacter, float holdTime);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnSecondaryReleasedBP(FVector AimDir, ACharacter* ShootingCharacter, float releaseTime);


	UFUNCTION(BlueprintImplementableEvent)
	void OnBulletSpawn(AProjectile* bullet);
	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReload();

private:
	void StartSpawnData();
	void UpdateSpawnData(float deltaTime);
};
