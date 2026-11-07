// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Patrolling.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DemoAiCharacter.h"
#include "DemoAiController.h"
#include "Navigation/PathFollowingComponent.h"
#include "PatrolNode.h"
#include "PatrolPath.h"

#define PATROL_OBJECT_KEY TEXT("PatrolNodeActor")
#define PATROL_LOCATION_KEY TEXT("PatrolMoveRequestLocation")
#define PATROL_ARRIVAL_KEY TEXT("PatrolArrivalRadius")
#define PATROL_WAIT_KEY TEXT("PatrolWaitTime")
#define IS_AT_PATROL_NODE_KEY TEXT("IsAtPatrolNode")

UBTTask_Patrolling::UBTTask_Patrolling()
{
	NodeName = "Patrol Task";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_Patrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ADemoAiController* const DemoAIController = CastChecked<ADemoAiController>(OwnerComp.GetAIOwner());
	const ADemoAiCharacter* const DemoAICharacter = DemoAIController->GetDemoAICharacter();
	if (!DemoAICharacter)
	{
		return EBTNodeResult::Failed;
	}

	// Handle Request -> Result
	const FPathFollowingRequestResult MoveResult = MoveToCurrentPatrolNode(OwnerComp);
	switch (MoveResult.Code)
	{
		case EPathFollowingRequestResult::RequestSuccessful:
			WaitForMessage(OwnerComp, UBrainComponent::AIMessage_MoveFinished, MoveResult.MoveId);
			break;

		case EPathFollowingRequestResult::AlreadyAtGoal:
			return EBTNodeResult::Succeeded;

		default:
			return EBTNodeResult::Failed;
	}

	// Update Memory now that task is in progress
	FBTPatrolTaskMemory* MyMemory = CastInstanceNodeMemory<FBTPatrolTaskMemory>(NodeMemory);
	check(MyMemory);
	MyMemory->MoveRequestID = MoveResult.MoveId;

	// Update node in memory and the wait time at the node
	UBlackboardComponent* const BlackboardComponent = OwnerComp.GetBlackboardComponent();
	check(BlackboardComponent);
	const APatrolNode* const CurrentPatrolNode = Cast<APatrolNode>(BlackboardComponent->GetValueAsObject(PATROL_OBJECT_KEY));
	if (CurrentPatrolNode)
	{
		MyMemory->CurrentPatrolNodePtr = CurrentPatrolNode;
		BlackboardComponent->SetValueAsFloat(PATROL_WAIT_KEY, FMath::FRandRange(CurrentPatrolNode->GetMinWaitTime(), CurrentPatrolNode->GetMaxWaitTime()));
	}
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Patrolling::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* const AIController = OwnerComp.GetAIOwner())
	{
		AIController->StopMovement();
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_Patrolling::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FPathFollowingRequestResult ResultData;
	FBTPatrolTaskMemory* MyMemory = CastInstanceNodeMemory<FBTPatrolTaskMemory>(NodeMemory);
	check(MyMemory);
	MyMemory->MoveRequestID = ResultData;
	MyMemory->CurrentPatrolNodePtr.Reset();
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_Patrolling::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
	UBlackboardComponent* const BlackboardComponent = OwnerComp.GetBlackboardComponent();
	check(BlackboardComponent);
	BlackboardComponent->SetValueAsBool(IS_AT_PATROL_NODE_KEY, bSuccess);

	if (bSuccess)
	{
		AAIController* const AIController = OwnerComp.GetAIOwner();
		const ADemoAiController* const DemoAIController = Cast<ADemoAiController>(AIController);
		const ADemoAiCharacter* const DemoAICharacter = DemoAIController ? DemoAIController->GetDemoAICharacter() : nullptr;
		if (DemoAICharacter)
		{
			FBTPatrolTaskMemory* MyMemory = CastInstanceNodeMemory<FBTPatrolTaskMemory>(NodeMemory);
			check(MyMemory);

			const APatrolNode* const CurrentPatrolNodeRaw = MyMemory->CurrentPatrolNodePtr.Get();
			APatrolNode* const NextPatrolNode = CurrentPatrolNodeRaw ? CurrentPatrolNodeRaw->GetNextNode() : nullptr;
			if (NextPatrolNode)
			{
				BlackboardComponent->SetValueAsObject(PATROL_OBJECT_KEY, NextPatrolNode);
				BlackboardComponent->SetValueAsVector(PATROL_LOCATION_KEY, NextPatrolNode->GetActorLocation());
				BlackboardComponent->SetValueAsFloat(PATROL_ARRIVAL_KEY, NextPatrolNode->GetArrivalRadius());
			}
		}
	}

	Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}

FPathFollowingRequestResult UBTTask_Patrolling::MoveToCurrentPatrolNode(const UBehaviorTreeComponent& OwnerComp)
{
	FPathFollowingRequestResult ResultData;

	AAIController* const AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return ResultData;
	}

	const UBlackboardComponent* const BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return ResultData;
	}

	const FVector CurrentPatrolLocation = BlackboardComponent->GetValueAsVector(PATROL_LOCATION_KEY);
	const float CurrentPatrolArrivalRadius = BlackboardComponent->GetValueAsFloat(PATROL_ARRIVAL_KEY);

	// Set Move Request
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(CurrentPatrolLocation);
	MoveRequest.SetAcceptanceRadius(CurrentPatrolArrivalRadius);
	MoveRequest.SetAllowPartialPath(true);
	MoveRequest.SetReachTestIncludesAgentRadius(false);

	// Handle Request -> Result
	return AIController->MoveTo(MoveRequest);
}

uint16 UBTTask_Patrolling::GetInstanceMemorySize() const
{
	return sizeof(FBTPatrolTaskMemory);
}

void UBTTask_Patrolling::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTPatrolTaskMemory>(NodeMemory, InitType);
}

void UBTTask_Patrolling::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTPatrolTaskMemory>(NodeMemory, CleanupType);
}