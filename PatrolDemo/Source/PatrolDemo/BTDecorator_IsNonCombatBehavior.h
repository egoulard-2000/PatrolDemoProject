// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsNonCombatBehavior.generated.h"

enum class ENonCombatBehaviorType : uint8;

/**
 * Checks Non-Combat Behavior for the BT
 */
UCLASS()
class PATROLDEMO_API UBTDecorator_IsNonCombatBehavior : public UBTDecorator
{
	GENERATED_BODY()
	
	UBTDecorator_IsNonCombatBehavior();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	// Blackboard key selector for the actor to check for the gameplay tag query
	UPROPERTY(EditAnywhere, Category = "Non Combat Behavior Type")
	ENonCombatBehaviorType NonCombatBehavior;
};
