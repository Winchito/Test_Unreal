// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Rifle.h"
#include "TempSebas2.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

ATM_Rifle::ATM_Rifle()
{
	TraceLenght = 10000.0f;
	MuzzleSocketName = "SCK_Muzzle";

	MaxBurstToShoot = 3;
	TimeBetweenShots = 0.1f;
	BurstCount = 0;
	bIsWeaponBursting = false;

}

void ATM_Rifle::StartAction()
{

	if (bIsFiring)
	{
		return;
	}
	Super::StartAction();

	//UE_LOG(LogTemp, Log, TEXT("Player is firing!"));

	if (bIsWeaponBursting)
	{
		BurstCount = 0;
		bIsFiring = true;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Burst, this, &ATM_Rifle::StartWeaponBursting, TimeBetweenShots, true);
	}
	else 
	{
		FireWeapon();
	}

}


void ATM_Rifle::StopAction()
{
	Super::StopAction();

	//BurstCount = 0;
	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Burst);

	//UE_LOG(LogTemp, Log, TEXT("Player has stop firing"));
}

void ATM_Rifle::SetFiringMode(bool bManageWeaponBursting)
{
	bIsWeaponBursting = bManageWeaponBursting;
}



void ATM_Rifle::FireWeapon()
{
	AActor* CurrentOwner = GetOwner();
	if (IsValid(CurrentOwner)) {
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);

		if (bHit)
		{
			//Make damage
			AActor* HitActor = HitResult.GetActor();

			if (IsValid(HitActor))
			{
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
			}

			TraceEndPoint = HitResult.ImpactPoint;

			if (IsValid(ImpactEffect)) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, TraceEndPoint, HitResult.ImpactNormal.Rotation());
			}



		}

		if (bDrawLineTrace)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 2.0f, 0.0f, 1.0f);
		}

		if (IsValid(MuzzleEfect)) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleEfect, CurrentOwnerCharacter->GetMesh(), MuzzleSocketName);
		}

		if (IsValid(TraceEffect))
		{
			USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
			if (IsValid(CharacterMeshComponent))
			{
				FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
				UParticleSystemComponent* TraceComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleSocketLocation);

				if (IsValid(TraceComponent))
				{
					TraceComponent->SetVectorParameter(TraceParamName, TraceEndPoint);
				}
			}
		}

	}
}

void ATM_Rifle::StartWeaponBursting()
{

	if (BurstCount < MaxBurstToShoot)
	{
		FireWeapon();
		BurstCount++;
	}
	else
	{
		BurstCount = 0;
		bIsFiring = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Burst);
	}


}

