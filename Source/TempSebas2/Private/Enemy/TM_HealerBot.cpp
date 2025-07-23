// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_HealerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TM_Character.h"
#include "Weapons/TM_Rifle.h"
#include "TeleportProjectile.h"
#include "Weapons/TM_Projectile.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Components/TM_HealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Items/TM_Item.h"
#include "Enemy/TM_BotSpawner.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "Enemy/TM_BotProjectile.h"
#include "TM_Shield.h"

// Sets default values
ATM_HealerBot::ATM_HealerBot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetCanEverAffectNavigation(false);
	//BotMeshComponent->SetSimulatePhysics(true);
	RootComponent = BotMeshComponent;

	HealthComponent = CreateAbstractDefaultSubobject<UTM_HealthComponent>(TEXT("HealthComponent"));

	BotScannerColliderComponent = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("BotScannerComponent"));
	BotScannerColliderComponent->SetupAttachment(RootComponent);
	BotScannerColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BotScannerColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BotScannerColliderComponent->SetSphereRadius(150.0f);

	BotHealZoneColliderComponent = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("BotHealZoneComponent"));
	BotHealZoneColliderComponent->SetupAttachment(RootComponent);
	BotHealZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BotHealZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	BotFlyingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
	BotFlyingMovement->MaxSpeed = 600.f;


	bIsExploded = false;
	ExplosionDamage = 100.0f;
	ExplosionRadius = 200.0f;

	XPValue = 20.f;

	LootProbability = 100.0f;

	bIsRampage = false;

	OrbitRadius = 1000.0f;
	OrbitHeight = -150.0f;
	OrbitSpeed = 1.0f;
	CurrentAngle = 0.0f;

	FireOffset = FVector(100.0f, 0.0f, 50.0f);


	bIsEnemyShielded = false;
	ShieldDistance = 100.f;

	ExplosionDistance = 150.f;
	RampageSpeed = 2000.0f;


}

// Called when the game starts or when spawned
void ATM_HealerBot::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATM_HealerBot::TakingDamage);
	HealthComponent->OnDeathDelegate.AddDynamic(this, &ATM_HealerBot::GiveXP);

	BotScannerColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_HealerBot::EnteredScanningZone);
	BotScannerColliderComponent->OnComponentEndOverlap.AddDynamic(this, &ATM_HealerBot::LeavedScanningZone);

	BotHealZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_HealerBot::EnteredHealZone);
	BotHealZoneColliderComponent->OnComponentEndOverlap.AddDynamic(this, &ATM_HealerBot::LeavedHealZone);


	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HealEnemy, this, &ATM_HealerBot::HealEnemies, 0.8f, true);

}

void ATM_HealerBot::DettachEnemy()
{
	bIsFollowingEnemy = false;
	PlayerCharacter = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldFollowingEnemy);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_RampageMode, this, &ATM_HealerBot::SetRampageMode, 4.0f, false);
	//Todo rampage
}

// Called every frame
void ATM_HealerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRampage)
	{
		RampageMode(DeltaTime);
	}
	else
	{
		BotHealerMovement(DeltaTime);
	}

}



void ATM_HealerBot::BotHealerMovement(float DeltaTime)
{
	float TargetX;
	float TargetY;
	float TargetZ;

	CurrentAngle += OrbitSpeed * DeltaTime;

	if (CurrentAngle > 2 * PI)
	{
		CurrentAngle -= 2 * PI;
	}

	if (bIsFollowingEnemy)
	{
		if (IsValid(PlayerCharacter))
		{
			OrbitCenter.X = PlayerCharacter->GetActorLocation().X;
			OrbitCenter.Y = PlayerCharacter->GetActorLocation().Y;
		}

		TargetX = OrbitCenter.X + (OrbitRadius + 150.0f) * FMath::Cos(CurrentAngle);
		TargetY = OrbitCenter.Y + (OrbitRadius + 150.0f) * FMath::Sin(CurrentAngle);
		TargetZ = OrbitCenter.Z + OrbitHeight + FMath::Sin(CurrentAngle);

		SetActorLocation(FVector(TargetX, TargetY, TargetZ));
		 
		return;
	}

	const float TangentX = -OrbitRadius * FMath::Sin(CurrentAngle);
	const float TangentY = OrbitRadius * FMath::Cos(CurrentAngle);
	TargetZ = OrbitCenter.Z + OrbitHeight + FMath::Sin(CurrentAngle * 2.f);


	FVector TangentDirection = FVector(TangentX, TangentY, 0.f).GetSafeNormal();
	AddMovementInput(TangentDirection, 1.0f);


	FVector CurrentLocation = GetActorLocation();

	SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ));
	
}

void ATM_HealerBot::TakingDamage(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if (CurrentHealthComponent->IsDead())
	{
		SelfDestruction();
	}
}

void ATM_HealerBot::GiveXP(AActor* DamageCauser)
{

	ATM_Character* PossiblePlayer = Cast<ATM_Character>(DamageCauser);
	if (IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
		TrySpawnLoot();
	}

	ATM_Rifle* PossibleRifle = Cast<ATM_Rifle>(DamageCauser);
	if (IsValid(PossibleRifle))
	{
		ATM_Character* RifleOwner = Cast<ATM_Character>(PossibleRifle->GetOwner());
		if (IsValid(RifleOwner) && RifleOwner->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			RifleOwner->GainUltimateXP(XPValue);
			TrySpawnLoot();
		}
	}

	ATeleportProjectile* PossibleTeleportProjectile = Cast<ATeleportProjectile>(DamageCauser);
	if (IsValid(PossibleTeleportProjectile))
	{
		ATM_Character* TeleportProjectileOwner = Cast<ATM_Character>(PossibleTeleportProjectile->GetOwner());
		if (IsValid(TeleportProjectileOwner) && TeleportProjectileOwner->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			TeleportProjectileOwner->GainUltimateXP(XPValue);
			TrySpawnLoot();
		}
	}

	ATM_Projectile* PossibleProjectile = Cast<ATM_Projectile>(DamageCauser);
	if (IsValid(PossibleProjectile))
	{
		APawn* InstigatorPawn = PossibleProjectile->GetInstigator();
		ATM_Character* PossibleProjectileOwner = Cast<ATM_Character>(InstigatorPawn);
		if (IsValid(PossibleProjectileOwner) && PossibleProjectileOwner->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			PossibleProjectileOwner->GainUltimateXP(XPValue);
			TrySpawnLoot();
		}
	}


	BP_GiveXP(DamageCauser);
}

bool ATM_HealerBot::TrySpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);

	if (SelectedProbability <= LootProbability)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ATM_Item>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameter);
	}

	return true;

}

void ATM_HealerBot::SelfDestruction()
{

	if (bIsExploded)
	{
		return;
	}

	bIsExploded = true;

	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 20, FColor::Red, true, 5.0f, 0, 2.0f);
	}

	if (IsValid(MySpawner))
	{
		MySpawner->NotifyBotDead();
	}

	Destroy();
}

void ATM_HealerBot::EnteredScanningZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered Overlap!"));
	if (!bIsFollowingEnemy)
	{
		if (ATM_Character* PossibleCharacter = Cast<ATM_Character>(OtherActor))
		{
			if (PossibleCharacter->GetCharacterType() == ETM_CharacterType::CharacterType_Enemy)
			{
				if (PossibleCharacter->GetHealthComponent()->GetCurrentHealth() <= 50.0f)
				{
					PlayerCharacter = PossibleCharacter;
					bIsFollowingEnemy = true;
					OrbitRadius = 300.0f;
					PossibleCharacter->SetHealerReference(this);
					AddEnemyToHealingArrayZone(PossibleCharacter);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldFollowingEnemy, this, &ATM_HealerBot::ShieldFollowingEnemy, 0.8f, true);
				}
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShootAtPlayer, this, &ATM_HealerBot::ShootAtPlayer, 1.0f, true);
			}


		}
	}

}

void ATM_HealerBot::LeavedScanningZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ATM_Character* PossibleCharacter = Cast<ATM_Character>(OtherActor))
	{
		if (PossibleCharacter->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShootAtPlayer);
		}
	}

}

void ATM_HealerBot::EnteredHealZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsFollowingEnemy)
	{
		if (ATM_Character* PossibleEnemy = Cast<ATM_Character>(OtherActor))
		{
			if (PossibleEnemy->GetCharacterType() == ETM_CharacterType::CharacterType_Enemy)
			{
					AddEnemyToHealingArrayZone(PossibleEnemy);
			}
		}
	}
}


void ATM_HealerBot::LeavedHealZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsFollowingEnemy)
	{
		if (ATM_Character* PossibleEnemy = Cast<ATM_Character>(OtherActor))
		{
			if (PossibleEnemy->GetCharacterType() == ETM_CharacterType::CharacterType_Enemy)
			{
				DeleteEnemyToHealingArrayZone(PossibleEnemy);
			}
		}
	}
}

void ATM_HealerBot::AddEnemyToHealingArrayZone(ATM_Character* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Added Enemy"));
	EnemiesInZone.Add(Enemy);
}


void ATM_HealerBot::DeleteEnemyToHealingArrayZone(ATM_Character* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("Deleted Enemy"));
	EnemiesInZone.Remove(Enemy);
}

void ATM_HealerBot::ShootAtPlayer()
{
	
	FActorSpawnParameters SpawnParams;

	FVector SpawnLocation = GetActorLocation() + FireOffset;
	ATM_BotProjectile* Projectile = GetWorld()->SpawnActor<ATM_BotProjectile>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);


}

void ATM_HealerBot::SetRampageMode()
{
	bIsRampage = true;
	CachedPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	BotFlyingMovement->MaxSpeed = RampageSpeed;

}

void ATM_HealerBot::RampageMode(float DeltaTime)
{
	FVector ToPlayer = CachedPlayer->GetActorLocation() - GetActorLocation();
	float Dist = ToPlayer.Size();

	if (Dist <= ExplosionDistance)
	{
		SelfDestruction();
		return;
	}

	FVector Dir = ToPlayer.GetSafeNormal();


	FRotator LookRot = Dir.Rotation();
	SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));

	AddMovementInput(Dir, 1.f);
}

void ATM_HealerBot::HealEnemies()
{
	int EnemyCount = EnemiesInZone.Num();
	UE_LOG(LogTemp, Warning, TEXT("Total Enemies in Array: %i"), EnemyCount);
	for (ATM_Character* Enemy : EnemiesInZone)
	{
		Enemy->TryAddHealth(10.0f);
	}
}

void ATM_HealerBot::ShieldFollowingEnemy()
{

	if (bIsEnemyShielded)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldFollowingEnemy);
		return;
	}

	if (PlayerCharacter->GetHealthComponent()->GetCurrentHealth() >= 90.0f)
	{
		FVector Forward = PlayerCharacter->GetActorForwardVector();
		//FVector SpawnLoc = PlayerCharacter->GetActorLocation() + Forward * ShieldDistance;
		FVector SpawnLoc = PlayerCharacter->GetActorLocation() + Forward;
		FRotator  SpawnRot = PlayerCharacter->GetActorRotation();


		ATM_Shield* Shield = GetWorld()->SpawnActor<ATM_Shield>(ShieldClass, SpawnLoc, SpawnRot);
		if (Shield)
		{
			Shield->AttachToActor(PlayerCharacter, FAttachmentTransformRules::KeepWorldTransform);
			Shield->GetRootComponent()->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
			bIsEnemyShielded = true;
		}
	}
}


