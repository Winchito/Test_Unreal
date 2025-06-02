// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TM_HealthBox.h"
#include "Components/StaticMeshComponent.h"
#include "TM_Character.h"

ATM_HealthBox::ATM_HealthBox()
{
	HealthBoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMeshComponent"));
	HealthBoxMeshComponent->SetupAttachment(RootComponent);
	HealthBoxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthValue = 15.0f;

}

void ATM_HealthBox::Pickup(ATM_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	bool bSuccesfulHeal = PickupCharacter->TryAddHealth(HealthValue);

	if (bSuccesfulHeal)
	{
		Destroy();
	}

	
}
