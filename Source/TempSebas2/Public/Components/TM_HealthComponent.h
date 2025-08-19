// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TM_HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignature, UTM_HealthComponent*, HealthComponent, AActor*, DamagedActor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DamageCauser);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdateSignature, float, CurrentHealth, float, MaxHealth);


UCLASS( ClassGroup=(ROOM), meta=(BlueprintSpawnableComponent) )
class TEMPSEBAS2_API UTM_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Debug")
	bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
	bool bIsDead;

	UPROPERTY(BlueprintReadWrite, Category="Health Component")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
	AActor* MyOwner;

	FTimerHandle TimerHandle_UpdateInitialHealth;

public: 

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeSignature OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeathDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdateSignature OnHealthUpdateDelegate;

public:	
	// Sets default values for this component's properties
	UTM_HealthComponent();

	UFUNCTION(BlueprintCallable)
	bool IsDead() const {return bIsDead;};

	bool TryAddHealth(float HealthToAdd);

	float GetCurrentHealth() { return Health; };

	void UpdateInitialHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	
		
};
