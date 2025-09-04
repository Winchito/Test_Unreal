// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_CheatManager.h"
#include "Kismet/GameplayStatics.h"
#include "TM_Character.h"

void UTM_CheatManager::TM_UltimateReady()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		ATM_Character* PlayerCharacter = Cast<ATM_Character>(PlayerPawn);

		if (IsValid(PlayerCharacter) && PlayerCharacter->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			PlayerCharacter->GainUltimateXP(1000);
		}
	}
}
