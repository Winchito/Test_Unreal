 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_LaunchPad.generated.h"


class UBoxComponent;
UCLASS()
class TEMPSEBAS2_API ATM_LaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATM_LaunchPad();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LaunchPadMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* LaunchPadColliderComponent;

protected:

	UFUNCTION()
	void PlayerStepped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pad Force")
	FVector LaunchForce;

	UPROPERTY(BlueprintReadOnly, Category = "Pad Force")
	bool bIsActivated;

public:
	void TriggerLaunchPad(bool LaunchPadAction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
