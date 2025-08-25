// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "TM_Character.h"
#include "Core/TM_GameMode.h"


ATM_DoorKey::ATM_DoorKey()
{

	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KeyTag = "Key_A";
	XPValue = 40.f;

}

void ATM_DoorKey::Pickup(ATM_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	if (IsValid(PickupCharacter) && PickupCharacter->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
	{
		if (IsValid(GameModeReference))
		{
			GameModeReference->AddKeyToCharacter(PickupCharacter, KeyTag);

			PickupCharacter->GainUltimateXP(XPValue);

		}

		Destroy();
	}


}
