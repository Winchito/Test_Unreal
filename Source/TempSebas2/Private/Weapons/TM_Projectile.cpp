// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/TM_Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/DamageType.h"

// Sets default values
ATM_Projectile::ATM_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = ProjectileCollision;
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileCollision->SetCollisionObjectType(ECC_WorldDynamic);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECR_Block);
	//ProjectileCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ProjectileCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
	ProjectileMovementComponent->MaxSpeed = 4000.0f;

	bIsLongTrigger = false;

	bHasCollided = false;

	bAirTimeout = false;

	MaxAirTime = 5.0f;

	FloorFuseTime = 0.5f;

	ExplosionDamage = 50.0f;

	BulletDamage = 10.f;

	bDebug = true;

	ExplosionRadius = 20.0f;


}

void ATM_Projectile::OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (bIsLongTrigger)
	{
		return;
	}

	if (CheckForGround(Hit))
	{
		FVector IncomingVelocity = HitComponent->GetComponentVelocity();
		FVector Reflected = FVector::VectorPlaneProject(IncomingVelocity, Hit.Normal) * -1;

		FVector PerfectBounce = FMath::GetReflectionVector(IncomingVelocity, Hit.Normal);

		HitComponent->SetPhysicsLinearVelocity(PerfectBounce * 10.0f);

		if (!bIsLongTrigger)
		{
			if (IsValid(OtherActor))
			{
				UGameplayStatics::ApplyPointDamage(OtherActor, BulletDamage, NormalImpulse, FHitResult(), GetInstigatorController(), this, UDamageType::StaticClass());
			}
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_TimeToExplode, this, &ATM_Projectile::Explode, FloorFuseTime, false);
			return;
		}

	}

	if (!bIsLongTrigger)
	{
		if (IsValid(OtherActor))
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, BulletDamage, NormalImpulse, FHitResult(), GetInstigatorController(), this, UDamageType::StaticClass());
		}
		Explode();
	}

	if (bAirTimeout)
	{
		Explode();
	}

	if (IsValid(OtherActor))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, BulletDamage, NormalImpulse, FHitResult(), GetInstigatorController(), this, UDamageType::StaticClass());
	}
}

bool ATM_Projectile::CheckForGround(FHitResult Hit)
{
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0, 0, -100);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		float ZNormal = Hit.ImpactNormal.Z;

		if (ZNormal > 0.99f)
		{
			return true;
		}
	}

	return false;
}

void ATM_Projectile::OnAirTimeOut()
{
	bAirTimeout = true;
}



void ATM_Projectile::Explode()
{
	if (IsValid(ExplosionEffect))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_IsInAirTimer);
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeToExplode);
		TArray<AActor*> IgnoredActors;
		//IgnoredActors.Add(this);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, UDamageType::StaticClass(), IgnoredActors, this, GetInstigatorController(), true);
	}

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 20, FColor::Red, true, 5.0f, 0, 2.0f);
	}

	Destroy();
}

// Called when the game starts or when spawned
void ATM_Projectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollision->OnComponentHit.AddDynamic(this, &ATM_Projectile::OnProjectileCollision);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_IsInAirTimer, this, &ATM_Projectile::OnAirTimeOut, MaxAirTime, false);

}

// Called every frame
void ATM_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

