// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TM_Weapon.h"
#include "TM_Rifle.generated.h"

/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API ATM_Rifle : public ATM_Weapon
{
	GENERATED_BODY()
	
protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

};

