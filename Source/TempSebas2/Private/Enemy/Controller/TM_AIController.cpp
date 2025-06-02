// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Controller/TM_AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "TM_Enemy.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"

ATM_AIController::ATM_AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	LoopPathParameterName = "bLoopPath";
	DirectionIndexParameterName = "DirectionIndex";
	WaitingTimeParameterName   = "WaitingOnPathPoint";
	CanSeePlayerParameterName = "bCanSeePlayer";
	InvestigatingParameterName = "bIsInvestigating";
	TargetLocationParameterName = "TargetLocation";
}

void ATM_AIController::BeginPlay()
{
	Super::BeginPlay();


	if (IsValid(EnemyBehaviorTree))
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}
	

	MyEnemy = Cast<ATM_Enemy>(K2_GetPawn());
	if (IsValid(MyEnemy))
	{
		MyBlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(this);
	}

	if (IsValid(MyBlackBoard))
	{
		MyBlackBoard->SetValueAsBool(LoopPathParameterName, MyEnemy->GetLoopPath());
		MyBlackBoard->SetValueAsInt(DirectionIndexParameterName, MyEnemy->GetDirectionIndex());
		MyBlackBoard->SetValueAsFloat(WaitingTimeParameterName, MyEnemy->GetWaitingTime());
	}

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ATM_AIController::UpdateSenses);
}

void ATM_AIController::UpdateSenses(const TArray<AActor*>& UpdatedActors)
{
	if (!IsValid(MyBlackBoard))
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

		ATM_Character* SensedCharacter = Cast<ATM_Character>(Actor);
		//ATM_Character* SensedCharacter = Cast<ATM_Character>(PerceptionInfo.Target);

		if (IsValid(SensedCharacter) && SensedCharacter->GetCharacterType() == ETM_CharacterType::CharacterType_Player)
		{
			for (int arrayIndex = 0; arrayIndex < PerceptionInfo.LastSensedStimuli.Num(); arrayIndex++)
			{
				switch (arrayIndex)
				{
				case 0:
					MyBlackBoard->SetValueAsBool(CanSeePlayerParameterName, PerceptionInfo.LastSensedStimuli[arrayIndex].WasSuccessfullySensed());
					break;
				case 1:
					MyBlackBoard->SetValueAsBool(InvestigatingParameterName, bReceivingDamage);
					if (bReceivingDamage)
					{
						MyBlackBoard->SetValueAsVector(TargetLocationParameterName, PerceptionInfo.LastSensedStimuli[arrayIndex].StimulusLocation);
					}
					break;
				default:
					break;
				}
			}
		}
	}
}
