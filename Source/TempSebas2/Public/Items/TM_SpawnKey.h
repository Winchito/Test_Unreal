// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/TM_Item.h"
#include "TM_SpawnKey.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
class ATM_BotSpawner;
UCLASS()
class TEMPSEBAS2_API ATM_SpawnKey : public ATM_Item
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* KeyMeshComponent;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	ATM_BotSpawner* BotSpawnerRef = nullptr;

public:

	// Sets default values for this actor's properties
	ATM_SpawnKey();

protected:
	virtual void Pickup(ATM_Character* PickupCharacter);

public:

	void SetSpawnerRef(ATM_BotSpawner* SpawnRef) { BotSpawnerRef = SpawnRef; };
};
