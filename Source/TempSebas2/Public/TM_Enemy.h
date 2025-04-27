// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TM_Character.h"
#include "TM_Enemy.generated.h"

class ATM_ActorRoutePaths;

/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API ATM_Enemy : public ATM_Character
{
	GENERATED_BODY()

public:

	ATM_Enemy();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|NavigationPath")
	bool bLoopPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|NavigationPath")
	int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|NavigationPath")
	float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|NavigationPath")
	ATM_ActorRoutePaths* ActorPath;
	
};
