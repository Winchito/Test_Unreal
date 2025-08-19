// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HealthBar.h"
#include "Components/TM_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TM_Character.h"

void UTM_HealthBar::InitializeWidget()
{
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (ATM_Character* PlayerCharacter = Cast<ATM_Character>(PlayerPawn))
		{
			if (UTM_HealthComponent* PlayerHealthComponent = PlayerCharacter->GetHealthComponent())
			{
				PlayerHealthComponent->OnHealthUpdateDelegate.AddDynamic(this, &UTM_HealthBar::UpdateHealth);
			}
		}
	}


}

void UTM_HealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercent = CurrentHealth / MaxHealth;
	HealthColor = FMath::Lerp(EmptyHealthColor, FullHealthColor, HealthPercent);
}
