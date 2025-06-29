// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_AnimNotifyStateMelee.h"
#include "TM_Character.h"
#include "Components/CapsuleComponent.h"

void UTM_AnimNotifyStateMelee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ATM_Character* Character = Cast<ATM_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::QueryOnly);
		}
	}
}

void UTM_AnimNotifyStateMelee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ATM_Character* Character = Cast<ATM_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::NoCollision);
		}
	}
}
