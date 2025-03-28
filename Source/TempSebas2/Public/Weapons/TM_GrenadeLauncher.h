// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TM_Weapon.h"
#include "TM_GrenadeLauncher.generated.h"

class ATM_Projectile;
/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API ATM_GrenadeLauncher : public ATM_Weapon
{
	GENERATED_BODY()
	
public:
	ATM_GrenadeLauncher();

protected:

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Effects")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade Launcher")
		TSubclassOf<ATM_Projectile> ProjectileClass;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;


};
