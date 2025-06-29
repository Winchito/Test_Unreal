// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_FireBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/DamageType.h"
#include "TM_Character.h"

// Sets default values
ATM_FireBall::ATM_FireBall()
{
	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = ProjectileCollision;


	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileCollision->SetCollisionObjectType(ECC_WorldDynamic);
	//ProjectileCollision->SetCollisionResponseToAllChannels(ECR_Block);

	//ProjectileCollision->SetCollisionProfileName(TEXT("ECC_GameTraceChannel1")); // o el preset que bloquea Pawn/Enemy
	//ProjectileCollision->SetNotifyRigidBodyCollision(true);           // NECESARIO para OnComponentHit



	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;

	FireballDamage = 20.0f;
}

void ATM_FireBall::OnProjectileCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnProjectileCollision llamado contra: %s"), *GetNameSafe(OtherActor));
	ATM_Character* PossibleCharacter = Cast<ATM_Character>(OtherActor);
	if (IsValid(PossibleCharacter))
	{
		if (PossibleCharacter->GetCharacterType() == ETM_CharacterType::CharacterType_Enemy)
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, FireballDamage, FVector::ZeroVector, FHitResult(), GetInstigatorController(), this, DamageType);
			Destroy();
		}
	}



}

// Called when the game starts or when spawned
void ATM_FireBall::BeginPlay()
{
	Super::BeginPlay();

	//ProjectileCollision->OnComponentHit.AddDynamic(this, &ATM_FireBall::OnProjectileCollision);
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ATM_FireBall::OnProjectileCollision);
	
}

