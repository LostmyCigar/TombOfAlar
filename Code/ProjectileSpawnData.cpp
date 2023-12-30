// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/ProjectileSpawnData.h"
#include "ActorClasses/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DataAssets/ProjectileData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetStringLibrary.h"


void UProjectileSpawnData::OnFireButtonPressed(UWorld* world, FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, AWeapon* weapon)
{
	if (!TryShootOnButtonPressed)
		return;

	Shoot(world, ShootingCharacter, weapon, AimDir, AimDistance, 0.0f);
}

void UProjectileSpawnData::OnFireButtonHeld(UWorld* world, FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, AWeapon* weapon, float primaryHeldTime)
{
	if (!TryShootOnButtonHeld)
		return;

	Shoot(world, ShootingCharacter, weapon, AimDir, AimDistance, primaryHeldTime);
}

void UProjectileSpawnData::OnFireButtonReleased(UWorld* world, FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, AWeapon* weapon, float releaseTime)
{
	if (!TryShootOnButtonReleased) {
		ShootCount = ResetComboCounterOnReleaseButton ? 0 : ShootCount;
		return;
	}

	Shoot(world, ShootingCharacter, weapon, AimDir, AimDistance, releaseTime);

	ShootCount = ResetComboCounterOnReleaseButton ? 0 : ShootCount;
}



void UProjectileSpawnData::Shoot(UWorld* world, ACharacter* ShootingCharacter, AWeapon* weapon, FVector aimDirection, float AimDistance, float buttonHeldTime)
{
	cachedWorld = world;
	if (!CanShoot(buttonHeldTime))
		return;

	auto comboShoot = CheckComboShoot(world, ShootingCharacter, weapon, aimDirection, AimDistance, buttonHeldTime);

	if (comboShoot && OnlyUseComboData) {
		OnShoot();
		weapon->OnShoot();
		return;
	}

	auto bullets = SpawnBullets(world, weapon->ShootPoint->GetComponentTransform(), weapon->ShootPoint->GetRightVector());
	SetBulletStats(bullets);
	AimBullets(bullets, aimDirection);

	for (int i = 0; i < bullets.Num(); i++)
	{
		bullets[i]->ApplyVelocity();
	}

	if (ShootingCharacter)
		ApplyRecoil(ShootingCharacter);

	OnShoot();
	weapon->OnShoot();
}

void UProjectileSpawnData::BulletExplosionSpawn(UWorld* world, AActor* SpawnActor, FVector aimDirection)
{
	auto bullets = SpawnBullets(world, SpawnActor->GetActorTransform(), SpawnActor->GetActorRightVector());
	SetBulletStats(bullets);
	AimBullets(bullets, aimDirection);

	for (int i = 0; i < bullets.Num(); i++)
	{
		bullets[i]->ApplyVelocity();
	}
}

bool UProjectileSpawnData::CheckComboShoot(UWorld* world, ACharacter* ShootingCharacter, AWeapon* weapon, FVector aimDirection, float AimDistance, float buttonHeldTime)
{
	bool shootAnyComboData = false;
	for (auto& Pair : NewSpawnDataAfterShoots)
	{
		if ((ShootCount+1) % Pair.Value == 0) {
			Pair.Key->Shoot(world, ShootingCharacter, weapon, aimDirection, AimDistance, buttonHeldTime);
			shootAnyComboData = true;
		}
	}

	return shootAnyComboData;
}


#pragma region ShootLogic

TArray<AProjectile*> UProjectileSpawnData::SpawnBullets(UWorld* world, FTransform SpawnPoint, FVector SpawnPointRightVector)
{
	TArray<AProjectile*> bulletArray = TArray<AProjectile*>();

	if (!Bullet)
		return bulletArray;

	FTransform SpawnLocation = SpawnPoint;

	for (int i = 0; i < ProjectileAmount; i++)
	{
		if (bUseRandomSpawnOffset) {

			float offSet = FMath::FRandRange(BulletSpawnOffsetMin, BulletSpawnOffsetMax);
			FVector NewLocation = SpawnLocation.GetLocation() + (SpawnPointRightVector * offSet);
			SpawnLocation.SetLocation(NewLocation);
		}

		auto bullet = world->SpawnActor<AProjectile>(Bullet, SpawnLocation);
		bulletArray.Add(bullet);
	}

	return bulletArray;
}

TArray<AProjectile*> UProjectileSpawnData::SpawnBullets(UWorld* world, FVector SpawnPoint, FRotator SpawnRotation)
{
	TArray<AProjectile*> bulletArray = TArray<AProjectile*>();

	if (!Bullet)
		return bulletArray;

	for (int i = 0; i < ProjectileAmount; i++)
	{
		auto bullet = world->SpawnActor<AProjectile>(Bullet, SpawnPoint, SpawnRotation);
		bulletArray.Add(bullet);
	}

	return bulletArray;
}

void UProjectileSpawnData::SetBulletStats(TArray<AProjectile*> bullets)
{
	for (int i = 0; i < bullets.Num(); i++)
	{
		bullets[i]->ApplyProjectileData(ProjectileData);
	}
}


//This is kinda badly done since we are copypasting a bunch of code
//But as long as its readable amirite
void UProjectileSpawnData::AimBullets(TArray<AProjectile*> bullets, FVector aimDirection)
{
	aimDirection.Normalize();

	if (bUseRandomAngles) {

		for (int i = 0; i < bullets.Num(); i++)
		{
			auto CurrentBulletAimDir = aimDirection;
			if (!ProjectileAnglesForward.IsEmpty()) {
				int rand = FMath::RandRange(0, ProjectileAnglesForward.Num() - 1);
				CurrentBulletAimDir = UKismetMathLibrary::RotateAngleAxis(CurrentBulletAimDir, ProjectileAnglesForward[rand], FVector::UpVector);
			}

			if (!ProjectileAnglesUp.IsEmpty()) {
				int rand = FMath::RandRange(0, ProjectileAnglesUp.Num() - 1);
				auto rightVector = UKismetMathLibrary::RotateAngleAxis(aimDirection, 90, FVector::UpVector);
				CurrentBulletAimDir = UKismetMathLibrary::RotateAngleAxis(CurrentBulletAimDir, ProjectileAnglesUp[rand], rightVector);
			}

			if (bullets[i])
				bullets[i]->AimDirection = CurrentBulletAimDir;
		}

		return;
	}

	for (int i = 0; i < bullets.Num(); i++)
	{
		auto CurrentBulletAimDir = aimDirection;
		if (ProjectileAnglesForward.IsValidIndex(i)) {
			CurrentBulletAimDir = UKismetMathLibrary::RotateAngleAxis(CurrentBulletAimDir, ProjectileAnglesForward[i], FVector::UpVector);
		}

		if (ProjectileAnglesUp.IsValidIndex(i)) {
			auto rightVector = UKismetMathLibrary::RotateAngleAxis(aimDirection, 90, FVector::UpVector);
			CurrentBulletAimDir = UKismetMathLibrary::RotateAngleAxis(CurrentBulletAimDir, ProjectileAnglesUp[i], rightVector);
		}

		if (bUseRandomAngleOffset) {
			float randAngle = FMath::FRandRange(BulletAngleOffsetMin, BulletAngleOffsetMax);
			CurrentBulletAimDir = UKismetMathLibrary::RotateAngleAxis(CurrentBulletAimDir, randAngle, FVector::UpVector);
		}

		if (bullets[i])
			bullets[i]->AimDirection = CurrentBulletAimDir;
	}
}


//ToDo: Improve this to work better with the character movement
void UProjectileSpawnData::ApplyRecoil(ACharacter* ShootingCharacter)
{
	if (ShootingCharacter) {
		auto CharMovement = ShootingCharacter->GetCharacterMovement();

		auto forceDir = ShootingCharacter->GetActorForwardVector();
		forceDir *= -RecoilForce;

		CharMovement->AddImpulse(forceDir, true);
	}
}

void UProjectileSpawnData::ApplyTimeSlow(float deltaTime)
{
	if (TimeScaleTimer <= 0) {
		if (TimeIsSlowed) {
			TimeIsSlowed = false;
			UGameplayStatics::SetGlobalTimeDilation(cachedWorld, 1.0f);
		}
		return;
	}

	TimeIsSlowed = true;
	TimeScaleTimer >= 0 ? TimeScaleTimer -= deltaTime : 0;
	UGameplayStatics::SetGlobalTimeDilation(cachedWorld, TimeScaleOnFire);
}

void UProjectileSpawnData::CheckOverheat(float deltaTime)
{
	if (OverheatCharge >= OvearheatLimit) {
		if (!bIsOverheated) {
			OverheatRechargeWaitTimer = OverheatRechargeWaitTime;
			bIsOverheated = true;
		}

	}

	if (bIsOverheated) {

		if (OverheatRechargeWaitTimer <= 0)
			OverheatCharge >= 0 ? OverheatCharge -= deltaTime * OverheatedRechargeRate : 0;
		else OverheatRechargeWaitTimer -= deltaTime;

	} else OverheatCharge >= 0 ? OverheatCharge -= deltaTime * RechargeRate : 0;

	if (bIsOverheated && OverheatCharge <= 0) {
		bIsOverheated = false;
	}
}


#pragma endregion




void UProjectileSpawnData::OnShoot()
{
	OnShootSignature.Broadcast();
	FireRateTimer = FireRate;
	ShootCount++;

	TimeScaleTimer = TimeScaleOnFireTime;

	Magazine--;
	if (Magazine <= 0) {
		Reloading = true;
		ReloadTimer = ReloadTime;
	}

	OverheatCharge += OvearheatChargePerShot;
}

bool UProjectileSpawnData::CanShoot(float buttonHeldTime)
{
	if (FireRateTimer > 0)
		return false;

	if (bIsOverheated)
		return false;

	if (Reloading)
		return false;

	if (buttonHeldTime < MinHoldTimeToFire)
		return false;

	return true;
}

void UProjectileSpawnData::StartSpawnData()
{
	ShootCount = 0;
	FireRateTimer = 0;
	OverheatCharge = 0;
	OverheatRechargeWaitTimer = 0;
	TimeScaleTimer = 0;
	Magazine = MagazineSize;

	for (auto& Pair : NewSpawnDataAfterShoots)
	{
		Pair.Key->StartSpawnData();
	}
}

void UProjectileSpawnData::UpdateSpawnData(float deltaTime)
{
	FireRateTimer >= 0 ? FireRateTimer -= deltaTime : 0;

	ApplyTimeSlow(deltaTime);
	CheckOverheat(deltaTime);

	for (auto& Pair : NewSpawnDataAfterShoots)
	{
		Pair.Key->UpdateSpawnData(deltaTime);
	}


	if (Reloading) {
		ReloadTimer -= deltaTime;

		if (ReloadTimer > 0)
			return;

		Magazine = MagazineSize; 
		Reloading = false;
	}
}


