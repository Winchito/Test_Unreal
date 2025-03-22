// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Weapon.h"

// Sets default values
ATM_Weapon::ATM_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATM_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATM_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATM_Weapon::StartAction()
{
	BP_StartAction();
}

void ATM_Weapon::StopAction()
{
	BP_StopAction();
}

