#include "PatrolDemoEditor.h"
#include "Editor/UnrealEdEngine.h"
#include "PatrolNodeComponentVisualizer.h"
#include "PatrolPathComponentVisualizer.h"
#include "PatrolDemo/PatrolNodeVisualizerComponent.h"
#include "PatrolDemo/PatrolPathVisualizerComponent.h"
#include "UnrealEdGlobals.h"

IMPLEMENT_GAME_MODULE(FPatrolDemoEditorModule, PatrolDemoEditor);

void FPatrolDemoEditorModule::StartupModule()
{
	if (IsRunningCommandlet())
	{
		return;
	}

	if (!GUnrealEd)
	{
		return;
	}

	GUnrealEd->RegisterComponentVisualizer(UPatrolNodeVisualizerComponent::StaticClass()->GetFName(), MakeShareable(new FPatrolNodeComponentVisualizer));
	GUnrealEd->RegisterComponentVisualizer(UPatrolPathVisualizerComponent::StaticClass()->GetFName(), MakeShareable(new FPatrolPathComponentVisualizer));
}

void FPatrolDemoEditorModule::ShutdownModule()
{
	if (!GUnrealEd)
	{
		return;
	}

	GUnrealEd->UnregisterComponentVisualizer(UPatrolNodeVisualizerComponent::StaticClass()->GetFName());
	GUnrealEd->UnregisterComponentVisualizer(UPatrolPathVisualizerComponent::StaticClass()->GetFName());
}