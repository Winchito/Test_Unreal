// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TM_Fire.h"
#include "Components/TM_HealthComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATM_ExplosiveBarrel::ATM_ExplosiveBarrel()
{
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel"));
	BarrelMeshComponent->SetupAttachment(CustomRootComponent);

	HealthComponent = CreateAbstractDefaultSubobject<UTM_HealthComponent>(TEXT("HealthComponent"));

	BarrelExplosionDamage = 100.0f;
	BarrelExplosionRadius = 80.0f;
}

// Called when the game starts or when spawned
void ATM_ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATM_ExplosiveBarrel::OnBarrelShooted);

	//OnTakePointDamage.AddDynamic(this, &ATM_ExplosiveBarrel::OnBarrelShooted);

}

void ATM_ExplosiveBarrel::OnBarrelShooted(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealthComponent->IsDead())
	{
		ExplodeBarrel();
	}
}

//
//void ATM_ExplosiveBarrel::OnBarrelShooted(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
//{
//	ExplodeBarrel();
//}

void ATM_ExplosiveBarrel::ExplodeBarrel()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	if (IsValid(BarrelExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BarrelExplosionEffect, GetActorLocation());
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), BarrelExplosionRadius, 20, FColor::Red, true, 5.0f, 0, 2.0f);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), BarrelExplosionDamage, GetActorLocation(), BarrelExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	CreateFire();

	Destroy();
}

void ATM_ExplosiveBarrel::CreateFire()
{
	GetWorld()->SpawnActor<ATM_Fire>(FireClass, GetActorTransform());
}

