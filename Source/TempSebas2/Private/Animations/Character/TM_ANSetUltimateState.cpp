// Fill out your copyright notice in the Description page of Project Settings.


#include "TM_ANSetUltimateState.h"
#include "TM_Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

void UTM_ANSetUltimateState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		ATM_Character* Character = Cast<ATM_Character>(CharacterActor);
		if (IsValid(Character))
		{
			UAnimInstance* UltimateMontage = Character->GetAnimInstance();
			UltimateMontage->Montage_Stop(0.0f);
			Character->SetCharacterSpeed();
		}
	}
}
