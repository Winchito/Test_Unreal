// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "TM_Character.h"
#include "TM_Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h" 
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "TempSebas2.h"
// Sets default values
ATeleportProjectile::ATeleportProjectile()
{

	TeleportProjectileColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileColliderComponent"));
	RootComponent = TeleportProjectileColliderComponent;
	TeleportProjectileColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TeleportProjectileColliderComponent->SetCollisionObjectType(ECC_WorldDynamic);
	TeleportProjectileColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TeleportProjectileColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	TeleportProjectileColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	TeleportProjectileColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	TeleportProjectileColliderComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	TeleportProjectileColliderComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	TeleportProjectileColliderComponent->SetNotifyRigidBodyCollision(true);


	TeleportProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	TeleportProjectileMeshComponent->SetupAttachment(RootComponent);
	TeleportProjectileMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TeleportProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	TeleportProjectileMovementComponent->UpdatedComponent = RootComponent;
	TeleportProjectileMovementComponent->InitialSpeed = 4000.f;
	TeleportProjectileMovementComponent->MaxSpeed = 4000.f;
	TeleportProjectileMovementComponent->bRotationFollowsVelocity = true;
	TeleportProjectileMovementComponent->bShouldBounce = false;
	TeleportProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	MaxLenght = 20.f;
	MaxTimeToTravel = 0.7f;

	TeleportRadialDamage = 100.0f;
	TeleportRadialRadius = 1500.0f;
}

// Called when the game starts or when spawned
void ATeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* OwnerActor = GetOwner()) {
		TeleportProjectileColliderComponent->IgnoreActorWhenMoving(OwnerActor, true);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_MaxTimeToTravel, this, &ATeleportProjectile::TeleportPlayer, MaxTimeToTravel, false);

	TeleportProjectileColliderComponent->OnComponentHit.AddDynamic(this, &ATeleportProjectile::OnProjectileCollision);

	ThrowProjectile();

}

void ATeleportProjectile::ThrowProjectile()
{
	if (IsValid(ProjectileEffect))
	{
		UGameplayStatics::SpawnEmitterAttached(ProjectileEffect,TeleportProjectileMeshComponent,NAME_None,FVector::ZeroVector,FRotator::ZeroRotator,FVector(1.0f),EAttachLocation::SnapToTarget,true);
	}

	AActor* CurrentOwner = GetOwner();
	if (IsValid(CurrentOwner)) {
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector().GetSafeNormal();
		TeleportProjectileMovementComponent->Velocity = ShotDirection * TeleportProjectileMovementComponent->InitialSpeed;
	}

}

void ATeleportProjectile::OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor && OtherActor != GetOwner())
	{
		TeleportPlayer();
	}

}


void ATeleportProjectile::TeleportPlayer()
{
	TArray<AActor*> Ignored;
	Ignored.Add(this);
	Ignored.Add(GetOwner());

	//DrawDebugSphere(GetWorld(), GetActorLocation(), TeleportRadialRadius, 20, FColor::Blue, true, 5.0f, 0, 2.0f);
	if (IsValid(DamageAreaEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageAreaEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));
	}
	UGameplayStatics::ApplyRadialDamage(GetWorld(), TeleportRadialDamage, GetActorLocation(), TeleportRadialRadius, UDamageType::StaticClass(), Ignored, this, GetInstigatorController(), true);

	if (ATM_Character* OwnerChar = Cast<ATM_Character>(GetOwner()))
	{
		FRotator CorrectRot = OwnerChar->GetActorRotation();
		OwnerChar->TeleportToProjectile(GetActorLocation(), CorrectRot);
	}
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_MaxTimeToTravel);
	Destroy();


}

