// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HudEnemyCounter.h"
#include "Core/TM_GameInstance.h"

void UTM_HudEnemyCounter::InitializeWidget()
{
	GameInstanceReference = Cast<UTM_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->OnEnemyKilledDelegate.AddDynamic(this, &UTM_HudEnemyCounter::UpdateCounter);
		UpdateCounter(GameInstanceReference->GetEnemiesDefeatedCounter());
	}
}

void UTM_HudEnemyCounter::UpdateCounter(int EnemiesDefeatedCounter)
{
	EnemiesDefeated = EnemiesDefeatedCounter;
}
