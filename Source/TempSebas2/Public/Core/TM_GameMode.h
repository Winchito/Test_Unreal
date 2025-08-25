// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TM_GameMode.generated.h"


class ATM_Character;
class ATM_SpectatingCamera;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyAddedSignature, FName, KeyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateChange);

UCLASS()
class TEMPSEBAS2_API ATM_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATM_GameMode();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	FName MainMenuMapName;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ATM_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ATM_SpectatingCamera* GameOverCamera;

	FTimerHandle FTimerHandle_BackToMainMenu;

public:

	UPROPERTY(BlueprintAssignable)
	FOnKeyAddedSignature OnKeyAddedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnVictoryDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnGameOverDelegate;

protected:
	
	virtual void BeginPlay() override;

	void SetUpSpectatingCameras();

	void MoveCameraToSpectatingPoint(ATM_SpectatingCamera* SpectatingCamera, ATM_Character* Character);

public:

	UFUNCTION()
	void AddKeyToCharacter(ATM_Character* KeyOwner, FName KeyTag);

	UFUNCTION()
	void Victory(ATM_Character* Character);

	UFUNCTION()
	void GameOver(ATM_Character* Character);

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(ATM_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(ATM_Character* Character);
};
