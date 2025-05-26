// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HealthComponent.h"
#include "..\..\Public\Components\TM_HealthComponent.h"

// Sets default values for this component's properties
UTM_HealthComponent::UTM_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;

	// ...
}


// Called when the game starts
void UTM_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	MyOwner = GetOwner();
	if (IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UTM_HealthComponent::TakingDamage);
	}
	
}

void UTM_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (Health == 0.0f)
	{
		bIsDead = true;
		OnDeathDelegate.Broadcast(DamageCauser);
	}

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	
	if (bDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("My health is: %s"), *FString::SanitizeFloat(Health));
	}
}


