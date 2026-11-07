#pragma once

#include "CoreMinimal.h"
#include "PatrolEditorObject.generated.h"

class APatrolNode;
class APatrolPath;

/**
 * Settings Object used by the Patrol Editor Mode to assign/visualize options in the side bar
 */
UCLASS()
class UPatrolEditorObject : public UObject
{
    GENERATED_BODY()

public:
    // Returns true if the PatrolNodeClass loaded and is ready for use
    bool Init();

    // Name of the patrol points when finalizing path
    UPROPERTY(EditAnywhere, Category = "Patrol | Path")
    FName PatrolPathName = TEXT("DEFAULT_PATROL_NAME");

    // When enabled, the last node in the path will loop back to the first node in path
    UPROPERTY(EditAnywhere, Category = "Patrol | Path")
    bool bPathShouldLoop = { true };

    // The placed node's wait time
    UPROPERTY(EditAnywhere, Category = "Patrol | Node")
    FFloatInterval PatrolNodeWaitTime = { FFloatInterval(1.f, 10.f) };

    // The placed node's arrival radius
    UPROPERTY(EditAnywhere, Category = "Patrol | Node")
    float PatrolArrivalRadius = { 100.f };

    UPROPERTY(Transient, Meta = (HideInDetailPanel))
    bool bCanFinalize = { false };

    // Button linked to this function. Responsible for ending the current path setup on this patrol
    UFUNCTION(CallInEditor, Meta = (EditCondition = "bCanFinalize"))
    void FinalizePath();

    TSubclassOf<APatrolNode> GetPatrolNodeClass() const;

    TSubclassOf<APatrolPath> GetPatrolPathClass() const;

private:
    // The type of Patrol Node to spawn in (if 'None' is selected, then use default Patrol Node class)
    UPROPERTY()
    TSubclassOf<APatrolNode> PatrolNodeClass;

    UPROPERTY()
    TSubclassOf<APatrolPath> PatrolPathClass;

#if WITH_EDITOR
    // ~ UObject class
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) final;
#endif
};