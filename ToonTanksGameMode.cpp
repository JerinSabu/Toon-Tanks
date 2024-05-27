// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"


    // Function called when an actor dies
	void AToonTanksGameMode::ActorDied(AActor *deadActor)
    {
         // Check if the dead actor is the player's tank
        if(deadActor == tank)
        {

            // Handle the tank's destruction
            tank->HandleDestruction();

            // Disable player input
            if(toonTanksPlayerController)
            {
                toonTanksPlayerController->SetPlayerEnabledState(false);
            }

            // End the game with a loss
            GameOver(false);
           
        }

        // Check if the dead actor is a tower
        else if(ATower* destroyedTower = Cast<ATower>(deadActor))
        {

            // Handle the tower's destruction
            destroyedTower->HandleDestruction();

            // Decrement the number of target towers and increment the score
            targetTowers--;
            score++;
            UpdateScore(score);

             // If all towers are destroyed, end the game with a win
            if(targetTowers ==0)
            {
                GameOver(true);
            }
        }

    }
    void  AToonTanksGameMode::BeginPlay()
    {
        Super::BeginPlay();

        // Handle the game start logic
        HandleGameStart();
    }

    // Handle the start of the game
    void AToonTanksGameMode::HandleGameStart()
    {

        // Initialize the score
        score = 0;
        UpdateScore(score);

        // Get the number of target towers
        targetTowers = GetTargetTowerCount();

        // Get references to the player's tank and controller
        tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
        toonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

        // Start the game
        StartGame();

        // Disable player input during the start delay
        if(toonTanksPlayerController)
        {
            toonTanksPlayerController->SetPlayerEnabledState(false);

            // Set a timer to enable player input after the start delay
            FTimerHandle playerEnableTimerHandle;
            FTimerDelegate playerEnableTimerDelegate = FTimerDelegate::CreateUObject(
                toonTanksPlayerController,
                &AToonTanksPlayerController::SetPlayerEnabledState,
                true
            );

            GetWorldTimerManager().SetTimer(playerEnableTimerHandle,
                playerEnableTimerDelegate,
                startDelay,
                false
            );
        }

    }


    // Get the number of target towers in the level
    int32 AToonTanksGameMode::GetTargetTowerCount()
    {
        TArray<AActor*> towers;

        // Find all actors of class ATower
        UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), towers);

        // Return the number of towers found
        return towers.Num();
    }

 