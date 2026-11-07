#pragma once

#include "CoreMinimal.h"
#include "Tools/UEdMode.h"
#include "PatrolEdMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPatrolEditorMode, Log, All);

class APatrolNode;
class APatrolPath;
class FPatrolNodeEdInputProcessor;

UCLASS()
class UPatrolEdMode : public UEdMode
{
    GENERATED_BODY()

	friend class FPatrolNodeEdInputProcessor; 

public:
    const static FEditorModeID EM_PatrolEdModeId;

    UPatrolEdMode();

    // Begin UEdMode interface
    void Enter() final;
    void Exit() final;
    void ModeTick(float DeltaTime) final;
    // End UEdMode interface 

    // Handles placement of Patrol Point into a Path
    bool PlacePatrolPoint();

    // Finalizes the path using all the nodes collected during placement (so that user can move onto other paths)
    void FinalizePath();

private:
    /**
     * Spawns a new patrol actor instance
     */
    APatrolNode* GetPatrolActorInstance();

    /**
     * Spawns a new path actor instance
     */
    APatrolPath* GetPathActorInstance();

    /**
     * Destroys the currently spawned actor instance if any
     */
    bool DestroyPatrolActorInstance();

    /**
     * Calculates the mouse's 3D projection in the world based on the cursor position
     */
    FVector GetMouseHitLocation() const;

    /**
     * Empties 'CurrentPatrolPathNodes' and resets 'CurrentPathIndex'
     */
    void ResetCurrentPath();

    /**
     * Updates the Finalize State for the Editor Object
     */
    void RefreshFinalizeState();

    /**
     * Index Tracker for CurrentPatrolPathNodes
     */
    int32 CurrentPathIndex = { 0 };

    /**
     * The currently spawned actor instance
     */
    UPROPERTY()
    TObjectPtr<APatrolNode> ActivePatrolActorInstance;

    /**
     * Path object to hold/manage all nodes in respect to current path
     */
    UPROPERTY(Transient)
    TObjectPtr<APatrolPath> CurrentPatrolPath;

    /**
     * The InputProcessor we pushed to react to inputs
     */
    TSharedPtr<FPatrolNodeEdInputProcessor> InputProcessor;
};