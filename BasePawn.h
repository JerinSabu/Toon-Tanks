// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	void HandleDestruction();
	
protected:
	void RotateTurret(FVector lookAtTarget);
	void Fire();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	class UCapsuleComponent* capsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* baseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* turretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	USceneComponent* projectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> projectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* deathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* deathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> deathCameraShakeClass;
};
