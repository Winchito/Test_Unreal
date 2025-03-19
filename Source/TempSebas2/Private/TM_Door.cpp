// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_Door.h"
#include "Components\StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TM_Character.h"

// Sets default values
ATM_Door::ATM_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrameComponent-> SetupAttachment(CustomRootComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorComponent-> SetupAttachment(CustomRootComponent);

	KeyZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyZoneCollider"));
	KeyZoneColliderComponent->SetupAttachment(CustomRootComponent);
	KeyZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	KeyZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	KeyZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	OpenAngle = -90.0f;
	DoorTag = "Key_A";


}

// Called when the game starts or when spawned
void ATM_Door::BeginPlay()
{
	Super::BeginPlay();
	KeyZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATM_Door::CheckKeyFromPlayer);
	
}

void ATM_Door::CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bisOpen) {
		return;
	}

	if (IsValid(OtherActor)) 
	{
		ATM_Character* OverlappedCharacter = Cast<ATM_Character>(OtherActor);
		if(IsValid(OverlappedCharacter))
		{
			if (OverlappedCharacter->HasKey(DoorTag))
			{
				OpenDoor();
			}
		}
	}
}

// Called every frame
void ATM_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATM_Door::OpenDoor()
{
	//FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	//DoorComponent->SetRelativeRotation(NewRotation)
	bisOpen = true;
	BP_OpenDoor();

	
}

