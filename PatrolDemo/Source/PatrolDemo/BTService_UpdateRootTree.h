// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateRootTree.generated.h"

class ADemoAiCharacter;
class ADemoAiController;
class ADemoPlayerCharacter;

struct FBTRootTreeServiceMemory
{
	/** Weak Tracking the Player for this tree */
	TWeakObjectPtr<const ADemoPlayerCharacter> DemoPlayerCharacterPtr;

	/** Weak Tracking our AI Character for this tree */
	TWeakObjectPtr<const ADemoAiCharacter> DemoAICharacterPtr;
};

/**
 * 
 */
UCLASS()
class PATROLDEMO_API UBTService_UpdateRootTree : public UBTService
{
	GENERATED_BODY()

	UBTService_UpdateRootTree();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	virtual uint16 GetInstanceMemorySize() const override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
};
