// Custom Gameplay Debugger category for Patrol AI

#include "GameplayDebuggerCategory_Patrol.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "DemoAiCharacter.h"
#include "GameplayDebuggerTypes.h"
#include "PatrolNode.h"
#include "PatrolPath.h"

FGameplayDebuggerCategory_Patrol::FGameplayDebuggerCategory_Patrol()
{
	// SetDataPackReplication<FPatrolData>(&Data);
}

void FGameplayDebuggerCategory_Patrol::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	const APawn* const DebugPawn = Cast<APawn>(DebugActor);
	const AAIController* const DebugController = DebugPawn ? Cast<AAIController>(DebugPawn->GetController()) : nullptr;
	const UBrainComponent* const BrainComponent = GetValid(DebugController ? DebugController->GetBrainComponent() : nullptr);
	const UBlackboardComponent* const BlackboardComponent = BrainComponent ? BrainComponent->GetBlackboardComponent() : nullptr;

	// Set Current Patrol and Path
	const APatrolNode* const CurrentPatrolNode = BlackboardComponent ? Cast<APatrolNode>(BlackboardComponent->GetValueAsObject(TEXT("PatrolNodeActor"))) : nullptr;
	Data.CurrentPatrolNodePtr = CurrentPatrolNode;
	const ADemoAiCharacter* const DemoAICharacter = Cast<ADemoAiCharacter>(DebugPawn);
	Data.CurrentPatrolPathPtr = DemoAICharacter ? DemoAICharacter->GetPatrolPath() : nullptr;

	// Debug Draw Points and Lines for Path
	if (DemoAICharacter && CurrentPatrolNode)
	{
		constexpr float PointRadius = 20.f;
		const FVector ZOffset = FVector::UpVector * 10.f;
		const FVector CharacterPointLocation = DemoAICharacter->GetCharacterFeetLocation() + ZOffset;
		AddShape(FGameplayDebuggerShape::MakePoint(CharacterPointLocation, PointRadius, FColor::Green));

		constexpr float LineThickness = 10.f;
		const FVector PatrolNodePointLocation = CurrentPatrolNode->GetActorLocation() + ZOffset;
		AddShape(FGameplayDebuggerShape::MakeSegment(CharacterPointLocation, PatrolNodePointLocation, LineThickness, FColor::Cyan));
		AddShape(FGameplayDebuggerShape::MakePoint(PatrolNodePointLocation, PointRadius, FColor::Green));
	}
}

void FGameplayDebuggerCategory_Patrol::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	CanvasContext.Printf(TEXT("---- PATROL ----"));
	CanvasContext.Printf(TEXT(""));

	const APatrolPath* const CurrentPatrolPathRaw = Data.CurrentPatrolPathPtr.Get();
	if (!CurrentPatrolPathRaw)
	{
		CanvasContext.Printf(TEXT("Current Patrol Path: {red}None"));
		return;
	}

	const APatrolNode* const CurrentPatrolNodeRaw = Data.CurrentPatrolNodePtr.Get();
	if (!CurrentPatrolNodeRaw)
	{
		CanvasContext.Printf(TEXT("Current Patrol Node: {red}None"));
		return;
	}

	// Display Path Info
	CanvasContext.Printf(TEXT("Current Patrol Path: {orange}%s"), *CurrentPatrolPathRaw->GetActorLabel());
	CanvasContext.Printf(TEXT(""));

	// Display Node Info
	CanvasContext.Printf(TEXT("Current Patrol Node: {magenta}%s"), *CurrentPatrolNodeRaw->GetActorLabel());
	CanvasContext.Printf(TEXT("Min Wait Time: {green}%.2f {white}| Max Wait Time: {green}%.2f"), CurrentPatrolNodeRaw->GetMinWaitTime(), CurrentPatrolNodeRaw->GetMaxWaitTime());
	CanvasContext.Printf(TEXT("Arrival Radius {green}%.2f"), CurrentPatrolNodeRaw->GetArrivalRadius());
	CanvasContext.Printf(TEXT(""));

	// Display Next Node Info
	if (const APatrolNode* const NextPatrolNode = CurrentPatrolNodeRaw->GetNextNode())
	{
		CanvasContext.Printf(TEXT("Next Patrol Node: {magenta}%s"), *NextPatrolNode->GetActorLabel());
	}
	else
	{
		CanvasContext.Printf(TEXT("Next Patrol Node: {yellow}None"));
	}

	// Display Previous Node Info
	if (const APatrolNode* const PrevPatrolNode = CurrentPatrolNodeRaw->GetPrevNode())
	{
		CanvasContext.Printf(TEXT("Previous Patrol Node: {magenta}%s"), *PrevPatrolNode->GetActorLabel());
	}
	else 
	{
		CanvasContext.Printf(TEXT("Previous Patrol Node: {yellow}None"));
	}
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_Patrol::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_Patrol());
}

#endif // WITH_GAMEPLAY_DEBUGGER_MENU
