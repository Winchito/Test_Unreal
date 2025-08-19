// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TM_BotSpawner.h"
#include "Components/BillboardComponent.h"
#include "Enemy/TM_Bot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "TM_Character.h"

// Sets default values
ATM_BotSpawner::ATM_BotSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("PathBillboard"));
	RootComponent = SpawnerBillboardComponent;

	BoxComponentCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PadTriggerComponent"));
	BoxComponentCollider->SetupAttachment(RootComponent);

	bIsActive = true;
	MaxBotCounter = 1;
	TimeToSpawn = 1.0f;

}

// Called when the game starts or when spawned
void ATM_BotSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ATM_BotSpawner::SpawnBot, TimeToSpawn, true);

	BoxComponentCollider->OnComponentBeginOverlap.AddDynamic(this, &ATM_BotSpawner::OnPlayerEntering);

}

void ATM_BotSpawner::SpawnBot()
{
	if (!bIsActive)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Current bot counter:  %i"), CurrentBotsCounter);

	if (CurrentBotsCounter >= MaxBotCounter)
	{
		return;
	}

	if (IsValid(BotClass))
	{
		FVector LocalSpawnPoint = GetSpawnPoint();
		FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform BotTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);

		ATM_Bot* NewBot = GetWorld()->SpawnActorDeferred<ATM_Bot>(BotClass, BotTransform, nullptr, nullptr, SpawnParameters.SpawnCollisionHandlingOverride);
		if (IsValid(NewBot))
		{
			NewBot->SetSpawner(this);
		}

		NewBot->FinishSpawning(BotTransform);

		CurrentBotsCounter++;
	}
}

FVector ATM_BotSpawner::GetSpawnPoint()
{
	if (SpawnPoints.Num() > 0)
	{
		int IndexSelected = FMath::RandRange(0, SpawnPoints.Num() - 1);
		return SpawnPoints[IndexSelected];
	}
	else
	{
		return GetActorLocation();
	}
}

void ATM_BotSpawner::OnPlayerEntering(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActive)
	{
		return;
	}

	if (ATM_Character* PossibleCharacter = Cast<ATM_Character>(OtherActor))
	{
		SetSpawnerState(true);
	}
}

void ATM_BotSpawner::NotifyBotDead()
{
	CurrentBotsCounter--;
}

void ATM_BotSpawner::DisableSpawn()
{
	bIsActive = false;
	BoxComponentCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

