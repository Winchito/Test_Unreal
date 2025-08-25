// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HUDObjectivesContainer.h"
#include "Core/TM_GameMode.h"
#include "UI/HUD/TM_HUDObjective.h"

void UTM_HUDObjectivesContainer::InitializeWidget()
{
	GameModeReference = Cast<ATM_GameMode>(GetWorld()->GetAuthGameMode());
	if(IsValid(GameModeReference))
	{
		GameModeReference->OnKeyAddedDelegate.AddDynamic(this, &UTM_HUDObjectivesContainer::UpdateObjectives);
	}

}

void UTM_HUDObjectivesContainer::InitiliazeObjectives(TArray<UTM_HUDObjective*> NewObjectives)
{
	for (UTM_HUDObjective* NewObjective : NewObjectives)
	{
		Objectives.AddUnique(NewObjective);
	}
}

void UTM_HUDObjectivesContainer::UpdateObjectives(FName KeyTag)
{
	int ObjectiveIndex = -1;

	if (KeyTag == "KeyA")
	{
		ObjectiveIndex = 0;
	}

	if (KeyTag == "KeyB")
	{
		ObjectiveIndex = 1;
	}

	if (KeyTag == "KeyC")
	{
		ObjectiveIndex = 2;
	}


	if (Objectives.IsValidIndex(ObjectiveIndex))
	{
		UTM_HUDObjective* CurrentObjective = Objectives[ObjectiveIndex];
		if (IsValid(CurrentObjective))
		{
			CurrentObjective->ObjectiveCompleted();
		}
	}
}

