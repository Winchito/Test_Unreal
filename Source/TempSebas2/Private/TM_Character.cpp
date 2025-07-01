// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Character.h"
#include "TempSebas2.h"
#include "Gameframework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/TM_Weapon.h"
#include "Weapons/TM_Rifle.h"
#include "Weapons/TM_GrenadeLauncher.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TM_HealthComponent.h"
#include "Core/TM_GameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "TM_FireBall.h"
#include "Items/TM_LaunchPadTrigger.h"
#include "TeleportProjectile.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DamageType.h"

// Sets default values
ATM_Character::ATM_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	HandRangedMeleeSocketName = "SCK_RangedMelee";
	TeleportProjectileSocketName = "Muzzle_03";
	MeleeDamage = 25.0f;
	bIsDoingMelee = false;
	bCanUseWeapon = true;
	bCanSprint = true;
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

	MeleeDetectorComponent->SetCollisionObjectType(COLLISION_ENEMY);    // test
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UTM_HealthComponent>(TEXT("HealthComponent"));


	bIsBurstModeActivated = false;

	bUltimateWithTick = true;
	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 10.0f;
	UltimateFrequency = 0.5f;

	UltimateWalkSpeed = 2000.0f;
	UltimatePlayRate = 2.0f;
	PlayRate = 1.0f;
	UltimateShotFrequency = 0.1f;
	MaxJumpsInAir = 2;
	CurrentJumpsInAir = 0;

	LongShotThreshold = 0.099f;
	bIsLongShotActivated = false;

	CurrentRangedMelees = 0;
	MaxRangedMelees = 3;

	MaxTeleportUltimateProjectiles = 3;
	CurrentTeleportUltimateProjectiles = 0;
	bCanUseTeleportUltimate = true;
}

/*
* -------------------------------------------------------------------------------------------------------------------------
* |											Character Initialization												      |
* |------------------------------------------------------------------------------------------------------------------------
*/

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

void ATM_Character::InitializeReferences()
{
	if (IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<ATM_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called when the game starts or when spawned
void ATM_Character::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
	CreateInitialWeapon();
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_Character::MakeMeleeDamage);

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATM_Character::OnHealthChange);

	NormalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}


void ATM_Character::AddControllerPitchInput(float value)
{
	//bIsLookInversion ? Super::AddControllerPitchInput(-value) : Super::AddControllerPitchInput(value);
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}
//-----------------------------------------------------------------------------------------------------------------------------------------

/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Health Component											          |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

void ATM_Character::OnHealthChange(UTM_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead() && GetCharacterType() == ETM_CharacterType::CharacterType_Player)
	{
		if (IsValid(GameModeReference))
		{
			GameModeReference->GameOver(this);
		}
	}
}

bool ATM_Character::TryAddHealth(float HealthToAdd)
{
	return HealthComponent->TryAddHealth(HealthToAdd);
}

//-----------------------------------------------------------------------------------------------------------------------------------------


/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Movement															  |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

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

	UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();
	if (PlayerMovement->IsMovingOnGround())
	{
		Super::Jump();
	}
	else if (CurrentJumpsInAir < MaxJumpsInAir)
	{
		Super::Jump();
		CurrentJumpsInAir++;
	}

}

bool ATM_Character::CanJumpInternal_Implementation() const
{
	if (CurrentJumpsInAir < MaxJumpsInAir)
	{
		return true;
	}
	return Super::CanJumpInternal_Implementation();
}


void ATM_Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CurrentJumpsInAir = 0;
}


void ATM_Character::StopJumping()
{
	Super::StopJumping();
}

 //-----------------------------------------------------------------------------------------------------------------------------------------------

/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Movement Abilities											      |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

void ATM_Character::StartSprinting()
{
	if (bIsUsingUltimate)
	{
		return;
	}
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (bCanSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		bIsSprinting = true;
	}
}

void ATM_Character::StopSprinting()
{
	if (bIsUsingUltimate)
	{
		return;
	}

	if (bCanSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		bIsSprinting = false;
	}

}

void ATM_Character::StartDashing()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	FVector Velocity = GetVelocity();
	FVector Direction = Velocity.GetSafeNormal();
	FVector ForwardVec = GetActorForwardVector();
	FVector RightVec = GetActorRightVector();
	const float Threshold = 0.1f;
	const float DashStrength = 5000.f;

	float ForwardDot = FVector::DotProduct(Direction, ForwardVec);
	if (FMath::Abs(ForwardDot) > Threshold)
	{
		FVector DashDir = ForwardVec * FMath::Sign(ForwardDot);
		LaunchCharacter(DashDir * DashStrength, true, true);
		return;
	}

	float RightDot = FVector::DotProduct(Direction, RightVec);
	if (FMath::Abs(RightDot) > Threshold)
	{
		FVector DashDir = RightVec * FMath::Sign(RightDot);
		LaunchCharacter(DashDir * DashStrength, true, true);
		return;
	}

	LaunchCharacter(ForwardVec * DashStrength, true, true);
}

void ATM_Character::StopDashing()
{

}

//---------------------------------------------------------------------------------------------------------------------------------

/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Mechanics															  |
* |-----------------------------------------------------------------------------------------------------------------------|
*/


/*
* ------------------------------Key Mechanic-----------------------------
*/

void ATM_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}


bool ATM_Character::HasKey(FName KeyTag) {
	return DoorKeys.Contains(KeyTag);
}

//-------------------------------------------------------------------------

/*
* ------------------------------XP Mechanic-----------------------------
*/

void ATM_Character::GainUltimateXP(float XPGained)
{
	if (bCanUseUltimate || bIsUsingUltimate)
	{
		return;
	}

	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XPGained, 0.0f, MaxUltimateXP);

	if (CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;

	}

	BP_GainUltimateXP(XPGained);
}

//-------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------

/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Weapons															  |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

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

void ATM_Character::StartWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}
	if (bIsSprinting) {
		return;
	}

	if (IsValid(CurrentWeapon)) {

		ATM_GrenadeLauncher* PossibleGrenadeLauncher = Cast<ATM_GrenadeLauncher>(CurrentWeapon);
		if (IsValid(PossibleGrenadeLauncher))
		{
			
			bIsLongShotActivated = false;

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_LongShootGrenadeLauncher, this, &ATM_Character::SetLongPressGrenadeLauncher, LongShotThreshold, false);

			PossibleGrenadeLauncher->SetLongPress(bIsLongShotActivated);

			CurrentWeapon->StartAction();

		}
		else
		{
			CurrentWeapon->StartAction();

			if (bIsUsingUltimate)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutomaticShoot, CurrentWeapon, &ATM_Weapon::StartAction, UltimateShotFrequency, true);
			}
		}

	}

}
void ATM_Character::SetLongPressGrenadeLauncher()
{
	bIsLongShotActivated = true;

	ATM_GrenadeLauncher* GrenadeLauncher = Cast<ATM_GrenadeLauncher>(CurrentWeapon);
	if (IsValid(GrenadeLauncher))
	{
		GrenadeLauncher->SetLongPress(bIsLongShotActivated);
	}
	
}

void ATM_Character::StopWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}

	if (IsValid(CurrentWeapon)) {

		ATM_GrenadeLauncher* PossibleGrenadeLauncher = Cast<ATM_GrenadeLauncher>(CurrentWeapon);
		if (IsValid(PossibleGrenadeLauncher))
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_LongShootGrenadeLauncher);

			if (bIsLongShotActivated)
			{
				CurrentWeapon->StopAction();
			}

		}
		CurrentWeapon->StopAction();
		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShoot);
		}
	}
}

void ATM_Character::SetFireMode()
{
	ATM_Rifle* CurrentRifle = Cast<ATM_Rifle>(CurrentWeapon);

	if (IsValid(CurrentRifle))
	{
		bIsBurstModeActivated = !bIsBurstModeActivated;
		CurrentRifle->SetFiringMode(bIsBurstModeActivated);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------

/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Melee															  |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

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
		MyAnimInstance->Montage_Play(MeleeMontage, PlayRate);
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
		if (OtherActor == this)
		{
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Player entered function"));

		ATM_LaunchPadTrigger* Pad = Cast<ATM_LaunchPadTrigger>(OtherActor);
		if (Pad)
		{
			Pad->LaunchPadTrigger();
		}


		ATM_Character* MeleeTarget = Cast<ATM_Character>(OtherActor);
		if (IsValid(MeleeTarget))
		{
			bool bPlayerAttackingEnemy = GetCharacterType() == ETM_CharacterType::CharacterType_Player && MeleeTarget->GetCharacterType() == ETM_CharacterType::CharacterType_Enemy;
			bool bEnemyAttackingPlayer = GetCharacterType() == ETM_CharacterType::CharacterType_Enemy && MeleeTarget->GetCharacterType() == ETM_CharacterType::CharacterType_Player;

			if (bPlayerAttackingEnemy || bEnemyAttackingPlayer)
			{
				UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
			}
		}
		else
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
		}


	}
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

//------------------------------------------------------------------------------------------------------------------------------

/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Ranged Melee													      |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

void ATM_Character::StartRangedMelee()
{

	const TArray<FName> AnimationNames = { "Melee1", "Melee2", "Melee3" };

	if (bIsDoingMelee && !bCanComboRangedMelee)
	{
		return;
	}

	if (CurrentRangedMelees == MaxRangedMelees)
	{
		StopRangedMelee();
	}

	if (bCanComboRangedMelee)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered melee if 1"));
		if (IsValid(MyAnimInstance) && IsValid(RangedMeleeMontage))
		{
			bIsDoingMelee = true;
			MyAnimInstance->Montage_JumpToSection(AnimationNames[CurrentRangedMelees], RangedMeleeMontage);
			CurrentRangedMelees++;
		}
	}
	else
	{
		if (IsValid(MyAnimInstance) && IsValid(RangedMeleeMontage))
		{
			bIsDoingMelee = true;
			bCanComboRangedMelee = true;
			MyAnimInstance->Montage_Play(RangedMeleeMontage, PlayRate);
			CurrentRangedMelees++;
		}
	}
}

void ATM_Character::LaunchFireball()
{

	FVector MuzzleSocketLocation = GetMesh()->GetSocketLocation(HandRangedMeleeSocketName);
	FRotator MuzzleSocketRotation = GetMesh()->GetSocketRotation(HandRangedMeleeSocketName);
	GetWorld()->SpawnActor<ATM_FireBall>(FireballClass, MuzzleSocketLocation, MuzzleSocketRotation);
}

void ATM_Character::StopRangedMelee()
{
	if (IsValid(MyAnimInstance))
	{
		MyAnimInstance->Montage_Stop(0.1f, RangedMeleeMontage);
	}
	// Reset completo
	bIsDoingMelee = false;
	bCanComboRangedMelee = false;
	CurrentRangedMelees = 0;
}

void ATM_Character::SetRangedMeleeComboState(bool MeleeState)
{
	bCanComboRangedMelee = MeleeState;
}

//---------------------------------------------------------------------------------------------------------------------------------


/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Ultimate													          |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

void ATM_Character::StartUltimate()
{

	if (bCanUseUltimate && !bIsUsingUltimate)
	{ 

		CurrentUltimateDuration = MaxUltimateDuration;
		bCanUseUltimate = false;

		if (bIsSprinting)
		{
			StopSprinting();
		}

		if (IsValid(MyAnimInstance) && IsValid(UltimateMontage))
		{
			bCanSprint = false;
			//GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateMontage);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehavior, this, &ATM_Character::BeginUltimateBehavior, StartUltimateMontageDuration, false);
		}
		else
		{
			BeginUltimateBehavior();
		}

		BP_StartUltimate();
	}
}

void ATM_Character::BeginUltimateBehavior()
{
	bIsUsingUltimate = true;
	GetCharacterMovement()->MaxWalkSpeed = UltimateWalkSpeed;
	PlayRate = UltimatePlayRate;

	if (!bUltimateWithTick)
	{
		GetWorld()->GetTimerManager().SetTimer(UltimateTimerHandle, this, &ATM_Character::UpdateUltimateDurationWithTimer, 1.0f, true);
	}
}

void ATM_Character::UpdateUltimateDuration(float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);
	BP_UpdateUltimateDuration(Value);

	if (CurrentUltimateDuration == 0.0f)
	{
		bIsUsingUltimate = false;
		bCanSprint = true;

		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		PlayRate = 1.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShoot);

		if (!bUltimateWithTick)
		{
			GetWorld()->GetTimerManager().ClearTimer(UltimateTimerHandle);
		}
		CurrentUltimateXP = 0.0f;
		BP_StopUltimate();
	}
}

void ATM_Character::UpdateUltimateDurationWithTimer()
{
	UpdateUltimateDuration(UltimateFrequency);
}


void ATM_Character::StopUltimate()
{

}
//---------------------------------------------------------------------------------------------------------------------------------


/*
* |-----------------------------------------------------------------------------------------------------------------------|
* |											Character Teleport Ultimate													  |
* |-----------------------------------------------------------------------------------------------------------------------|
*/

void ATM_Character::StartTeleportUltimate()
{
	if (bCanUseUltimate || bIsUsingUltimate)
	{
		if (bCanUseTeleportUltimate)
		{
			if (CurrentTeleportUltimateProjectiles == 0)
			{
				CurrentUltimateDuration = MaxUltimateDuration;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehavior, this, &ATM_Character::BeginTeleportUltimateBehavior, UltimateFrequency, false);
			}

			if (bIsSprinting)
			{
				StopSprinting();
			}

			if (IsValid(MyAnimInstance) && IsValid(UltimateTeleportMontage))
			{
				const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateTeleportMontage);
				CurrentTeleportUltimateProjectiles++;
			}

			if (CurrentTeleportUltimateProjectiles > MaxTeleportUltimateProjectiles - 1)
			{
				bIsUsingUltimate = false;
				bCanUseUltimate = false;
				CurrentTeleportUltimateProjectiles = 0;
				CurrentUltimateDuration = 0.0f;
				UpdateUltimateDuration(0.0f);
				return;
			}

			BP_StartTeleportUltimate();
		}
	}

}

void ATM_Character::LaunchTeleportProjectile()
{
	FVector MuzzleSocketLocation = GetMesh()->GetSocketLocation(TeleportProjectileSocketName);
	FRotator MuzzleSocketRotation = GetMesh()->GetSocketRotation(TeleportProjectileSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<ATeleportProjectile>(TeleportProjectileClass, MuzzleSocketLocation, MuzzleSocketRotation, SpawnParams);

}


void ATM_Character::TeleportToProjectile(FVector TeleportVector, FRotator RotatorVector)
{
	const TArray<FName> AnimationNames = { "TeleportPlayer" };
	this->TeleportTo(TeleportVector, RotatorVector, false, false);
	const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateTeleportMontage, 3.0f);
	MyAnimInstance->Montage_JumpToSection(AnimationNames[0], UltimateTeleportMontage);
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

void ATM_Character::BeginTeleportUltimateBehavior()
{
	bIsUsingUltimate = true;
	if (!bUltimateWithTick)
	{
		GetWorld()->GetTimerManager().SetTimer(UltimateTimerHandle, this, &ATM_Character::UpdateTeleportUltimateDurationWithTimer, UltimateFrequency, true);
	}
}

void ATM_Character::UpdateTeleportUltimateDurationWithTimer()
{
	UpdateTeleportUltimateDuration(UltimateFrequency);
}

void ATM_Character::UpdateTeleportUltimateDuration(float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);
	BP_UpdateUltimateDuration(Value);

	if (CurrentUltimateDuration == 0.0f)
	{
		bIsUsingUltimate = false;
		bCanUseUltimate = false;
		CurrentTeleportUltimateProjectiles = 0;

		if (!bUltimateWithTick)
		{
			GetWorld()->GetTimerManager().ClearTimer(UltimateTimerHandle);
		}
		CurrentUltimateXP = 0.0f;
		BP_StopUltimate();
	}
}

void ATM_Character::SetCharacterSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
}

/*
* TODO: Update Timer with this ultimate. We are close :D 
*/


//---------------------------------------------------------------------------------------------------------------------------------


// Called every frame
void ATM_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUltimateWithTick && bIsUsingUltimate)
	{
		UpdateUltimateDuration(DeltaTime);
	}

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

	PlayerInputComponent->BindAction("RangedMelee", IE_Pressed, this, &ATM_Character::StartRangedMelee);
	//PlayerInputComponent->BindAction("RangedMelee", IE_Released, this, &ATM_Character::StopRangedMelee);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &ATM_Character::StartUltimate);
	PlayerInputComponent->BindAction("Ultimate", IE_Released, this, &ATM_Character::StopUltimate);

	PlayerInputComponent->BindAction("TeleportUltimate", IE_Pressed, this, &ATM_Character::StartTeleportUltimate);

	PlayerInputComponent->BindAction("TestButton", IE_Pressed, this, &ATM_Character::LaunchTeleportProjectile);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATM_Character::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATM_Character::StopSprinting);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ATM_Character::StartDashing);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &ATM_Character::StopDashing);

	PlayerInputComponent->BindAction("SetFireMode", IE_Pressed, this, &ATM_Character::SetFireMode);
}


