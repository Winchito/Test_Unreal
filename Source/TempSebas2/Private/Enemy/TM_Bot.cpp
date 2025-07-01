// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Bot.h"
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
#include "Items/TM_Item.h"
#include "Enemy/TM_BotSpawner.h"

// Sets default values
ATM_Bot::ATM_Bot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetCanEverAffectNavigation(false);
	BotMeshComponent->SetSimulatePhysics(true);
	RootComponent = BotMeshComponent;

	HealthComponent = CreateAbstractDefaultSubobject<UTM_HealthComponent>(TEXT("HealthComponent"));

	SelfDestructionDetectorComponent = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetector"));
	SelfDestructionDetectorComponent->SetupAttachment(RootComponent);
	SelfDestructionDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SelfDestructionDetectorComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SelfDestructionDetectorComponent->SetSphereRadius(150.0f);


	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 500.0f;
	bIsExploded = false;
	ExplosionDamage = 100.0f;
	ExplosionRadius = 50.0f;

	XPValue = 20.f;

	LootProbability = 100.0f;

}

// Called when the game starts or when spawned
void ATM_Bot::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (IsValid(PlayerPawn))
	{
		PlayerCharacter = Cast<ATM_Character>(PlayerPawn);
	}

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATM_Bot::TakingDamage);
	HealthComponent->OnDeathDelegate.AddDynamic(this, &ATM_Bot::GiveXP);
	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_Bot::StartCountDown);

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComponent->GetMaterial(0));

	NextPathPoint = GetNextPathPoint();


}

FVector ATM_Bot::GetNextPathPoint()
{
	if (!IsValid(PlayerCharacter))
	{
		return GetActorLocation();
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter);

	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	//If Navigation Points are less or equal than 1
	return GetActorLocation();

}

// Called every frame
void ATM_Bot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget <= MinDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();

	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= ForceMagnitude;
		BotMeshComponent->AddForce(ForceDirection, NAME_None, true);
	}

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15, FColor::Purple, false, 0.0f, 0, 1.0f);
	}
}

void ATM_Bot::TakingDamage(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if (CurrentHealthComponent->IsDead())
	{
		//if (IsValid(DamageCauser))
		//{
		//	ATM_Rifle* CharacterCauser = Cast<ATM_Character>(DamageCauser);
		//	if (IsValid(CharacterCauser) && CharacterCauser->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		//	{
		//		TrySpawnLoot();
		//	}
		//}

		SelfDestruction();
	}
}

void ATM_Bot::SelfDestruction()
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

void ATM_Bot::StartCountDown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsStartingCountdown)
	{
		return;
	}

	if (OtherActor == PlayerCharacter)
	{
		bIsStartingCountdown = true;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ATM_Bot::SelfDamage, 0.5f, true);
	}

}

void ATM_Bot::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), nullptr, nullptr);
}

void ATM_Bot::GiveXP(AActor* DamageCauser)
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

bool ATM_Bot::TrySpawnLoot()
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
