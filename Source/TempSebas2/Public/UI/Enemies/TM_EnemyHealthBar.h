// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TM_EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API UTM_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float HealthPercent;

public:

	UFUNCTION()
		void UpdateHealth(float CurrentHealth, float MaxHealth);
	
};
