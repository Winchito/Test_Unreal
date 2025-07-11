// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_Weapon.generated.h"

class UDamageType;
class ACharacter;

UCLASS()
class TEMPSEBAS2_API ATM_Weapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATM_Weapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	ACharacter* CurrentOwnerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_StartAction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_StopAction();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartAction();

	UFUNCTION(BlueprintCallable)
	virtual void StopAction();

	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(ACharacter* NewOwner);

};
