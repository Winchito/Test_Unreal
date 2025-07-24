// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_HealerBot.generated.h"


class UStaticMeshComponent;
//class USphereComponent;
class UTM_HealthComponent;
class ATM_Character;
class ATM_BotSpawner;
class ATM_BotProjectile;
class ATM_Item;
class ATM_Shield;
class UFloatingPawnMovement;
class UBoxComponent;
UCLASS()
class TEMPSEBAS2_API ATM_HealerBot : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* BotHealZoneColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UTM_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UFloatingPawnMovement* BotFlyingMovement;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
		bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Self Destruction")
		bool bIsExploded;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Patrolling")
	bool bIsFollowingEnemy;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Shielding")
	bool bIsEnemyShielded;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Rampage")
	bool bIsRampage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bot Ultimate XP")
		float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bot Healing System")
		float MinimumEnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		float LootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot Patrolling")
	float OrbitRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot Patrolling")
	float OrbitHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot Patrolling")
	float OrbitSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bot Patrolling")
		float CurrentAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rampage")
	float ExplosionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rampage")
	float RampageSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Patrolling")
	FVector OrbitCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		FVector FireOffset;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	ATM_Character* PlayerCharacter;

	UMaterialInstanceDynamic* BotMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	UParticleSystem* ExplosionEffect;

	FTimerHandle TimerHandle_AutoDestruct;

	FTimerHandle TimerHandle_FollowEnemy;

	FTimerHandle TimerHandle_HealEnemy;

	FTimerHandle TimerHandle_ShootAtPlayer;
	
	FTimerHandle TimerHandle_ShieldFollowingEnemy;

	FTimerHandle TimerHandle_RampageMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
	TSubclassOf<ATM_Item> LootItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Projectile")
	TSubclassOf<ATM_BotProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Shield")
	TSubclassOf<ATM_Shield> ShieldClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Healing Area")
	TArray<ATM_Character*> EnemiesInZone;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	ATM_BotSpawner* MySpawner;

	APawn* CachedPlayer = nullptr;

protected:

	void BotHealerMovement(float DeltaTime);

	UFUNCTION()
	void TakingDamage(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

	void SelfDestruction();

	UFUNCTION()
	void EnteredHealZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void LeavedHealZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckEnemyHealth();

	void FollowEnemy(ATM_Character* Enemy);

	void HealEnemies();

	void ShieldFollowingEnemy();

	void AddEnemyToHealingArrayZone(ATM_Character* Enemy);

	void DeleteEnemyToHealingArrayZone(ATM_Character* Enemy);

	void ShootAtPlayer();

	void SetRampageMode();

	void RampageMode(float DeltaTime);

public:
	// Sets default values for this actor's properties
	ATM_HealerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void DettachEnemy();

	void SetShieldState();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
