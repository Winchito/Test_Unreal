// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Fire.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "TM_Character.h"


// Sets default values
ATM_Fire::ATM_Fire()
{
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	//FireMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fire"));
	//FireMeshComponent->SetupAttachment(CustomRootComponent);

	FireColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("FireCollision"));
	FireColliderComponent->SetupAttachment(CustomRootComponent);
	FireColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FireColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	FireColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	FireDurationForPlayer = 5.0f;
	FireDuration = 5.0f;

}

// Called when the game starts or when spawned
void ATM_Fire::BeginPlay()
{
	Super::BeginPlay();

	FireColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_Fire::EnterFireZone);
	FireColliderComponent->OnComponentEndOverlap.AddDynamic(this, &ATM_Fire::LeaveFireZone);

	if (FireAreaEffect)
	{
		FireSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireAreaEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandler_TimeOnFire, this, &ATM_Fire::EndFire, FireDuration, false);
}


void ATM_Fire::EnterFireZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered Fire!"));

	if (ATM_Character* PossibleCharacter = Cast<ATM_Character>(OtherActor))
	{
		PossibleCharacter->SetPlayerOnFire(FireDurationForPlayer);
	}
}

void ATM_Fire::LeaveFireZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Leaved Fire!"));
}

void ATM_Fire::EndFire()
{
	if (FireSystemComponent && FireSystemComponent->IsActive())
	{
		FireSystemComponent->DeactivateSystem();
	}

	Destroy();
}

