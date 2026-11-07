// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Patrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "DemoAiCharacter.h"
#include "DemoAiController.h"
#include "PatrolNode.h"
#include "PatrolPath.h"

#define PATROL_OBJECT_KEY TEXT("PatrolNodeActor")
#define PATROL_LOCATION_KEY TEXT("PatrolMoveRequestLocation")
#define PATROL_ARRIVAL_KEY TEXT("PatrolArrivalRadius")
#define PATROL_WAIT_KEY TEXT("PatrolWaitTime")
#define IS_AT_PATROL_NODE_KEY TEXT("IsAtPatrolNode")

UBTService_Patrol::UBTService_Patrol()
{
	NodeName = "Patrol Behavior Service";
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_Patrol::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const ADemoAiController* const DemoAIController = CastChecked<ADemoAiController>(OwnerComp.GetAIOwner());
	ADemoAiCharacter* const DemoAICharacter = DemoAIController->GetDemoAICharacter();
	if (!DemoAICharacter)
	{
		return;
	}

	const APatrolPath* const PatrolPath = DemoAICharacter->GetPatrolPath();
	if (!PatrolPath)
	{
		return;
	}

	APatrolNode* const FirstPatrolNode = PatrolPath->GetFirstNodeInPath();
	if (!FirstPatrolNode)
	{
		return;
	}

	// Enable Patrol State
	DemoAICharacter->SetIsInPatrol(true);

	UBlackboardComponent* const BlackboardComponent = OwnerComp.GetBlackboardComponent();
	check(BlackboardComponent);
	BlackboardComponent->SetValueAsObject(PATROL_OBJECT_KEY, FirstPatrolNode);
	BlackboardComponent->SetValueAsVector(PATROL_LOCATION_KEY, FirstPatrolNode->GetActorLocation());
	BlackboardComponent->SetValueAsFloat(PATROL_ARRIVAL_KEY, FirstPatrolNode->GetArrivalRadius());
	BlackboardComponent->SetValueAsFloat(PATROL_WAIT_KEY, FMath::FRandRange(FirstPatrolNode->GetMinWaitTime(), FirstPatrolNode->GetMaxWaitTime()));
}

void UBTService_Patrol::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	const ADemoAiController* const DemoAIController = CastChecked<ADemoAiController>(OwnerComp.GetAIOwner());
	ADemoAiCharacter* const DemoAICharacter = DemoAIController->GetDemoAICharacter();
	if (!DemoAICharacter)
	{
		return;
	}

	// Disable Patrol State
	DemoAICharacter->SetIsInPatrol(false);
}
