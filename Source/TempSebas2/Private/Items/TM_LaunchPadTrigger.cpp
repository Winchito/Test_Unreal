// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_LaunchPadTrigger.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "TM_LaunchPad.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TM_Character.h"
#include "Components/CapsuleComponent.h" 
#include "TempSebas2.h"

// Sets default values
ATM_LaunchPadTrigger::ATM_LaunchPadTrigger()
{

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	PadTriggerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadTriggerMesh"));
	PadTriggerMeshComponent->SetupAttachment(RootComponent);


	PadTriggerColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PadTriggerComponent"));
	PadTriggerColliderComponent->SetupAttachment(RootComponent);

	PadTriggerColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PadTriggerColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	PadTriggerColliderComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);

	//PadTriggerColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//PadTriggerColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//PadTriggerColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bIsLaunchTriggerActivated = false;
}

// Called when the game starts or when spawned
void ATM_LaunchPadTrigger::BeginPlay()
{
	Super::BeginPlay();

	//PadTriggerColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_LaunchPadTrigger::LaunchPadTrigger);

	//PadTriggerColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_LaunchPadTrigger::LaunchPadTrigger);
	
}

void ATM_LaunchPadTrigger::LaunchPadTrigger()
{
		//ACharacter* CharacterPlayerToTrigger = Cast<ACharacter>(OtherActor);
		//if (IsValid(CharacterPlayerToTrigger))
		//{
		//	SetLaunchPadTriggerState();
		//	ChangeMaterialColor(bIsLaunchTriggerActivated);
		//	for (ATM_LaunchPad* LaunchPad : LaunchPadCategories)
		//	{
		//		if (LaunchPad)
		//		{
		//			LaunchPad->TriggerLaunchPad(bIsLaunchTriggerActivated);
		//		}
		//	}
		//}

		SetLaunchPadTriggerState();
		ChangeMaterialColor(bIsLaunchTriggerActivated);
		for (ATM_LaunchPad* LaunchPad : LaunchPadCategories)
		{
			if (LaunchPad)
			{
				LaunchPad->TriggerLaunchPad(bIsLaunchTriggerActivated);
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

