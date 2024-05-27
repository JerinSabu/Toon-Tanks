// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include "GameFramework/Pawn.h"


// Function to enable or disable player input
void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{

    // If player input should be enabled
    if(bPlayerEnabled)
    {
        // Enable input for the pawn controlled by this player controller
        GetPawn()->EnableInput(this);
    }

    // If player input should be disabled
    else
    {
        // Disable input for the pawn controlled by this player controller
        GetPawn()->DisableInput(this);

    }

    // Show or hide the mouse cursor based on the player enabled state
    bShowMouseCursor = bPlayerEnabled;
}