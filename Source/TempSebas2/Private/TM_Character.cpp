// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Character.h"
#include "Camera\CameraComponent.h"

// Sets default values
ATM_Character::ATM_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraSocketName = "SCK_Camera";

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent-> bUsePawnControlRotation = true;
	FPSCameraComponent-> SetupAttachment(GetMesh(), FPSCameraSocketName);

}

// Called when the game starts or when spawned
void ATM_Character::BeginPlay()
{
	Super::BeginPlay();
	
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

void ATM_Character::AddControllerPitchInput(float value)
{
	//bIsLookInversion ? Super::AddControllerPitchInput(-value) : Super::AddControllerPitchInput(value);
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
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
}


