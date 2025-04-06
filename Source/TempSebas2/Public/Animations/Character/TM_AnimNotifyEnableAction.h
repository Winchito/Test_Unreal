// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TM_AnimNotifyEnableAction.generated.h"

/**
 * 
 */
UCLASS()
class TEMPSEBAS2_API UTM_AnimNotifyEnableAction : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
