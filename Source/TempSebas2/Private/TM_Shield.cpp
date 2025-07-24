// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Shield.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TM_HealthComponent.h"
#include "Enemy/TM_HealerBot.h"

// Sets default values
ATM_Shield::ATM_Shield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShieldMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	RootComponent = ShieldMeshComponent;

	HealthComponent = CreateAbstractDefaultSubobject<UTM_HealthComponent>(TEXT("HealthComponent"));

	ShieldMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

// Called when the game starts or when spawned
void ATM_Shield::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATM_Shield::OnShieldShooted);
	
}

void ATM_Shield::OnShieldShooted(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealthComponent->IsDead())
	{
		HealerBotReference->SetShieldState();
		HealerBotReference = nullptr;
		BP_Destroy();
	}
}

// Called every frame
void ATM_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

