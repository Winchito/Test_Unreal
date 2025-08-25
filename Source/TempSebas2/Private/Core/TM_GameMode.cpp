// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_GameMode.h"
#include "TM_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TM_SpectatingCamera.h"

ATM_GameMode::ATM_GameMode()
{
	MainMenuMapName = "MainMenuMap";
}

void ATM_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetUpSpectatingCameras();
}

void ATM_GameMode::SetUpSpectatingCameras()
{
	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATM_SpectatingCamera::StaticClass(), SpectatingCameraActors);

	if (SpectatingCameraActors.Num() > 0)
	{
		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			ATM_SpectatingCamera* SpectatingCamera = Cast<ATM_SpectatingCamera>(SpectatingActor);
			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case ETM_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;
				case ETM_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}
			}

		}
	}
}

void ATM_GameMode::MoveCameraToSpectatingPoint(ATM_SpectatingCamera* SpectatingCamera, ATM_Character* Character)
{
	if (!IsValid(Character) || !IsValid(SpectatingCamera))
	{
		return;
	}

	AController* CharacterController = Character->GetController();
	if (IsValid(CharacterController))
	{
		APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
		if (IsValid(PlayerController))
		{
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}

}

void ATM_GameMode::AddKeyToCharacter(ATM_Character* KeyOwner, FName KeyTag)
{
	if (IsValid(KeyOwner))
	{
		OnKeyAddedDelegate.Broadcast(KeyTag);
		KeyOwner->AddKey(KeyTag);
	}
}

void ATM_GameMode::Victory(ATM_Character* Character)
{
	Character->DisableInput(nullptr);

	MoveCameraToSpectatingPoint(VictoryCamera, Character);

	OnVictoryDelegate.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(FTimerHandle_BackToMainMenu, this, &ATM_GameMode::BackToMainMenu, 3.0f, false);

	BP_Victory(Character); 
}

void ATM_GameMode::GameOver(ATM_Character* Character)
{
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Character->HasToDestroy())
	{
		Character->DetachFromControllerPendingDestroy();
		Character->SetLifeSpan(10.0f);
	}
	else
	{
		Character->DisableInput(nullptr);
		MoveCameraToSpectatingPoint(GameOverCamera, Character);
	}

	OnGameOverDelegate.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(FTimerHandle_BackToMainMenu, this, &ATM_GameMode::BackToMainMenu, 3.0f, false);

	BP_GameOver(Character);
}

void ATM_GameMode::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}
