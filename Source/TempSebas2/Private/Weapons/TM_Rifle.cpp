// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Rifle.h"

void ATM_Rifle::StartAction()
{
	Super::StartAction();
	UE_LOG(LogTemp, Log, TEXT("Player is firing!"));
}


void ATM_Rifle::StopAction()
{
	Super::StopAction();

	UE_LOG(LogTemp, Log, TEXT("Player has stop firing"));
}


