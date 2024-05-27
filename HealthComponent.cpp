// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize health to the maximum health value
	health = maxHealth;

	// Bind the DamageTaken function to the owner's OnTakeAnyDamage event
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	// Get the game mode and cast it to AToonTanksGameMode
	toonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DamageTaken(AActor *damageActor, float damage, const UDamageType *damageType,class  AController *Instigator, AActor *damageCauser)
{

	// If damage is less than or equal to zero, do nothing
	if(damage <=0.0f) return;

	// Subtract the damage from the current health
	health-= damage;

	// If health is depleted and the game mode is valid, notify the game mode that the actor died
	if(health <=0.f && toonTanksGameMode)
	{
		toonTanksGameMode->ActorDied(damageActor);

	}
	

}

