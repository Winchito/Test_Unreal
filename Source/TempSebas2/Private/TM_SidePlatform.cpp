// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_SidePlatform.h"
#include "Components\StaticMeshComponent.h"
#include "Kismet\KismetMathLibrary.h"
#include "TimerManager.h"

// Sets default values
ATM_SidePlatform::ATM_SidePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMeshComponent->SetupAttachment(CustomRootComponent);

	bIsGoingForward = true;
	Speed = 10.0f;
	MinDistance = 0.0f;
	MaxDistance = 50.0f;
	waitTime = 2.0f;
	currentTime = 0.0f;
	isMaxDistance = false;
}

// Called when the game starts or when spawned
void ATM_SidePlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATM_SidePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Move(DeltaTime);


}

void ATM_SidePlatform::Move(float DeltaTime)
{
	FVector forwardDirection = UKismetMathLibrary::GetForwardVector(GetActorRotation());

	if (currentTime >= waitTime)
	{
		isMaxDistance = false;
		currentTime = 0.0f;
		return;
	}

	if (isMaxDistance) {
		currentTime += DeltaTime;
		return;
	}

	if (bIsGoingForward) {
		forwardDirection = forwardDirection;
	}
	else {
		forwardDirection = forwardDirection * -1;
	}

	FVector CurrentVelocity = forwardDirection * Speed;
	FVector NewPosition = GetActorLocation() + CurrentVelocity;
	SetActorLocation(NewPosition);

	if (bIsGoingForward && GetActorLocation().X >= MaxDistance || !bIsGoingForward && GetActorLocation().X <= MinDistance) {
		bIsGoingForward = !bIsGoingForward;
		isMaxDistance = true;
	}
}


