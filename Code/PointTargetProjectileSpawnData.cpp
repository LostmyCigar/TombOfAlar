// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/PointTargetProjectileSpawnData.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "ActorClasses/Weapon.h"

void UPointTargetProjectileSpawnData::Shoot(UWorld* world, ACharacter* ShootingCharacter, AWeapon* weapon, FVector aimDirection, float AimDistance, float buttonHeldTime)
{
	if (!CanShoot(buttonHeldTime))
		return;

	FVector hitNormal = FVector::UpVector;
	auto aimPoint = GetMouseAimPosition(world, hitNormal);

	DrawDebugSphere(world, aimPoint, 100.0f, 12, FColor::Blue);

	auto bullets = SpawnBullets(world, weapon->ShootPoint->GetComponentTransform(), weapon->ShootPoint->GetRightVector());
	SetBulletStats(bullets);
	AimBullets(bullets, aimDirection);

	for (int i = 0; i < bullets.Num(); i++)
	{
		bullets[i]->ApplyVelocity();
		bullets[i]->TargetPoint = aimPoint;
	}

	if (ShootingCharacter)
		ApplyRecoil(ShootingCharacter);

	OnShoot();
	weapon->OnShoot();
}

FVector UPointTargetProjectileSpawnData::FirstHitPointUnderTargetPoint(UWorld* world, FVector targetPoint)
{
	FHitResult OutHit;

	FVector EndPoint = targetPoint + FVector::DownVector * 100000;

	//Dont wanna hit this actor
	// FCollisionQueryParams params(FName(TEXT("LineTrace")), true, this);

	bool hit = world->LineTraceSingleByChannel(OutHit, targetPoint, EndPoint, ECC_Visibility);

	if (hit)
		return OutHit.ImpactPoint;


	return FVector(0.0f, 0.0f, 0.0f);
}

//this info should be coming from the player when we rewrite
FVector UPointTargetProjectileSpawnData::GetMouseAimPosition(UWorld* world, FVector& hitNormal)
{
	FVector worldLocation;
	FVector worldDirection;
	APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0);
	if (controller)
		controller->DeprojectMousePositionToWorld(worldLocation, worldDirection);


	FVector startPos = worldLocation;
	FVector endPos = worldLocation + (worldDirection * 100000);
	FHitResult OutHit;


	bool hit = world->LineTraceSingleByChannel(OutHit, startPos, endPos, ECC_Visibility);
		

	if (hit) {
		hitNormal = OutHit.ImpactNormal;
		return OutHit.ImpactPoint;
	}



	return FVector(0.0f, 0.0f, 0.0f);
}

