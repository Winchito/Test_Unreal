// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_EnemyHealthBar.h"



void UTM_EnemyHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercent = CurrentHealth / MaxHealth;
}
