// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HUDUltimate.h"
#include "TM_Character.h"
#include "Kismet/GameplayStatics.h"

void UTM_HUDUltimate::InitializeWidget()
{
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (ATM_Character* PlayerCharacter = Cast<ATM_Character>(PlayerPawn))
		{
			PlayerCharacter->OnUltimateUpdateDelegate.AddDynamic(this, &UTM_HUDUltimate::UpdateUltimateValue);
			PlayerCharacter->OnUltimateStatusDelegate.AddDynamic(this, &UTM_HUDUltimate::UpdateUltimateStatus);

			UpdateUltimateValue(PlayerCharacter->GetCurrentUltimateXP(), PlayerCharacter->GetMaxUltimateXP());
			UpdateUltimateStatus(PlayerCharacter->CanUseUltimate());
		}
	}

}

void UTM_HUDUltimate::UpdateUltimateValue(float CurrentUltimateXP, float MaxUltimateXP)
{
	UltimatePercent = CurrentUltimateXP / MaxUltimateXP;
	UE_LOG(LogTemp, Warning, TEXT("Entro a la funcion del delegate"));
	UE_LOG(LogTemp, Warning, TEXT("Valor del UltimatePercent: %f"), UltimatePercent);
}

void UTM_HUDUltimate::UpdateUltimateStatus(bool bIsAvailable)
{
	UltimateColor = bIsAvailable ? EnabledUltimateColor : DisabledUltimateColor;
}
