#include "PatrolNodeComponentVisualizer.h"
#include "PatrolDemo/PatrolNode.h"
#include "SceneView.h"
#include "SceneManagement.h"

void FPatrolNodeComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	// Grab this node
	const AActor* const OwningActor = Component ? Component->GetOwner() : nullptr;
	const APatrolNode* const PatrolNode = Cast<APatrolNode>(OwningActor);
	if (!PatrolNode)
	{
		return;
	}

	if (PDI)
	{
		constexpr int32 NumSides = 32;
		constexpr float LineThickness = 5.f;

		DrawCircle(PDI, PatrolNode->GetActorLocation(), PatrolNode->GetActorForwardVector(), PatrolNode->GetActorRightVector(), FColor::Red,
			PatrolNode->GetArrivalRadius(), NumSides, SDPG_World, LineThickness);
	}
}
