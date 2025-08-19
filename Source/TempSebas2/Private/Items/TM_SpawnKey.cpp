// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_SpawnKey.h"
#include "Components/StaticMeshComponent.h"
#include "TM_Character.h"
#include "Items/TM_SpawnKey.h"
#include "Enemy/TM_BotSpawner.h"

ATM_SpawnKey::ATM_SpawnKey()
{
	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ATM_SpawnKey::Pickup(ATM_Character* PickupCharacter)
{

	Super::Pickup(PickupCharacter);

	if (IsValid(BotSpawnerRef))
	{
		BotSpawnerRef->DisableSpawn();
		BotSpawnerRef = nullptr;
	}

	Destroy();
}
