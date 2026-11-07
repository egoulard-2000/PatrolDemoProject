#pragma once

#include "Framework/Application/IInputProcessor.h"
#include "Templates/SharedPointer.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FSlateApplication;
class ICursor;
struct FPointerEvent;
class UPatrolEdMode;

/**
 * Input Processor used by the Quick Placement Editor Mode to consume input before the editor handles it
 */
class FPatrolNodeEdInputProcessor : public IInputProcessor
{
public:
    FPatrolNodeEdInputProcessor(UPatrolEdMode* InEditorMode);

    // ~ Start IInputProcessor
    virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
    virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
    virtual const TCHAR* GetDebugName() const override { return TEXT("FPatrolNodeEdInputProcessor"); }
    // ~ End IInputProcessor

    // Driver to trigger commands
    TWeakObjectPtr<UPatrolEdMode> PatrolEditorMode;

private:
	bool IsMouseOverViewport(const FPointerEvent& MouseEvent) const;
};