// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "TM_CheatManager.generated.h"

/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API UTM_CheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UFUNCTION(exec)
	void TM_UltimateReady();
	
};
