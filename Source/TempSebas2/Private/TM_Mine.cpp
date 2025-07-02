// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Mine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TempSebas2.h"


// Sets default values
ATM_Mine::ATM_Mine()
{
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	MineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mine"));
	MineMeshComponent->SetupAttachment(CustomRootComponent);

	LightMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light Mine"));
	LightMeshComponent->SetupAttachment(CustomRootComponent);

	MineWarningColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MineWarningCollision"));
	MineWarningColliderComponent->SetupAttachment(CustomRootComponent);
	MineWarningColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MineWarningColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MineWarningColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MineWarningColliderComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);

	MineExplosionColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MineExplosionCollision"));
	MineExplosionColliderComponent->SetupAttachment(CustomRootComponent);
	MineExplosionColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MineExplosionColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MineExplosionColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MineExplosionColliderComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MineExplosionColliderComponent->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	MineExplosionColliderComponent->SetGenerateOverlapEvents(true);
	MineExplosionColliderComponent->SetNotifyRigidBodyCollision(true);

	MineExplosionDamage = 100.0f;
	MineExplosionRadius = 80.0f;
}

void ATM_Mine::ActivateMineWarning(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LightMeshComponent->SetMaterial(0, WidgetMaterial_Enabled);

}

void ATM_Mine::DisableMineWarning(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	LightMeshComponent->SetMaterial(0, WidgetMaterial_Disabled);
}

void ATM_Mine::OnMineShooted(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	ExplodeMine();
}

void ATM_Mine::OnMineStepped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ExplodeMine();
}

void ATM_Mine::ExplodeMine()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	if (IsValid(MineExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MineExplosionEffect, GetActorLocation());
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), MineExplosionRadius, 20, FColor::Red, true, 5.0f, 0, 2.0f);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), MineExplosionDamage, GetActorLocation(), MineExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	Destroy();
}

// Called when the game starts or when spawned
void ATM_Mine::BeginPlay()
{
	Super::BeginPlay();


	MineWarningColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_Mine::ActivateMineWarning);
	MineWarningColliderComponent->OnComponentEndOverlap.AddDynamic(this, &ATM_Mine::DisableMineWarning);
	OnTakePointDamage.AddDynamic(this, &ATM_Mine::OnMineShooted);

	MineExplosionColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_Mine::OnMineStepped);
}


