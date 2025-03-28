// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Weapon.h"
#include "GameFramework/Character.h"

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

	Damage = 20.0f;
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

void ATM_Weapon::SetCharacterOwner(ACharacter* NewOwner)
{
	if (IsValid(NewOwner)) {
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

