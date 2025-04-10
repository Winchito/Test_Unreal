// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Character.h"
#include "TempSebas2.h"
#include "Gameframework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/TM_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATM_Character::ATM_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	MeleeDamage = 25.0f;
	bIsDoingMelee = false;
	bCanUseWeapon = true;
	MaxComboMultiplier = 4.0f;
	CurrentComboMultiplier = 1.0f;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

FVector ATM_Character::GetPawnViewLocation() const
{
	if (IsValid(FPSCameraComponent) && bUseFirstPersonView) {
		return FPSCameraComponent->GetComponentLocation();
	}

	if (IsValid(TPSCameraComponent) && !bUseFirstPersonView) {
		return TPSCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

// Called when the game starts or when spawned
void ATM_Character::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
	CreateInitialWeapon();
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_Character::MakeMeleeDamage);
}

void ATM_Character::InitializeReferences()
{
	if (IsValid(GetMesh())) 
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}
}

void ATM_Character::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void ATM_Character::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void ATM_Character::Jump()
{
	Super::Jump();
}

void ATM_Character::StopJumping()
{
	Super::StopJumping();
}

void ATM_Character::StartWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartAction();
	}

}

void ATM_Character::StopWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}

	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StopAction();
	}
}

void ATM_Character::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<ATM_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetCharacterOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void ATM_Character::StartMelee()
{
	//UE_LOG(LogTemp, Warning, TEXT("Player starts melee action"));

	if (bIsDoingMelee && !bCanMakeCombos)
	{
		return;
	}

	if (bCanMakeCombos)
	{
		if (bIsDoingMelee)
		{
			if (bIsComboEnable)
			{
				if (CurrentComboMultiplier < MaxComboMultiplier)
				{
					CurrentComboMultiplier++;
					SetComboEnable(false);
				}
			}
			else
			{
				return;
			}
		}

	}

	if (IsValid(MyAnimInstance) && IsValid(MeleeMontage))
	{
		MyAnimInstance->Montage_Play(MeleeMontage);
	}

	SetActionsState(true);

}

void ATM_Character::StopMelee()
{
	//UE_LOG(LogTemp, Warning, TEXT("Player stops melee action"));
}

void ATM_Character::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
	}
}

void ATM_Character::AddControllerPitchInput(float value)
{
	//bIsLookInversion ? Super::AddControllerPitchInput(-value) : Super::AddControllerPitchInput(value);
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}

void ATM_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool ATM_Character::HasKey(FName KeyTag) {
	return DoorKeys.Contains(KeyTag);
}

void ATM_Character::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

void ATM_Character::SetActionsState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void ATM_Character::SetComboEnable(bool NewState)
{
	bIsComboEnable = NewState;
}

void ATM_Character::ResetCombo()
{
	SetComboEnable(false);
	CurrentComboMultiplier = 1.0f;
}

// Called every frame
void ATM_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ATM_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &ATM_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATM_Character::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ATM_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATM_Character::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATM_Character::StopJumping);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &ATM_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &ATM_Character::StopWeaponAction);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ATM_Character::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &ATM_Character::StopMelee);
}


