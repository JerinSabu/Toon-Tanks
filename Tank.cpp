// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"

ATank::ATank()
{   

    // Create and attach the spring arm component
    springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    springArm->SetupAttachment(RootComponent); 

    // Create and attach the camera component to the spring arm
    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    camera->SetupAttachment(springArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Called to bind functionality to input
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"),this, &ATank::Turn);

    // Bind action input for firing
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}


// Handle the tank's destruction
void ATank::HandleDestruction()
{
    Super::HandleDestruction();

     // Hide the actor and disable ticking
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

    // Mark the tank as dead
    bAlive = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

    // Get the player controller
    tankPlayerController = Cast<APlayerController>(GetController());
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   
   // If the player controller is valid, handle turret rotation based on cursor position
    if(tankPlayerController)
    {
        FHitResult hitResult;

         // Get the hit result under the cursor
        tankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            hitResult);
        
        // Rotate the turret towards the hit location
        RotateTurret(hitResult.ImpactPoint);
    }

}


// Move the tank forward or backward
void ATank::Move(float value)
{
    // Calculate the change in location
    FVector deltaLocation = FVector::ZeroVector;
    deltaLocation.X = value * speed * UGameplayStatics::GetWorldDeltaSeconds(this);

    // Apply the movement to the actor
    AddActorLocalOffset(deltaLocation, true);

}

// Turn the tank
void ATank::Turn(float value)
{
    // Calculate the change in rotation
    FRotator deltaRotation = FRotator::ZeroRotator;
    deltaRotation.Yaw = value *turnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

    // Apply the rotation to the actor
    AddActorLocalRotation(deltaRotation, true);
}


