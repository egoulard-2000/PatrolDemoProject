#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

class FCanvas;
class FPrimitiveDrawInterface;
class FSceneView;
class UActorComponent;

/**
*	Class for drawing visualizations for patrol nodes (editor-only)
*/
class FPatrolPathComponentVisualizer : public FComponentVisualizer
{
public:
	/** Draw visualization for the supplied component */
	void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) final;
	/** Draw HUD on viewport for the supplied component */
	void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) final;
};