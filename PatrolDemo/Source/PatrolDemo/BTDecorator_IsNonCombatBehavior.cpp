// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsNonCombatBehavior.h"
#include "DemoAiCharacter.h"
#include "DemoAiController.h"

UBTDecorator_IsNonCombatBehavior::UBTDecorator_IsNonCombatBehavior()
{
	NodeName = "Is Non-Combat Behavior";
	bNotifyTick = false;
	bCreateNodeInstance = false;
}

bool UBTDecorator_IsNonCombatBehavior::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ADemoAiController* const DemoAIController = CastChecked<ADemoAiController>(OwnerComp.GetAIOwner());
	const ADemoAiCharacter* const DemoAICharacter = DemoAIController->GetDemoAICharacter();
	if (!DemoAICharacter)
	{
		return false;
	}

	const ENonCombatBehaviorType CurrentNonCombatBehavior = DemoAICharacter->GetNonCombatBehaviorType();
	return CurrentNonCombatBehavior == NonCombatBehavior;
}
