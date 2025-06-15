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

	UPROPERTY(BlueprintReadOnly, Category = "GL Properties")
	bool bIsLongPress;

	UPROPERTY(BlueprintReadOnly, Category = "GL Properties")
	bool bTriggerReleased;

	FTimerHandle TimerHandle_FloorFuseHandle;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade Launcher")
	TSubclassOf<ATM_Projectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, Category = "Grenade Launcher")
	ATM_Projectile* CurrentProjectile;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

public:

	void SetLongPress(bool bStatus);

};
