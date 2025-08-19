// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_MainMenuButton.h"

UTM_MainMenuButton::UTM_MainMenuButton()
{
	CheckFocusRate = 0.1f;
}

void UTM_MainMenuButton::Start()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckFocus, this, &UTM_MainMenuButton::CheckFocus, CheckFocusRate, true);
}

void UTM_MainMenuButton::CheckFocus()
{
	if (IsHovered())
	{
		//This button is affected by mouse
		SetKeyboardFocus();
		SetButtonStyle(ETM_ButtonStyleType::ButtonStyleType_Mouse);

	}
	else
	{
		//Check if the keyboard/gamepad is on me!
		ETM_ButtonStyleType StyleTypeSelect = HasKeyboardFocus() ? ETM_ButtonStyleType::ButtonStyleType_KeySelected : ETM_ButtonStyleType::ButtonStyleType_KeyNotSelected;
		SetButtonStyle(StyleTypeSelect);
	}
}


void UTM_MainMenuButton::SetButtonStyle(ETM_ButtonStyleType NewStyleType)
{
	BP_SetButtonStyle(NewStyleType);
}