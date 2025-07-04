// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TM_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ATM_Weapon;
class UAnimMontage;
class UAnimInstance;
class UTM_HealthComponent;
class ATM_GameMode;
class ATM_GrenadeLauncher;
class ATM_FireBall;
class ATeleportProjectile;
class UParticleSystemComponent;

UENUM()
enum class ETM_CharacterType : uint8
{
	CharacterType_Player		UMETA(DisplayName = "Player"),
	CharacterType_Enemy			UMETA(DisplayName = "Enemy")
};

UCLASS()
class TEMPSEBAS2_API ATM_Character : public ACharacter
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FPSCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UCapsuleComponent* MeleeDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTM_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleSystemComponent = nullptr;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	bool bUseFirstPersonView;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aiming")
	bool bIsLookInversion;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsDoingMelee;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bCanUseWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	bool bCanMakeCombos;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsComboEnable;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bCanUseUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bIsUsingUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	bool bCanUseTeleportUltimate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Over")
	bool bHasToDestroy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsMoving;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bCanSprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged Melee")
	bool bIsDoingRangedMelee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged Melee")
	bool bCanComboRangedMelee;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsBurstModeActivated;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsLongShotActivated;

	UPROPERTY(BlueprintReadOnly, Category = "Player Effects")
	bool bIsBurning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxJumpsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CurrentJumpsInAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	bool bUltimateWithTick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
	float MaxComboMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
	float CurrentComboMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateXP;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	float CurrentUltimateXP;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Time", meta = (ClampMin = 0.0, UIMin = 0.0))
	float MaxUltimateDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Time")
	float CurrentUltimateDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
	float UltimateFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
	float UltimateShotFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimateWalkSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
	float NormalWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
	float UltimatePlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities")
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float LongShotThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Effects")
	float BurningDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged Melee")
	int MaxRangedMelees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged Melee")
	int CurrentRangedMelees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
	int MaxTeleportUltimateProjectiles;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
	int CurrentTeleportUltimateProjectiles;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName HandRangedMeleeSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName TeleportProjectileSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TArray<FName> DoorKeys;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETM_CharacterType CharacterType;

	//Blueprint que existe en el Content Browser del creator
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ATM_Weapon> InitialWeaponClass;

	//Referencia de un objeto ya creado en la escena
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ATM_Weapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Fireball")
	ATM_FireBall* Fireball;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball")
	TSubclassOf<ATM_FireBall> FireballClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fireball")
	TSubclassOf<ATeleportProjectile> TeleportProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* RangedMeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateTeleportMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire Particle")
	UParticleSystem* FireEffectParticleSystem;

	UAnimInstance* MyAnimInstance;

	ATM_GameMode* GameModeReference;

	FTimerHandle UltimateTimerHandle;

	FTimerHandle TimerHandle_AutomaticShoot;

	FTimerHandle TimerHandle_LongShootGrenadeLauncher;
	
	FTimerHandle TimerHandle_BeginUltimateBehavior;

	FTimerHandle TimerHandle_BurnPlayer;

	FTimerHandle TimerHandle_BurnEnd;

public:
	// Sets default values for this character's properties
	ATM_Character();

	virtual FVector GetPawnViewLocation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeReferences();

	void MoveForward(float value);

	void MoveRight(float value);

	void SetLongPressGrenadeLauncher();

	virtual void Jump() override;

	virtual void StopJumping() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual bool CanJumpInternal_Implementation() const override;

	virtual void StartWeaponAction();

	virtual void StopWeaponAction();

	virtual void CreateInitialWeapon();

	UFUNCTION(BlueprintCallable)
	void StartMelee();

	UFUNCTION(BlueprintCallable)
	void StartRangedMelee();

	void StopMelee();

	void StartUltimate();

	void StartTeleportUltimate();

	void StopUltimate();

	void StartSprinting();

	void StopSprinting();

	void StartDashing();

	void StopDashing();

	void SetFireMode();

	void BurnPlayer();

	void EndBurningPlayer();

	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHealthChange(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float value) override;

	void AddKey(FName NewKey);

	bool TryAddHealth(float HealthToAdd);

	bool HasKey(FName KeyTag);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetActionsState(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetComboEnable(bool NewState);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	bool HasToDestroy() { return bHasToDestroy; };

	UFUNCTION(BlueprintCallable)
	void GainUltimateXP(float XPGained);

	UFUNCTION()
	void LaunchFireball();

	UFUNCTION()
	void LaunchTeleportProjectile();

	void TeleportToProjectile(FVector TeleportVector, FRotator RotatorVector);

	void MakeTeleportDamage();

	void UpdateUltimateDuration(float Value);

	void UpdateUltimateDurationWithTimer();

	void UpdateTeleportUltimateDurationWithTimer();

	void UpdateTeleportUltimateDuration(float Value);

	void BeginUltimateBehavior();

	void BeginTeleportUltimateBehavior();

	void StopRangedMelee();

	void SetRangedMeleeComboState(bool MeleeState);

	void SetTeleportProjectileUltimateState(bool UltimateState) { bCanUseTeleportUltimate = UltimateState; };

	void SetCharacterSpeed();

	void SetPlayerOnFire(float FireDuration);

	UAnimInstance* GetAnimInstance() {return MyAnimInstance;};

	UCapsuleComponent* GetMeleeDetectorComponent() { return MeleeDetectorComponent; };

	UFUNCTION(BlueprintCallable)
	ETM_CharacterType GetCharacterType() { return CharacterType; };

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GainUltimateXP(float XPGained); 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartTeleportUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateUltimateDuration(float Value);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate();
};
