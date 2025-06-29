// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TEMPSEBAS2_API ATeleportProjectile : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* ProjectileColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMeshComponent;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly, Category = "Projectile")
	float MaxLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	float MaxTimeToTravel;

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
