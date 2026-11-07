#include "PatrolNodeEdInputProcessor.h"
#include "GenericPlatform/ICursor.h"
#include "LevelEditorViewport.h"
#include "PatrolEdMode.h"
#include "Slate/SceneViewport.h"

FPatrolNodeEdInputProcessor::FPatrolNodeEdInputProcessor(UPatrolEdMode* InEditorMode) : PatrolEditorMode(InEditorMode)
{
}

void FPatrolNodeEdInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
}

bool FPatrolNodeEdInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
    if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton || MouseEvent.IsAltDown())
    {
        return false;
    }

    if (!IsMouseOverViewport(MouseEvent))
    {
        return false;
    }

    UPatrolEdMode* const PatrolEdModeRaw = PatrolEditorMode.Get();
    if (!PatrolEdModeRaw)
    {
        return false;
	}

    return PatrolEdModeRaw->PlacePatrolPoint();
}

bool FPatrolNodeEdInputProcessor::IsMouseOverViewport(const FPointerEvent& MouseEvent) const
{
    const FLevelEditorViewportClient* const LevelViewportClient = GCurrentLevelEditingViewportClient;
    if (!LevelViewportClient)
    {
        return false;
	}

    const FViewport* const Viewport = LevelViewportClient->Viewport;
    if (!Viewport)
    {
        return false;
    }

	// Mouse in Level Viewport?
	const FSceneViewport* SceneViewport = static_cast<const FSceneViewport*>(Viewport);
    return SceneViewport->GetCachedGeometry().IsUnderLocation(MouseEvent.GetScreenSpacePosition());
}
