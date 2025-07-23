// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_BotProjectile.h"
#include "TM_Character.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATM_BotProjectile::ATM_BotProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;


	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	ProjectileMesh->SetNotifyRigidBodyCollision(true);

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ATM_BotProjectile::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 5000.f;


	Speed = 2000.0f;
	HomingOffset = FVector(100.0f, 0, 25.0f);

}

// Called when the game starts or when spawned
void ATM_BotProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.0f);
	
}

// Called every frame
void ATM_BotProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		HomingTargetComponent = NewObject<USceneComponent>(PlayerPawn);
		HomingTargetComponent->RegisterComponent();
		HomingTargetComponent->AttachToComponent(
			PlayerPawn->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform
		);
		HomingTargetComponent->SetRelativeLocation(HomingOffset);

		ProjectileMovement->HomingTargetComponent = HomingTargetComponent;
	}
}


void ATM_BotProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == this) return;

	if (ATM_Character* PossiblePlayer = Cast<ATM_Character>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 20.0f, GetInstigatorController(), this, nullptr);
		Destroy();
	}
}

