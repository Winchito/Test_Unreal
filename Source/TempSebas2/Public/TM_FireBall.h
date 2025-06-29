// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_FireBall.generated.h"


class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TEMPSEBAS2_API ATM_FireBall : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball Damage Type")
	TSubclassOf<UDamageType> DamageType;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball Properties")
	float FireballDamage;
	
public:	
	// Sets default values for this actor's properties
	ATM_FireBall();

protected:

	//UFUNCTION()
	//void OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnProjectileCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
