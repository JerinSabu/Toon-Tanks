// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // If the tank is in firing range, rotate the turret towards the tank
    if(InFireRange())
    {
        RotateTurret(tank->GetActorLocation());
    }
}

// Handle the tower's destruction
void ATower::HandleDestruction()
{
    Super::HandleDestruction();

    // Destroy the tower actor
    Destroy();
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    // Get the player's tank
    tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    // Set a timer to repeatedly check the fire condition at intervals defined by fireRate
    GetWorldTimerManager().SetTimer(fireRateTimerHandle, this, &ATower::CheckFireCondition, fireRate,true);

}

// Check if the tower should fire
void ATower::CheckFireCondition()
{
    // If there's no tank, do nothing
    if(tank == nullptr)
    {
        return;
    }

    // If the tank is in firing range and is alive, fire at the tank
    if(InFireRange() && tank-> bAlive)
    {
        Fire();
    }

}

// Check if the tank is within firing range
bool ATower::InFireRange()
{
    if(tank)
    {
        // Calculate the distance to the tank
        float distance = FVector::Dist(GetActorLocation(), tank->GetActorLocation());

        // Return true if the tank is within firing range
        if(distance <= fireRange)
        {
            return true;
        }
    }
    // Return false if the tank is not within firing range
    return false;
}