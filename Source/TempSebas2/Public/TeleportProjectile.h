// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportProjectile.generated.h"


class UStaticMeshComponent;
class USphereComponent;
class UParticleSystem;
class UProjectileMovementComponent;



UCLASS()
class TEMPSEBAS2_API ATeleportProjectile : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* TeleportProjectileColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TeleportProjectileMeshComponent;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* TeleportProjectileMovementComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Projectile")
	float MaxLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float MaxTimeToTravel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	float TeleportRadialDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	float TeleportRadialRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate Effect")
	UParticleSystem* DamageAreaEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate Effect")
	UParticleSystem* ProjectileEffect;

	FTimerHandle TimerHandle_MaxTimeToTravel;

public:	
	// Sets default values for this actor's properties
	ATeleportProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ThrowProjectile();

	UFUNCTION()
	void OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void TeleportPlayer();
};
