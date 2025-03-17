// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/TM_Item.h"
#include "TM_DoorKey.generated.h"

class UStaticMeshComponent;
class ATM_Character;

UCLASS()
class TEMPSEBAS2_API ATM_DoorKey : public ATM_Item
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* KeyMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FName KeyTag;

public:

	// Sets default values for this actor's properties
	ATM_DoorKey();


public:

	UFUNCTION(BlueprintCallable, Category = "Getter")
	FName GetKeyTag() const { return KeyTag; };
	
protected:
	virtual void Pickup(ATM_Character* PickupCharacter);
};
