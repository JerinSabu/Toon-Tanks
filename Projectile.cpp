// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and set up the projectile mesh as the root component
	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = projectileMesh;

	// Create and set up the projectile movement component with initial and max speed
	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	projectileMovementComponent->MaxSpeed = 1300.f;
	projectileMovementComponent->InitialSpeed = 1300.f;

	// Create and attach the trail particle system to the root component
	trailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	trailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Bind the OnHit function to the projectile mesh's OnComponentHit event
	projectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
	// Play the launch sound if it's set
	if(launchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, launchSound, GetActorLocation());
	}
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Function called when the projectile hits another object
void AProjectile::OnHit(UPrimitiveComponent *hitComp, AActor *otherActor, UPrimitiveComponent *otherComp, FVector normalImpulse, const FHitResult &hit)
{
	// Get the owner of the projectile
	AActor* myOwner = GetOwner();
	if(myOwner == nullptr)
	{
		// Destroy the projectile if it has no owner
		Destroy();
		return;
	}

	// Get the instigator controller of the projectile owner
	AController* myOwnerInstigator = myOwner->GetInstigatorController();

	// Set the damage type class to the default damage type
	UClass* damageTypeClass = UDamageType::StaticClass();

	// Apply damage to the other actor if it's valid and not the projectile or its owner
	if(otherActor &&  otherActor != this && otherActor != myOwner)
	{
		UGameplayStatics::ApplyDamage(otherActor, damage, myOwnerInstigator, this, damageTypeClass);

		// Spawn hit particles if they're set
		if(hitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, hitParticles, GetActorLocation(),GetActorRotation());
		}
		
		// Play hit sound if it's set
		if(hitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation());
		}

		// Trigger camera shake if it's set
		if(hitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(hitCameraShakeClass);
		}
	}

	// Destroy the projectile after hitting something
	Destroy();
}