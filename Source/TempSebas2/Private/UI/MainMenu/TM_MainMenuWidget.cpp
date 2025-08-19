// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Core/TM_GameInstance.h"


void UTM_MainMenuWidget::NewGame()
{
	BP_NewGame();

	if (UTM_GameInstance* GameInstanceReference = Cast<UTM_GameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstanceReference->ResetData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);
}

void UTM_MainMenuWidget::ContinueGame()
{
	BP_ContinueGame();

	if (UTM_GameInstance* GameInstanceReference = Cast<UTM_GameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstanceReference->LoadData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);
}

void UTM_MainMenuWidget::QuitGame()
{
	BP_QuitGame();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
