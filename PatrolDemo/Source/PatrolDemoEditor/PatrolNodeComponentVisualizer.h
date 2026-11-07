#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

class FPrimitiveDrawInterface;
class FSceneView;
class UActorComponent;

/**
*	Class for drawing visualizations for patrol nodes (editor-only)
*/
class FPatrolNodeComponentVisualizer : public FComponentVisualizer
{
public:
	/** Draw visualization for the supplied component */
	void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) final;
};