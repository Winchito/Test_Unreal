// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TM_HudGameState.generated.h"

class ATM_GameMode;


UCLASS()
class TEMPSEBAS2_API UTM_HudGameState : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	FName GameStateName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	FName VictoryName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Setup")
	FName GameOverName;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	ATM_GameMode* GameModeReference;

public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void OnVictory();

	UFUNCTION()
	void OnGameOver();

	
};
