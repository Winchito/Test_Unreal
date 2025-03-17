// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "TM_Character.h"


ATM_DoorKey::ATM_DoorKey()
{

	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KeyTag = "Key_A";

}

void ATM_DoorKey::Pickup(ATM_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	PickupCharacter->AddKey(KeyTag);
	Destroy();
}
