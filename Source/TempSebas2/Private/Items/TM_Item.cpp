// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TM_Item.h"
#include "Components/SphereComponent.h"
#include "TM_Character.h"

// Sets default values
ATM_Item::ATM_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = MainColliderComponent;



}

// Called when the game starts or when spawned
void ATM_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATM_Item::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor)) 
	{
		ATM_Character* OverlappedCharacter = Cast<ATM_Character>(OtherActor);

		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			Pickup(OverlappedCharacter);
		}
	}


}

void ATM_Item::Pickup(ATM_Character* PickupCharacter)
{
	BP_Pickup(PickupCharacter);
}

// Called every frame
void ATM_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

