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
#include "DrawDebugHelpers.h"
// Sets default values
ATeleportProjectile::ATeleportProjectile()
{
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadTriggerMesh"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);


	ProjectileColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PadTriggerComponent"));
	ProjectileColliderComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 4000.f;
	ProjectileMovementComponent->MaxSpeed = 4000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	MaxLenght = 20.f;
	MaxTimeToTravel = 0.01f;
}

// Called when the game starts or when spawned
void ATeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	//GetProjectileLaunchLocation();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_MaxTimeToTravel, this, &ATeleportProjectile::TeleportPlayer, MaxTimeToTravel, false);

	ThrowProjectile();

	ProjectileColliderComponent->OnComponentHit.AddDynamic(this, &ATeleportProjectile::OnProjectileCollision);


}

void ATeleportProjectile::ThrowProjectile()
{
	//UE_LOG(LogTemp, Warning, TEXT("Entered Projectile"));
	AActor* CurrentOwner = GetOwner();
	if (IsValid(CurrentOwner)) {
		//ProjectileColliderComponent->IgnoreActorWhenMoving(CurrentOwner, true);
		//UE_LOG(LogTemp, Warning, TEXT("Entered if 1"));
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector().GetSafeNormal();
		ProjectileMovementComponent->Velocity = ShotDirection * ProjectileMovementComponent->InitialSpeed;
	}

}

void ATeleportProjectile::OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlaped!"));
	//ATM_Enemy* PossiblePlayer = Cast<ATM_Enemy>(OtherActor);
	//if (IsValid(PossiblePlayer))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Entered overlaped if 1"));
	//	if (PossiblePlayer->GetCharacterType() == ETM_CharacterType::CharacterType_Enemy)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Entered overlaped if 2"));
	//		//GetWorldTimerManager().ClearTimer(TimerHandle_MaxTimeToTravel);
	//		//TArray<AActor*> IgnoredActors;
	//		//IgnoredActors.Add(GetOwner());
	//		//DrawDebugSphere(GetWorld(), GetActorLocation(), 3000.0f, 20, FColor::Blue, true, 5.0f, 0, 2.0f);
	//		//UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, GetActorLocation(), 3000.0f, UDamageType::StaticClass(), IgnoredActors, this, GetInstigatorController(), true);
	//		TeleportPlayer();
	//	}
	//}

	if (ATM_Enemy* HitEnemy = Cast<ATM_Enemy>(OtherActor))
	{
		TArray<AActor*> Ignored;
		Ignored.Add(this);
		Ignored.Add(GetOwner());

		UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, Hit.ImpactPoint, 3000.f, UDamageType::StaticClass(), Ignored, this, GetInstigatorController(), true);

		if (ATM_Character* OwnerChar = Cast<ATM_Character>(GetOwner()))
		{
			OwnerChar->TeleportToProjectile(Hit.ImpactPoint, GetActorRotation());
		}

		Destroy();
	}
}

void ATeleportProjectile::TeleportPlayer()
{

	AActor* CurrentOwner = GetOwner();
	ATM_Character* PossiblePlayer = Cast<ATM_Character>(CurrentOwner);
	if (IsValid(PossiblePlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered overlaped if 3"));
		PossiblePlayer->MakeTeleportDamage();
		PossiblePlayer->TeleportToProjectile(GetActorLocation(), GetActorRotation());
		GetWorldTimerManager().ClearTimer(TimerHandle_MaxTimeToTravel);
		Destroy();
	}

	

}

