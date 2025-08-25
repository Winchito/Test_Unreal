// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TM_HUDUltimate.generated.h"

/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API UTM_HUDUltimate : public UUserWidget
{
	GENERATED_BODY()

protected:


	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	float UltimatePercent;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	FLinearColor UltimateColor;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		FLinearColor EnabledUltimateColor;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		FLinearColor DisabledUltimateColor;

public:


	UFUNCTION(BlueprintCallable)
	void InitializeWidget();


	UFUNCTION()
	void UpdateUltimateValue(float CurrentUltimate, float MaxUltimate);

	UFUNCTION()
	void UpdateUltimateStatus(bool bIsAvailable);
	
};
