// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HudGameState.h"
#include "Core/TM_GameMode.h"

void UTM_HudGameState::InitializeWidget()
{
	GameModeReference = Cast<ATM_GameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameModeReference))
	{
		GameModeReference->OnVictoryDelegate.AddDynamic(this, &UTM_HudGameState::OnVictory);
		GameModeReference->OnGameOverDelegate.AddDynamic(this, &UTM_HudGameState::OnGameOver);
	}

	SetVisibility(ESlateVisibility::Hidden);
}

void UTM_HudGameState::OnVictory()
{
	GameStateName = VictoryName;
	SetVisibility(ESlateVisibility::Visible);
}

void UTM_HudGameState::OnGameOver()
{
	GameStateName = GameOverName;
	SetVisibility(ESlateVisibility::Visible);
}
