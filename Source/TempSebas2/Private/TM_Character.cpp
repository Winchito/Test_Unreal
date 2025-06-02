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
#include "Components/TM_HealthComponent.h"
#include "Core/TM_GameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"


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
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UTM_HealthComponent>(TEXT("HealthComponent"));

	bUltimateWithTick = true;
	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 10.0f;
	UltimateFrequency = 0.5f;

	UltimateWalkSpeed = 2000.0f;
	UltimatePlayRate = 2.0f;
	PlayRate = 1.0f;
	UltimateShotFrequency = 0.1f;
	
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

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ATM_Character::OnHealthChange);

	NormalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void ATM_Character::InitializeReferences()
{
	if (IsValid(GetMesh())) 
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<ATM_GameMode>(GetWorld()->GetAuthGameMode());
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
	if (bIsSprinting) {
		return;
	}
	if (IsValid(CurrentWeapon)) {
		CurrentWeapon->StartAction();

		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutomaticShoot, CurrentWeapon, &ATM_Weapon::StartAction, UltimateShotFrequency, true);
		}
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

		if (bIsUsingUltimate)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShoot);
		}
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
		MyAnimInstance->Montage_Play(MeleeMontage, PlayRate);
	}

	SetActionsState(true);

}

void ATM_Character::StopMelee()
{
	//UE_LOG(LogTemp, Warning, TEXT("Player stops melee action"));
}

void ATM_Character::StartUltimate()
{

	if (bCanUseUltimate && !bIsUsingUltimate)
	{ 

		CurrentUltimateDuration = MaxUltimateDuration;
		bCanUseUltimate = false;

		if (bIsSprinting)
		{
			//GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			StopSprinting();
			//const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateMontage);
			//GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehavior, this, &ATM_Character::BeginUltimateBehavior, StartUltimateMontageDuration, false);
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
		GetWorld()->GetTimerManager().SetTimer(UltimateTimerHandle, this, &ATM_Character::UpdateUltimateDurationWithTimer, UltimateFrequency, true);
	}
}

void ATM_Character::StopUltimate()
{

}

void ATM_Character::StartSprinting()
{
	if (bIsUsingUltimate)
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

void ATM_Character::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor == this) {
			return;
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

void ATM_Character::AddControllerPitchInput(float value)
{
	//bIsLookInversion ? Super::AddControllerPitchInput(-value) : Super::AddControllerPitchInput(value);
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}

void ATM_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool ATM_Character::TryAddHealth(float HealthToAdd)
{
	return HealthComponent->TryAddHealth(HealthToAdd);
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

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &ATM_Character::StartUltimate);
	PlayerInputComponent->BindAction("Ultimate", IE_Released, this, &ATM_Character::StopUltimate);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATM_Character::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATM_Character::StopSprinting);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ATM_Character::StartDashing);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &ATM_Character::StopDashing);
}


