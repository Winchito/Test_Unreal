// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_BotSpawner.generated.h"

class UBillboardComponent;
class ATM_Bot;
class UBoxComponent;

UCLASS()
class TEMPSEBAS2_API ATM_BotSpawner : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* SpawnerBillboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponentCollider;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 1.0f, ClampMin = 1.0f))
	int MaxBotCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	int CurrentBotsCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float TimeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<ATM_Bot> BotClass;

	FTimerHandle TimerHandle_SpawnBot;
	
public:	
	// Sets default values for this actor's properties
	ATM_BotSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBot();

	FVector GetSpawnPoint();

	UFUNCTION()
	void OnEntrandoTest(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	void NotifyBotDead();
};
