// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Enemy.h"
#include "TM_Character.h"
#include "Weapons/TM_Rifle.h"
#include "Components/TM_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/TM_Item.h"
#include "AIModule/Classes/Perception/AISense_Damage.h"
#include "Enemy/Controller/TM_AIController.h"
#include "TeleportProjectile.h"
#include "Weapons/TM_Projectile.h"
#include "Enemy/TM_HealerBot.h"

ATM_Enemy::ATM_Enemy()
{
	DirectionIndex = 1;
	bLoopPath = false;
	WaitingTimeOnPathPoint = 1.0f;
	XPValue = 20.0f;
	LootProbability = 100.0f;
	bCanHide = false;
}

void ATM_Enemy::BeginPlay()
{
	Super::BeginPlay();

	MyAIController = Cast<ATM_AIController>(GetController());

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATM_Enemy::HealthChanged);
	HealthComponent->OnDeathDelegate.AddDynamic(this, &ATM_Enemy::GiveXP);
}

void ATM_Enemy::GiveXP(AActor* DamageCauser)
{
	const FString CauserName = DamageCauser
		? DamageCauser->GetName()
		: TEXT("NULL");

	UE_LOG(LogTemp, Warning, TEXT("Intento de XP %s"), *CauserName);

	ATM_Character* PossiblePlayer = Cast<ATM_Character>(DamageCauser);
	if(IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
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
			TrySpawnLoot();
		}
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BP_GiveXP(DamageCauser);
}

bool ATM_Enemy::TrySpawnLoot()
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

void ATM_Enemy::HealthChanged(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsValid(MyAIController))
	{
		return;
	}

	if (CurrentHealthComponent->IsDead())
	{
		MyAIController->UnPossess();

		if (HealerBotReference)
		{
			HealerBotReference->DettachEnemy();
			HealerBotReference = nullptr;
		}

	}
	else
	{
		ATM_Rifle* Rifle = Cast<ATM_Rifle>(DamageCauser);
		if (IsValid(Rifle))
		{
			AActor* RifleOwner = Rifle->GetOwner();
			MyAIController->SetReceivingDamage(true);
			UAISense_Damage::ReportDamageEvent(GetWorld(), this, RifleOwner, Damage, RifleOwner->GetActorLocation(), FVector::ZeroVector);
		}
	}
}

