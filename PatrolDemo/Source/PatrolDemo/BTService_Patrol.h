// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class PATROLDEMO_API UBTService_Patrol : public UBTService
{
	GENERATED_BODY()
	
	UBTService_Patrol();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
