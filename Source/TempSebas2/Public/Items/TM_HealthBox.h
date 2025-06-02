// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/TM_Item.h"
#include "TM_HealthBox.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
class ATM_Character;

UCLASS()
class TEMPSEBAS2_API ATM_HealthBox : public ATM_Item
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HealthBoxMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthBox")
	float HealthValue;

public:

	// Sets default values for this actor's properties
	ATM_HealthBox();


public:

	UFUNCTION(BlueprintCallable, Category = "Getter")
	float GetHealthValue() const { return HealthValue; };

protected:
	virtual void Pickup(ATM_Character* PickupCharacter);
	
};
