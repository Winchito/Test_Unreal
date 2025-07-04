// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_ExplosiveBarrel.generated.h"


class ATM_Fire;
UCLASS()
class TEMPSEBAS2_API ATM_ExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* BarrelMeshComponent;

protected:

	UFUNCTION()
		void OnBarrelShooted(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	void ExplodeBarrel();

	void CreateFire();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel Damage")
		float BarrelExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel Damage")
		float BarrelExplosionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel Effect")
		UParticleSystem* BarrelExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball")
		TSubclassOf<ATM_Fire> FireClass;

public:
	// Sets default values for this actor's properties
	ATM_ExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
