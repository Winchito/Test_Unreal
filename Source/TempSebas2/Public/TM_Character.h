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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	bool bUseFirstPersonView;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aiming")
	bool bIsLookInversion;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
	FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TArray<FName> DoorKeys;

	//Blueprint que existe en el Content Browser del creator
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ATM_Weapon> InitialWeaponClass;

	//Referencia de un objeto ya creado en la escena
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ATM_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UAnimInstance* MyAnimInstance;

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

	void StartMelee();

	void StopMelee();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float value) override;

	void AddKey(FName NewKey);

	bool HasKey(FName KeyTag);

};
