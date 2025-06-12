// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TM_GrenadeLauncher.h"
#include "Weapons/TM_Projectile.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"


ATM_GrenadeLauncher::ATM_GrenadeLauncher() {
	MuzzleSocketName = "SCK_Muzzle";
}

void ATM_GrenadeLauncher::StopAction()
{
	Super::StopAction();

}

void ATM_GrenadeLauncher::StartAction()
{

	Super::StartAction();

	if (IsValid(CurrentOwnerCharacter))
	{
		USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
		if (IsValid(CharacterMeshComponent))
		{
			FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleSocketRotation = CharacterMeshComponent->GetSocketRotation(MuzzleSocketName);
			ATM_Projectile* CurrentProjectile = GetWorld()->SpawnActor<ATM_Projectile>(ProjectileClass, MuzzleSocketLocation, MuzzleSocketRotation);
		}
	}
}
