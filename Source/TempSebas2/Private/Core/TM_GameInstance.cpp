// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/TM_SaveGame.h"

UTM_GameInstance::UTM_GameInstance()
{
	SaveSlotName = "TM_SaveData";
}

void UTM_GameInstance::AddEnemyDefeatedToCounter()
{
	EnemiesDefeatedCounter++;
	BP_AddEnemyDefeatedToCounter();
}

void UTM_GameInstance::SaveData()
{
	USaveGame* SaveGameObject = nullptr;
	bool ExistingData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	if (ExistingData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(UTM_SaveGame::StaticClass());
	}


	if (IsValid(SaveGameObject))
	{
		if (UTM_SaveGame* SaveFile = Cast<UTM_SaveGame>(SaveGameObject))
		{
			SaveFile->SetEnemiesDefeatedCounterInfo(EnemiesDefeatedCounter);

			UGameplayStatics::SaveGameToSlot(SaveFile, SaveSlotName, 0);
		}
		
	}
}

void UTM_GameInstance::LoadData()
{
	USaveGame* SaveGameObject = nullptr;
	bool ExistingData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	if (ExistingData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	}

	if (IsValid(SaveGameObject))
	{
		if (UTM_SaveGame* SaveFile = Cast<UTM_SaveGame>(SaveGameObject))
		{
			EnemiesDefeatedCounter = SaveFile->GetEnemiesDefeatedCounterInfo();
		}
	}
}

void UTM_GameInstance::ResetData()
{
	EnemiesDefeatedCounter = 0;
	BP_ResetData();
}
