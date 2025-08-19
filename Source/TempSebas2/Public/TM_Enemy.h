// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TM_Character.h"
#include "TM_Enemy.generated.h"

class ATM_ActorRoutePaths;
class ATM_Item;
class ATM_AIController;
class ATM_EnemySpawner;

/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API ATM_Enemy : public ATM_Character
{
	GENERATED_BODY()

public:

	ATM_Enemy();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|NavigationPath")
	bool bLoopPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Hiding")
	bool bCanHide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|NavigationPath")
	int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|NavigationPath")
	float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ultimate XP")
	float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	float LootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|NavigationPath")
	ATM_ActorRoutePaths* ActorPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<ATM_Item> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Controller")
	ATM_AIController* MyAIController;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	ATM_EnemySpawner* MySpawner;

protected:

	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

	UFUNCTION()
	void HealthChanged(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:

	bool GetLoopPath() { return bLoopPath; };

	int GetDirectionIndex() { return DirectionIndex; };

	float GetWaitingTime() { return WaitingTimeOnPathPoint; };

	bool GetCanHide() { return bCanHide; };	

	void SetSpawner(ATM_EnemySpawner* NewSpawner) { MySpawner = NewSpawner; };

};
