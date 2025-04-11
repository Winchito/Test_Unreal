// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_GameMode.h"
#include "TM_Character.h"

void ATM_GameMode::Victory(ATM_Character* Character)
{
	Character->DisableInput(nullptr);
	BP_Victory(Character);
}

void ATM_GameMode::GameOver()
{
	BP_GameOver();
}
