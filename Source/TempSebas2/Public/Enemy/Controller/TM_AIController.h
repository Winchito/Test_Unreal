// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TM_AIController.generated.h"

class UBehaviorTree;
class ATM_Enemy;
class UBlackboardComponent;
class UAIPerceptionComponent;

UCLASS()
class TEMPSEBAS2_API ATM_AIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATM_AIController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPerceptionComponent* AIPerceptionComponent;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Controller")
	bool bReceivingDamage;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName LoopPathParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName DirectionIndexParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName WaitingTimeParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName CanSeePlayerParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName InvestigatingParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName TargetLocationParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	FName CanHideFromPlayerParameterName;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Controller")
	ATM_Enemy* MyEnemy;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Controller")
	UBlackboardComponent* MyBlackBoard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
	UBehaviorTree* EnemyBehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateSenses(const TArray<AActor*>& UpdatedActors);

public:

	void SetReceivingDamage(bool bNewState) { bReceivingDamage = bNewState; };
};
