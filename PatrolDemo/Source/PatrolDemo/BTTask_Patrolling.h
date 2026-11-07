// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Patrolling.generated.h"

class APatrolNode;
struct FPathFollowingRequestResult;

struct FBTPatrolTaskMemory
{
	/** Move request ID */
	FAIRequestID MoveRequestID;

	/** Weak patrol node tracker */
	TWeakObjectPtr<const APatrolNode> CurrentPatrolNodePtr;
};

/**
 *	Handles Patrol Movement for the Patrol Behavior Tree
 */
UCLASS()
class PATROLDEMO_API UBTTask_Patrolling : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Patrolling();

public:
	FPathFollowingRequestResult MoveToCurrentPatrolNode(const UBehaviorTreeComponent& OwnerComp);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;

private:
	virtual uint16 GetInstanceMemorySize() const override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
};
