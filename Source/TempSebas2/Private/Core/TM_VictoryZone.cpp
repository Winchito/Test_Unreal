// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_VictoryZone.h"
#include "Components/BoxComponent.h"
#include "TM_Character.h"
#include "Core/TM_GameMode.h"

// Sets default values
ATM_VictoryZone::ATM_VictoryZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VictoryZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VictoryZoneComponent"));
	RootComponent = VictoryZoneComponent;

	VictoryZoneComponent->SetBoxExtent(FVector(100.0f));
}

// Called when the game starts or when spawned
void ATM_VictoryZone::BeginPlay()
{
	Super::BeginPlay();
	

	GameModeReference = Cast<ATM_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ATM_VictoryZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATM_VictoryZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (IsValid(OtherActor) && IsValid(GameModeReference))
	{
		ATM_Character* TMCharacter = Cast<ATM_Character>(OtherActor);
		if (IsValid(TMCharacter))
		{
			GameModeReference->Victory(TMCharacter);
		}
	}
}

