// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BillboardComponent.h"
#include "TM_Character.h"
#include "TM_Enemy.h"
#include "Controller/TM_AIController.h"
#include "NavigationSystem.h"
// Sets default values
ATM_EnemySpawner::ATM_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("PathBillboard"));
	RootComponent = SpawnerBillboardComponent;

	BoxComponentCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyTriggerComponent"));
	BoxComponentCollider->SetupAttachment(RootComponent);

	bIsActive = true;
	MaxEnemyCounter = 1;
	TimeToSpawn = 1.0f;


}

// Called when the game starts or when spawned
void ATM_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ATM_EnemySpawner::SpawnEnemy, TimeToSpawn, true);

	BoxComponentCollider->OnComponentBeginOverlap.AddDynamic(this, &ATM_EnemySpawner::OnPlayerEntering);
	
}

void ATM_EnemySpawner::SpawnEnemy()
{
	if (!bIsActive)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Current bot counter:  %i"), CurrentEnemyCounter);

	if (CurrentEnemyCounter >= MaxEnemyCounter)
	{
		return;
	}

	if (IsValid(EnemyClass))
	{
		FVector LocalSpawnPoint = GetSpawnPoint();
		FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform EnemyTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);

		ATM_Enemy* NewEnemy = GetWorld()->SpawnActorDeferred<ATM_Enemy>(EnemyClass, EnemyTransform, nullptr, nullptr, SpawnParameters.SpawnCollisionHandlingOverride);
		if (IsValid(NewEnemy))
		{
			//NewEnemy->AIControllerClass = ATM_AIController::StaticClass();
			NewEnemy->SetSpawner(this);
		}

		NewEnemy->FinishSpawning(EnemyTransform);

		NewEnemy->SpawnDefaultController();
		CurrentEnemyCounter++;
	}
}

FVector ATM_EnemySpawner::GetSpawnPoint()
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

void ATM_EnemySpawner::OnPlayerEntering(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ATM_EnemySpawner::NotifyEnemyDead()
{
	CurrentEnemyCounter--;
}

void ATM_EnemySpawner::DisableSpawn()
{
	bIsActive = false;
	BoxComponentCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}