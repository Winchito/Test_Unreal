// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Enemy.h"
#include "TM_Character.h"
#include "Weapons/TM_Rifle.h"
#include "Components/TM_HealthComponent.h"
#include "Components/CapsuleComponent.h"

ATM_Enemy::ATM_Enemy()
{
	DirectionIndex = 1;
	bLoopPath = false;
	WaitingTimeOnPathPoint = 1.0f;
	XPValue = 20.0f;
}

void ATM_Enemy::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnDeathDelegate.AddDynamic(this, &ATM_Enemy::GiveXP);
}

void ATM_Enemy::GiveXP(AActor* DamageCauser)
{
	ATM_Character* PossiblePlayer = Cast<ATM_Character>(DamageCauser);
	if(IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
	}

	ATM_Rifle* PossibleRifle = Cast<ATM_Rifle>(DamageCauser);
	if (IsValid(PossibleRifle))
	{
		ATM_Character* RifleOwner = Cast<ATM_Character>(PossibleRifle->GetOwner());
		if (IsValid(RifleOwner) && RifleOwner->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			RifleOwner->GainUltimateXP(XPValue);
		}
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BP_GiveXP(DamageCauser);
}

