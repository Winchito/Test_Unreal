// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_Fire.generated.h"


class UBoxComponent;
class UParticleSystem;
class UParticleSystemComponent;
UCLASS()
class TEMPSEBAS2_API ATM_Fire : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* CustomRootComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//UStaticMeshComponent* FireMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UParticleSystemComponent* FireSystemComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* FireColliderComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire Particle")
	UParticleSystem* FireAreaEffect;

	FTimerHandle TimerHandler_TimeOnFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire Properties")
		float FireDurationForPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire Properties")
		float FireDuration;

protected:

	UFUNCTION()
		void EnterFireZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void LeaveFireZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void EndFire();


public:
	// Sets default values for this actor's properties
	ATM_Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
