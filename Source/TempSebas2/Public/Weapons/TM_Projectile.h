// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystem;

UCLASS()
class TEMPSEBAS2_API ATM_Projectile : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Projectile Properties")
	bool bIsLongTrigger;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile Properties")
	bool bHasCollided;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile Properties")
	bool bAirTimeout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float MaxAirTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float FloorFuseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Properties")
	float BulletDamage;

	FTimerHandle TimerHandle_IsInAirTimer;

	FTimerHandle TimerHandle_TimeToExplode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Effect")
	UParticleSystem* ExplosionEffect;

public:
	// Sets default values for this actor's properties
	ATM_Projectile();

protected:

	UFUNCTION()
	void OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnAirTimeOut();

	bool CheckForGround(FHitResult Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void SetLongPress(bool bLongPressedTrigger) { bIsLongTrigger = bLongPressedTrigger; };

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Explode();

};