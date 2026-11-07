// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_UpdateRootTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DemoAiCharacter.h"
#include "DemoAiController.h"
#include "DemoPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

#define START_LOCATION_KEY TEXT("StartingLocation")
#define FEET_START_LOCATION_KEY TEXT("FeetStartingLocation")
#define PLAYER_KEY TEXT("PlayerActor")
#define PLAYER_LOCATION_KEY TEXT("PlayerLocation")
#define PLAYER_DETECTED_LOCATION_KEY TEXT("DetectedPlayerLocation")
#define TRANSITION_TO_NONCOMBAT TEXT("TransitioningToNonCombat")

UBTService_UpdateRootTree::UBTService_UpdateRootTree()
{
	Interval = 0.1f;
	RandomDeviation = 0.0f;
	NodeName = "Update Root Keys";
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_UpdateRootTree::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// Blackboard and Memory
	UBlackboardComponent* const BlackboardComponent = OwnerComp.GetBlackboardComponent();
	check(BlackboardComponent);
	FBTRootTreeServiceMemory* MyMemory = CastInstanceNodeMemory<FBTRootTreeServiceMemory>(NodeMemory);
	check(MyMemory);

	// Setup NPC Controller/Character Keys
	ADemoAiController* const DemoAIController = CastChecked<ADemoAiController>(OwnerComp.GetAIOwner());
	const ADemoAiCharacter* const DemoAICharacter = DemoAIController->GetDemoAICharacter();
	if (ensure(DemoAICharacter))
	{
		BlackboardComponent->SetValueAsVector(START_LOCATION_KEY, DemoAICharacter->GetActorLocation());
		BlackboardComponent->SetValueAsVector(FEET_START_LOCATION_KEY, DemoAICharacter->GetCharacterFeetLocation());
		MyMemory->DemoAICharacterPtr = DemoAICharacter;
	}

	// Setup Player Keys
	const UWorld* const World = GetWorld();
	check(World);
	ADemoPlayerCharacter* const DemoPlayerCharacter = Cast<ADemoPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (ensure(DemoPlayerCharacter))
	{
		BlackboardComponent->SetValueAsObject(PLAYER_KEY, DemoPlayerCharacter);
		BlackboardComponent->SetValueAsVector(PLAYER_LOCATION_KEY, DemoPlayerCharacter->GetActorLocation());
		MyMemory->DemoPlayerCharacterPtr = DemoPlayerCharacter;
	}
}

void UBTService_UpdateRootTree::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	FBTRootTreeServiceMemory* MyMemory = CastInstanceNodeMemory<FBTRootTreeServiceMemory>(NodeMemory);
	check(MyMemory);
	MyMemory->DemoAICharacterPtr.Reset();
	MyMemory->DemoPlayerCharacterPtr.Reset();
}

void UBTService_UpdateRootTree::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Update Player Location
	FBTRootTreeServiceMemory* MyMemory = CastInstanceNodeMemory<FBTRootTreeServiceMemory>(NodeMemory);
	check(MyMemory);
	const ADemoAiCharacter* const DemoAICharacterRaw = MyMemory->DemoAICharacterPtr.Get();
	if (!DemoAICharacterRaw)
	{
		return;
	}

	const ADemoPlayerCharacter* const DemoPlayerCharacterRaw = MyMemory->DemoPlayerCharacterPtr.Get();
	if (!DemoPlayerCharacterRaw)
	{
		return;
	}

	// Update Player Location always
	UBlackboardComponent* const BlackboardComponent = OwnerComp.GetBlackboardComponent();
	check(BlackboardComponent);
	BlackboardComponent->SetValueAsVector(PLAYER_LOCATION_KEY, DemoPlayerCharacterRaw->GetActorLocation());

	// Only updated detected location for sight purposes
	if (DemoAICharacterRaw->CanSeeCharacter(DemoPlayerCharacterRaw))
	{
		BlackboardComponent->SetValueAsVector(PLAYER_DETECTED_LOCATION_KEY, DemoPlayerCharacterRaw->GetActorLocation());
	}
	else
	{
		// Transition to Non-Combat if no longer seen
		BlackboardComponent->ClearValue(PLAYER_DETECTED_LOCATION_KEY);
		BlackboardComponent->SetValueAsBool(TRANSITION_TO_NONCOMBAT, true);
	}
}

uint16 UBTService_UpdateRootTree::GetInstanceMemorySize() const
{
	return sizeof(FBTRootTreeServiceMemory);
}

void UBTService_UpdateRootTree::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTRootTreeServiceMemory>(NodeMemory, InitType);
}

void UBTService_UpdateRootTree::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTRootTreeServiceMemory>(NodeMemory, CleanupType);
}
