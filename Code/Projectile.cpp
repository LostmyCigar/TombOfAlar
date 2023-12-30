// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorClasses/Projectile.h"
#include "Interfaces/Damageable.h"
#include "Interfaces/Knockbackable.h"
#include "DataAssets/ProjectileData.h"
#include "DataAssets/ProjectileSpawnData.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void AProjectile::ApplyVelocity()
{
	ProjectileMovement->Velocity = AimDirection * Speed;
}

//This should be removed and projectile should instead use values from the projectile data
//(Instead of copying values)
//Projectile should still cache some of these though
void AProjectile::ApplyProjectileData(TObjectPtr<UProjectileData> projectileData)
{
	if (!projectileData)
		return;
	
	ProjectileData = projectileData;
	TimeSinceBulletDataApplied = 0;

	if (ProjectileData->ChangeAimDirWhenApplied)
		ChangeAimDir(ProjectileData->AimDirChange);

	//Damage
	Damage = ProjectileData->Damage;
	DamageType = ProjectileData->DamageType;

	KnockbackForce = ProjectileData->KnockbackForce;

	//Movement
	Speed = ProjectileData->Speed;
	LinearChangeSpeedOverTime = ProjectileData->LinearChangeSpeedOverTime;
	LinearSpeedChangeValue = ProjectileData->LinearSpeedChangeValue;
	ExponentialChangeSpeedOverTime = ProjectileData->ExponentialChangeSpeedOverTime;
	ExponentialSpeedChangeMultiplier = ProjectileData->ExponentialSpeedChangeMultiplier;

	bUseGravity = ProjectileData->bUseGravity;
	GravityForce = ProjectileData->GravityForce;

	//Destroy
	GetBulletDestruction();

	//Targeting
	bAimOnTarget = ProjectileData->bAimOnTarget;
	TargetLockTurnPower = ProjectileData->TargetLockTurnPower;

	//Data
	ChangeBulletDataAfterTime = ProjectileData->ChangeBulletDataAfterTime;
	NewBulletDataAfterTime = ProjectileData->NewBulletDataAfterTime;
	

}

void AProjectile::OnHitActor(AActor* hitActor, FVector HitWorldLocation)
{
	if (hitActor->Implements<UDamageable>()) {
		DealDamage(hitActor, HitWorldLocation);
		OnHitDamagable(hitActor, HitWorldLocation);

		//Lets not try to destroy twice just in case
		if (DestroyOnHitDamagable && !DestroyOnHitAnything)
			DestroyBullet();
	}

	if (DestroyOnHitAnything)
		DestroyBullet();

	OnHitAnything(hitActor, HitWorldLocation);
}

void AProjectile::DealDamage(AActor* damagableActor, FVector HitWorldLocation)
{
	if (damagableActor->Implements<UDamageable>())
		IDamageable::Execute_TakeDamage(damagableActor, Damage, HitWorldLocation, DamageType);
}

void AProjectile::ApplyKnockback(AActor* knockbackableActor, FVector HitWorldLocation)
{
	if (knockbackableActor->Implements<UKnockbackable>()) {

		auto dir = knockbackableActor->GetActorLocation() - this->GetActorLocation();
		dir.Normalize();

		IKnockbackable::Execute_ApplyKnockback(knockbackableActor, KnockbackForce, dir, this);
	}
		
}

void AProjectile::DestroyBullet()
{
	if (SpawnNewBulletOnDestroy && NewBulletSpawnData)
		NewBulletSpawnData->BulletExplosionSpawn(GetWorld(), this, GetActorForwardVector());

	OnDestroyBullet();
	Destroy();
}

void AProjectile::GetBulletDestruction()
{
	DestroyOnHitDamagable = ProjectileData->DestroyOnHitDamagable;
	DestroyOnHitAnything = ProjectileData->DestroyOnHitAnything;

	SpawnNewBulletOnDestroy = ProjectileData->bSpawnNewBulletOnDestroy;
	NewBulletSpawnData = ProjectileData->NewBulletOnDestroySpawnData;

	float destroyTimer = ProjectileData->DestroyTimer;
	if (ProjectileData->bRandomDestroyTimeOffset) {

		float rand = FMath::FRandRange(ProjectileData->DestroyTimeOffsetMin, ProjectileData->DestroyTimeOffsetMax);
		destroyTimer += rand;
	}

	DestroyTimer = destroyTimer;
}

void AProjectile::AimOnTarget(float deltaTime)
{
	FVector targetPos = TargetPoint;
	FVector currentPos = GetActorLocation();
	
	FVector targetDir = targetPos - currentPos;
	targetDir.Normalize();

	FVector currentDir = ProjectileMovement->Velocity;
	currentDir.Normalize();

	FVector newDir = FMath::Lerp(currentDir, targetDir, TargetLockTurnPower * deltaTime);
	AimDirection = newDir;
	AimDirection.Normalize();

	ApplyVelocity();
}

void AProjectile::ApplySpeedChangeOverTime(float deltaTime)
{
	if (ProjectileData->LinearChangeSpeedOverTime)
		Speed += LinearSpeedChangeValue * deltaTime;

	if (ProjectileData->ExponentialChangeSpeedOverTime)
		Speed += Speed * (ExponentialSpeedChangeMultiplier * deltaTime);

	ApplyVelocity();
}

void AProjectile::ApplyNewBulletDataAfterTime(float timeSinceLastAppliedBulletData)
{
	for (auto& Pair : NewBulletDataAfterTime)
	{
		if (Pair.Value <= timeSinceLastAppliedBulletData) {
			ApplyProjectileData(Pair.Key);
			break;
		}
	}
}

void AProjectile::ChangeAimDir(FVector aimDirAdjustment)
{
	FVector currentDir = ProjectileMovement->Velocity;

	currentDir.Normalize();
	aimDirAdjustment.Normalize();

	FVector newDir = currentDir * aimDirAdjustment;
	newDir.Normalize();

	AimDirection = newDir;
	ApplyVelocity();
}

void AProjectile::ApplyGravity(float deltaTime)
{
	FVector tempVector = ProjectileMovement->Velocity;

	tempVector += FVector::DownVector * ProjectileData->GravityForce * deltaTime;

	ProjectileMovement->Velocity = tempVector;
}



#pragma region Tick/Begin

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceBulletDataApplied += DeltaTime;

	if (!ProjectileData)
		return;

	if (ProjectileData->ChangeBulletDataAfterTime)
		ApplyNewBulletDataAfterTime(TimeSinceBulletDataApplied);

	ApplySpeedChangeOverTime(DeltaTime);

	if (ProjectileData->bAimOnTarget)
		AimOnTarget(DeltaTime);

	if (ProjectileData->bUseGravity)
		ApplyGravity(DeltaTime);

	if (ProjectileData->DontUseDestroyTimer)
		return;
	DestroyTimer -= DeltaTime;
	if (DestroyTimer <= -0.2f) {
		DestroyBullet();
	}
}

#pragma endregion



