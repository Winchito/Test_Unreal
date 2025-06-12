// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/TM_Weapon.h"
#include "TM_Rifle.generated.h"


class UParticleSystem;
/**
 *
 */
UCLASS()
class TEMPSEBAS2_API ATM_Rifle : public ATM_Weapon
{
	GENERATED_BODY()

public:

	ATM_Rifle();

protected:
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "LineTrace")
	float TraceLenght;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "LineTrace|Debug")
	bool bDrawLineTrace;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsWeaponBursting;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int MaxBurstToShoot;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	int BurstCount;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Effects")
	FName TraceParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleEfect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* TraceEffect;

	FTimerHandle TimerHandle_Burst;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

	void FireWeapon();

	void StartWeaponBursting();

public:

	void SetFiringMode(bool bManageWeaponBursting);

};

