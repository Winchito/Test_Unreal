// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_AnimNotifyLaunchFireball.h"
#include "TM_Character.h"

void UTM_AnimNotifyLaunchFireball::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ATM_Character* Character = Cast<ATM_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->LaunchFireball();
		}
	}
}
