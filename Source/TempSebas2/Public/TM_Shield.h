// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_Shield.generated.h"


class UTM_HealthComponent;
class ATM_HealerBot;
UCLASS()
class TEMPSEBAS2_API ATM_Shield : public AActor
{
	GENERATED_BODY()

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShieldMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTM_HealthComponent* HealthComponent;

protected:

	ATM_HealerBot* HealerBotReference;

public:	
	// Sets default values for this actor's properties
	ATM_Shield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnShieldShooted(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Shield")
	void BP_Destroy();

public:

	void SetHealerReference(ATM_HealerBot* HealerBotObject) { HealerBotReference = HealerBotObject; };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
