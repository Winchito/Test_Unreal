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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Over")
	bool bHasToDestroy;

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeSocketName;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* UltimateMontage;

	UAnimInstance* MyAnimInstance;

	ATM_GameMode* GameModeReference;

	FTimerHandle UltimateTimerHandle;

	FTimerHandle TimerHandle_AutomaticShoot;
	
	FTimerHandle TimerHandle_BeginUltimateBehavior;

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

	virtual void Jump() override;

	virtual void StopJumping() override;

	virtual void StartWeaponAction();

	virtual void StopWeaponAction();

	virtual void CreateInitialWeapon();

	UFUNCTION(BlueprintCallable)
	void StartMelee();

	void StopMelee();

	void StartUltimate();

	void StopUltimate();

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

	void UpdateUltimateDuration(float Value);

	void UpdateUltimateDurationWithTimer();

	void BeginUltimateBehavior();

	UFUNCTION(BlueprintCallable)
	ETM_CharacterType GetCharacterType() { return CharacterType; };

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GainUltimateXP(float XPGained); 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateUltimateDuration(float Value);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate();
};
