// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TM_Item.generated.h"

class USphereComponent;
class ATM_Character;

UCLASS()
class TEMPSEBAS2_API ATM_Item : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* MainColliderComponent;
	
public:	

	// Sets default values for this actor's properties
	ATM_Item();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Pickup(ATM_Character* PickupCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item")
	void BP_Pickup(ATM_Character* PickupCharacter);

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
