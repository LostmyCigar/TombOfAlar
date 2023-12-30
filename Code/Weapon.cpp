

#include "ActorClasses/Weapon.h"
#include "ActorClasses/Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"



AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	Mesh->SetupAttachment(RootComponent);

	ShootPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootPoint"));
	ShootPoint->SetupAttachment(Mesh);
}


#pragma region Begin/Update

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnData();
}

void AWeapon::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	UpdateSpawnData(deltaTime);
}


void AWeapon::StartSpawnData()
{
	for (int i = 0; i < PrimarySpawnData.Num(); i++)
	{
		if (PrimarySpawnData[i]) {
			PrimarySpawnData[i]->StartSpawnData();
		}
	}
}

void AWeapon::UpdateSpawnData(float deltaTime)
{
	for (int i = 0; i < PrimarySpawnData.Num(); i++)
	{
		if (PrimarySpawnData[i]) {
			PrimarySpawnData[i]->UpdateSpawnData(deltaTime);
		}
	}
}

#pragma endregion


#pragma region Primary

void AWeapon::OnPrimaryPressed(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter)
{
	OnPrimaryPressedBP(AimDir, ShootingCharacter);

	for (int i = 0; i < PrimarySpawnData.Num(); i++)
	{
		if (PrimarySpawnData[i]) {
			PrimarySpawnData[i]->OnFireButtonPressed(GetWorld(), AimDir, AimDistance, ShootingCharacter, this);
		}
	}
}

void AWeapon::OnPrimaryHeld(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float holdTime)
{
	OnPrimaryHeldBP(AimDir, ShootingCharacter, holdTime);

	for (int i = 0; i < PrimarySpawnData.Num(); i++)
	{
		if (PrimarySpawnData[i]) {
			PrimarySpawnData[i]->OnFireButtonHeld(GetWorld(), AimDir, AimDistance, ShootingCharacter, this, holdTime);
		}
	}
}

void AWeapon::OnPrimaryReleased(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float releaseTime)
{
	OnPrimaryReleasedBP(AimDir, ShootingCharacter, releaseTime);

	for (int i = 0; i < PrimarySpawnData.Num(); i++)
	{
		if (PrimarySpawnData[i]) {
			PrimarySpawnData[i]->OnFireButtonReleased(GetWorld(), AimDir, AimDistance, ShootingCharacter, this, releaseTime);
		}
	}
}

#pragma endregion


#pragma region Secondary

void AWeapon::OnSecondaryPressed(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter)
{
	OnSecondaryPressedBP(AimDir, ShootingCharacter);

	for (int i = 0; i < SecondarySpawnData.Num(); i++)
	{
		if (SecondarySpawnData[i]) {
			SecondarySpawnData[i]->OnFireButtonPressed(GetWorld(), AimDir, AimDistance, ShootingCharacter, this);
		}
	}
}

void AWeapon::OnSecondaryHeld(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float holdTime)
{
	OnSecondaryHeldBP(AimDir, ShootingCharacter, holdTime);

	for (int i = 0; i < SecondarySpawnData.Num(); i++)
	{
		if (SecondarySpawnData[i]) {
			SecondarySpawnData[i]->OnFireButtonHeld(GetWorld(), AimDir, AimDistance, ShootingCharacter, this, holdTime);
		}
	}
}

void AWeapon::OnSecondaryReleased(FVector AimDir, float AimDistance, ACharacter* ShootingCharacter, float releaseTime)
{
	OnSecondaryReleasedBP(AimDir, ShootingCharacter, releaseTime);

	for (int i = 0; i < SecondarySpawnData.Num(); i++)
	{
		if (SecondarySpawnData[i]) {
			SecondarySpawnData[i]->OnFireButtonReleased(GetWorld(), AimDir, AimDistance, ShootingCharacter, this, releaseTime);
		}
	}
}

#pragma endregion

