// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HUDObjective.h"

void UTM_HUDObjective::ObjectiveCompleted()
{
	bIsCompleted = true;
	CurrentColor = CompletedColor;
}
