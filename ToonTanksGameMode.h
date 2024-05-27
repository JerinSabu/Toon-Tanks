// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* deadActor);

protected:

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore(int32 updateScore);

private:
	class ATank* tank;
	class AToonTanksPlayerController* toonTanksPlayerController;

	float startDelay = 3.f;

	void HandleGameStart();
	int32 targetTowers = 0;
	int32 GetTargetTowerCount();

	int32 score = 0;

	
};
