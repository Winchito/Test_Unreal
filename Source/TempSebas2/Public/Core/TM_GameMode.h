// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TM_GameMode.generated.h"


class ATM_Character;
/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API ATM_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UFUNCTION()
	void Victory(ATM_Character* Character);

	UFUNCTION()
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(ATM_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver();
};
