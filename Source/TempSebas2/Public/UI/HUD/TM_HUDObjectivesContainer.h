// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TM_HUDObjectivesContainer.generated.h"

class ATM_GameMode;
class UTM_HUDObjective;

UCLASS()
class TEMPSEBAS2_API UTM_HUDObjectivesContainer : public UUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	ATM_GameMode* GameModeReference;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	TArray<UTM_HUDObjective*> Objectives;


public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION(BlueprintCallable)
	void InitiliazeObjectives(TArray<UTM_HUDObjective*> NewObjectives);

	UFUNCTION()
	void UpdateObjectives(FName KeyTag);

};
