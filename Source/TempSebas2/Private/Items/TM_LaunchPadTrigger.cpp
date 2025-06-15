// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_LaunchPadTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "TM_LaunchPad.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ATM_LaunchPadTrigger::ATM_LaunchPadTrigger()
{

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	PadTriggerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadTriggerMesh"));
	PadTriggerMeshComponent->SetupAttachment(RootComponent);


	PadTriggerColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PadTriggerComponent"));
	PadTriggerColliderComponent->SetupAttachment(RootComponent);
	PadTriggerColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PadTriggerColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	PadTriggerColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bIsLaunchTriggerActivated = false;
}

// Called when the game starts or when spawned
void ATM_LaunchPadTrigger::BeginPlay()
{
	Super::BeginPlay();

	PadTriggerColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_LaunchPadTrigger::ActivateLaunchPad);
	
}

void ATM_LaunchPadTrigger::ActivateLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		ACharacter* CharacterPlayerToTrigger = Cast<ACharacter>(OtherActor);
		if (IsValid(CharacterPlayerToTrigger))
		{
			bIsLaunchTriggerActivated = !bIsLaunchTriggerActivated;
			ChangeMaterialColor(bIsLaunchTriggerActivated);
			for (ATM_LaunchPad* LaunchPad : LaunchPadCategories)
			{
				if (LaunchPad)
				{
					LaunchPad->TriggerLaunchPad(bIsLaunchTriggerActivated);
				}
			}
		}
	}
}

void ATM_LaunchPadTrigger::ChangeMaterialColor(bool ColorTrigger)
{
	if (ColorTrigger)
	{
		PadTriggerMeshComponent->SetMaterial(0, WidgetMaterial_Y);
	}
	else
	{
		PadTriggerMeshComponent->SetMaterial(0, WidgetMaterial_X);
	}
}

