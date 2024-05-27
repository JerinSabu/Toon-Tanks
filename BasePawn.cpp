// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create and set up the capsule component as the root component
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = capsuleComp;

	// Create and attach the base mesh to the capsule component
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->SetupAttachment(capsuleComp);

	// Create and attach the turret mesh to the base mesh
	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	turretMesh->SetupAttachment(baseMesh);

	// Create and attach the projectile spawn point to the turret mesh
	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	projectileSpawnPoint->SetupAttachment(turretMesh);

}

void ABasePawn::HandleDestruction()
{
	// Play visual and sound effects upon destruction
	if(deathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, deathParticles, GetActorLocation(), GetActorRotation());
	}
	if(deathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, deathSound, GetActorLocation());
	}
	if(deathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(deathCameraShakeClass);
	}
	
}


void ABasePawn::RotateTurret(FVector lookAtTarget)
{
	// Calculate the direction to the target
	FVector toTarget = lookAtTarget - turretMesh->GetComponentLocation();

	// Calculate the rotation needed to look at the target, ignoring pitch and roll
	FRotator lookAtRotation = FRotator(0.f,toTarget.Rotation().Yaw, 0.f);

	// Smoothly interpolate the turret's rotation towards the target rotation
	turretMesh->SetWorldRotation(
		FMath::RInterpTo(turretMesh->GetComponentRotation(),
		lookAtRotation,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		15.f));
}


void ABasePawn::Fire()
{

	// Get the location and rotation of the projectile spawn point
	FVector location = projectileSpawnPoint->GetComponentLocation();
	FRotator rotation = projectileSpawnPoint->GetComponentRotation();

	// Spawn the projectile at the spawn point's location and rotation
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, location, rotation);

	// Set the owner of the projectile to this pawn
	projectile->SetOwner(this);
}