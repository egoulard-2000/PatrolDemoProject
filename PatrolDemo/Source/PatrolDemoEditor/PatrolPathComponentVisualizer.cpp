#include "PatrolPathComponentVisualizer.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "PatrolDemo/PatrolNode.h"
#include "PatrolDemo/PatrolPath.h"
#include "SceneView.h"
#include "SceneManagement.h"

void FPatrolPathComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!Component || !View || !PDI)
	{
		return;
	}

	// Grab this node
	const APatrolPath* const PatrolPath = Cast<APatrolPath>(Component->GetOwner());
	if (!PatrolPath)
	{
		return;
	}

	for (const APatrolNode* Node : PatrolPath->PatrolPathNodes)
	{
		if (!Node)
		{
			continue;
		}

		constexpr float LineThickness = 20.f;
		PDI->DrawLine(PatrolPath->GetActorLocation(), Node->GetActorLocation(), FLinearColor::Red, SDPG_World, LineThickness);
	}
}

void FPatrolPathComponentVisualizer::DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	if (!Component || !Viewport || !View || !Canvas)
	{
		return;
	}

	if (!GEngine)
	{
		return;
	}

	const APatrolPath* const PatrolPath = Cast<APatrolPath>(Component->GetOwner());
	if (!PatrolPath)
	{
		return;
	}

	int Index = 1;
	for (const APatrolNode* Node : PatrolPath->PatrolPathNodes)
	{
		if (!Node)
		{
			continue;
		}

		// Depth Distance check to see if the node is in front of the camera
		const FVector4 ScreenPoint = View->WorldToScreen(Node->GetActorLocation() + (FVector::UpVector * 100.f));
		if (ScreenPoint.W <= 0.f)
		{
			continue;
		}

		FVector2D PixelPos;
		if (!View->ScreenToPixel(ScreenPoint, PixelPos))
		{
			continue;
		}

		// Scale the pixel position by the DPI scale to ensure consistent sizing across different screen resolutions
		PixelPos /= Canvas->GetDPIScale();
		const FText Label = FText::AsNumber(Index++);

		FCanvasTextItem TextItem(PixelPos, Label, GEngine->GetLargeFont(), FLinearColor::Red);
		TextItem.Scale = FVector2D(2.f);
		TextItem.bOutlined = true;
		TextItem.bCentreX = true;
		TextItem.bCentreY = true;

		Canvas->DrawItem(TextItem);
	}
}
