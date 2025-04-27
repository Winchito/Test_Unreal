// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_ActorRoutePaths.h"
#include "Components/BillboardComponent.h"


// Sets default values
ATM_ActorRoutePaths::ATM_ActorRoutePaths()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PathBillboardComponent = CreateAbstractDefaultSubobject<UBillboardComponent>(TEXT("PathBillboard"));
	RootComponent = PathBillboardComponent;

}

// Called when the game starts or when spawned
void ATM_ActorRoutePaths::BeginPlay()
{
	Super::BeginPlay();
	
}


