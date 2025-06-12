// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ATM_LaunchPad::ATM_LaunchPad()
{
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	LaunchPadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaunchPadMesh"));
	LaunchPadMeshComponent->SetupAttachment(CustomRootComponent);

	LaunchPadColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyZoneCollider"));
	LaunchPadColliderComponent->SetupAttachment(CustomRootComponent);

	LaunchForce = FVector(4000, 4000, 4000);

}


// Called when the game starts or when spawned
void ATM_LaunchPad::BeginPlay()
{
	Super::BeginPlay();

	LaunchPadColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_LaunchPad::PlayerStepped);
	
}

void ATM_LaunchPad::PlayerStepped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActivated)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayerOverlapped!"));
	if (IsValid(OtherActor))
	{
		ACharacter* CharacterPlayerToLaunch = Cast<ACharacter>(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("Entered If 1"));
		if (IsValid(CharacterPlayerToLaunch))
		{
			UE_LOG(LogTemp, Warning, TEXT("Entered if 2"));
			CharacterPlayerToLaunch->LaunchCharacter(LaunchForce, true, true);
		}
	}
}

void ATM_LaunchPad::TriggerLaunchPad(bool LaunchPadAction)
{
	UE_LOG(LogTemp, Warning, TEXT("Etered triggerlaunchpad function"));
	bIsActivated = LaunchPadAction;
}


