// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_LaunchPadTrigger.generated.h"


class UBoxComponent;
class ATM_LaunchPad;
class UMaterialInterface;
UCLASS()
class TEMPSEBAS2_API ATM_LaunchPadTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATM_LaunchPadTrigger();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PadTriggerMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* PadTriggerColliderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* WidgetMaterial_X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* WidgetMaterial_Y;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "LaunchPadActivated")
	bool bIsLaunchTriggerActivated;

	UPROPERTY(EditAnywhere, Category = "LaunchPads")
	TArray<ATM_LaunchPad*> LaunchPadCategories;

protected:

	UFUNCTION()
	void ActivateLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ChangeMaterialColor(bool ColorTrigger);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
