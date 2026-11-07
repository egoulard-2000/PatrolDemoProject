#include "PatrolEdMode.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/GameplayStatics.h"
#include "LevelEditorViewport.h"
#include "PatrolDemo/PatrolNode.h"
#include "PatrolDemo/PatrolPath.h"
#include "PatrolNodeEdInputProcessor.h"
#include "PatrolEditorObject.h"
#include "Styling/AppStyle.h"

DEFINE_LOG_CATEGORY(LogPatrolEditorMode);

const FEditorModeID UPatrolEdMode::EM_PatrolEdModeId = TEXT("EM_PatrolEdMode");

UPatrolEdMode::UPatrolEdMode()
{
    Info = FEditorModeInfo(
        EM_PatrolEdModeId,
        NSLOCTEXT("PatrolEdMode", "ModeName", "Patrol Tool"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "DetailsView.PropertyIsFavorite"),
        true,
        INT_MAX
    );

	SettingsClass = UPatrolEditorObject::StaticClass();
    if (SettingsClass)
    {
        SettingsObject = SettingsClass->GetDefaultObject();
    }
}

void UPatrolEdMode::Enter()
{
    Super::Enter();
    InputProcessor = MakeShared<FPatrolNodeEdInputProcessor>(this);
    FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, 0);

    if (UPatrolEditorObject* const PatrolEditorSettingsObject = Cast<UPatrolEditorObject>(SettingsObject))
    {
        const bool bSuccessLoad = PatrolEditorSettingsObject->Init();
        if (ensure(bSuccessLoad))
        {
            UE_LOG(LogPatrolEditorMode, Display, TEXT("Patrol: Editor Settings Object loaded successfully"));
            CurrentPatrolPath = GetPathActorInstance();
            ensure(CurrentPatrolPath);
        }
        else
        {
            UE_LOG(LogPatrolEditorMode, Error, TEXT("Patrol: Editor Settings Object NOT loaded!"));
        }
    }
}

void UPatrolEdMode::Exit()
{
    FinalizePath();

    // Unregister and reset
    if (InputProcessor.IsValid() && FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
    }
    InputProcessor.Reset();

	Super::Exit();
}

void UPatrolEdMode::ModeTick(float DeltaTime)
{
	Super::ModeTick(DeltaTime);
}

void UPatrolEdMode::RefreshFinalizeState()
{
	UPatrolEditorObject* const PatrolEditorSettingsObject = Cast<UPatrolEditorObject>(SettingsObject);
	if (ensure(PatrolEditorSettingsObject))
	{
        PatrolEditorSettingsObject->bCanFinalize = CurrentPatrolPath ? !CurrentPatrolPath->IsEmpty() : false;
	}
}

bool UPatrolEdMode::PlacePatrolPoint()
{
    const UPatrolEditorObject* const PatrolEditorSettingsObject = Cast<UPatrolEditorObject>(SettingsObject);
    if (!ensure(PatrolEditorSettingsObject))
    {
        UE_LOG(LogPatrolEditorMode, Display, TEXT("Patrol: PlacePatrolPoint | No Settings Object"));
        return false;
    }

    APatrolNode* const SpawnedNode = GetPatrolActorInstance();
    if (!ensure(SpawnedNode))
    {
        UE_LOG(LogPatrolEditorMode, Display, TEXT("Patrol: PlacePatrolPoint | Failed to Spawn New Node. Early Exit"));
        return false;
    }

    // 1. Update Spawned Node Name and Construct
    SpawnedNode->SetActorLabel(FString::Printf(TEXT("%s_%d"), *PatrolEditorSettingsObject->PatrolPathName.ToString(), CurrentPathIndex));
    SpawnedNode->WaitTime = PatrolEditorSettingsObject->PatrolNodeWaitTime;
    SpawnedNode->ArrivalRadius = PatrolEditorSettingsObject->PatrolArrivalRadius;

    // 2. Append to current path we're tracking
    CurrentPathIndex++;
    if (!CurrentPatrolPath)
    {
        CurrentPatrolPath = GetPathActorInstance();
        check(CurrentPatrolPath);
    }
    CurrentPatrolPath->AddPatrolNode(SpawnedNode);

    // 3. Assign next node and prev node if we now have a valid path to work with
    if (ActivePatrolActorInstance)
    {
        ActivePatrolActorInstance->SetNextNode(SpawnedNode);
        SpawnedNode->SetPrevNode(ActivePatrolActorInstance);
        
        // We rerun this to have the BP construction scripts re-run so the path visuals properly update
        ActivePatrolActorInstance->RerunConstructionScripts();
        SpawnedNode->RerunConstructionScripts();
    }

    // 4. Traverse patrol node list
    ActivePatrolActorInstance = SpawnedNode;

    // 5. Refresh path for finalization purposes
    RefreshFinalizeState();
    return true;
}

void UPatrolEdMode::FinalizePath()
{
    const UPatrolEditorObject* const PatrolEditorSettingsObject = Cast<UPatrolEditorObject>(SettingsObject);
    if (!PatrolEditorSettingsObject)
    {
        UE_LOG(LogPatrolEditorMode, Error, TEXT("Patrol. Could not finalize!"));
        return;
    }

    // Eliminate risk of adding a next node that won't exit
    if (ActivePatrolActorInstance)
    {
        // If proper looping path, then re-route the next node to be the first node in the path
        if (PatrolEditorSettingsObject->bPathShouldLoop && CurrentPatrolPath && CurrentPatrolPath->GetSize() >= 3)
        {
            APatrolNode* const FirstNodeInPath = CurrentPatrolPath->GetFirstNodeInPath();
            ActivePatrolActorInstance->SetNextNode(FirstNodeInPath);
            if (FirstNodeInPath)
            {
                FirstNodeInPath->SetPrevNode(ActivePatrolActorInstance);
            }

            // We rerun this to have the BP construction scripts re-run so the path visuals properly update
            ActivePatrolActorInstance->RerunConstructionScripts();
            FirstNodeInPath->RerunConstructionScripts();
        }
        else
        {
            ActivePatrolActorInstance->SetNextNode(nullptr);
        }
        ActivePatrolActorInstance = nullptr;
    }
    
    // Finalize Patrol Path (unless no nodes were added)
    if (CurrentPatrolPath)
    {
        if (CurrentPatrolPath->IsEmpty())
        {
            CurrentPatrolPath->Destroy();
            ResetCurrentPath();
            UE_LOG(LogPatrolEditorMode, Error, TEXT("Patrol: no nodes placed since the last finalize."));
            return;
        }

        const FString PathNameString = PatrolEditorSettingsObject->PatrolPathName.ToString();
        CurrentPatrolPath->SetActorLabel(FString::Printf(TEXT("%s_Path"), *PathNameString));
        CurrentPatrolPath->FinalizePath();
    }

    ResetCurrentPath();
}

APatrolNode* UPatrolEdMode::GetPatrolActorInstance()
{
	UWorld* const World = GetWorld();
    check(World);

    // Zero Vector is an invalid location (At least for now... it's not great to compare at 0-XYZ)
    const FVector WorldLocation = GetMouseHitLocation();
    if (WorldLocation == FVector::ZeroVector)
    {
        return nullptr;
    }

    const UPatrolEditorObject* const PatrolEditorSettingsObject = Cast<UPatrolEditorObject>(SettingsObject);
    if (!PatrolEditorSettingsObject)
    {
        UE_LOG(LogPatrolEditorMode, Warning, TEXT("Patrol | SpawnPatrolActorInstance | Could not find Settings Object"));
        return nullptr;
    }

    const TSubclassOf<APatrolNode> PatrolNodeClassToSpawn = PatrolEditorSettingsObject->GetPatrolNodeClass() != nullptr ? 
        PatrolEditorSettingsObject->GetPatrolNodeClass().Get() : APatrolNode::StaticClass();

    return World->SpawnActor<APatrolNode>(PatrolNodeClassToSpawn, WorldLocation, FRotator::ZeroRotator);
}

APatrolPath* UPatrolEdMode::GetPathActorInstance()
{
    UWorld* const World = GetWorld();
    check(World);

    const UPatrolEditorObject* const PatrolEditorSettingsObject = Cast<UPatrolEditorObject>(SettingsObject);
    if (!PatrolEditorSettingsObject)
    {
        UE_LOG(LogPatrolEditorMode, Warning, TEXT("Patrol | SpawnPathActorInstance | Could not find Settings Object"));
        return nullptr;
    }

    const TSubclassOf<APatrolPath> PatrolPathClassToSpawn = PatrolEditorSettingsObject->GetPatrolPathClass() != nullptr ?
        PatrolEditorSettingsObject->GetPatrolPathClass().Get() : APatrolPath::StaticClass();

    const double WorldBufferLimit = UE_LARGE_WORLD_MAX * 0.9f;
    const FVector DefaultPathSpawnLocation = FVector(WorldBufferLimit, WorldBufferLimit, 0.f);
    return World->SpawnActor<APatrolPath>(PatrolPathClassToSpawn, DefaultPathSpawnLocation, FRotator::ZeroRotator);
}

bool UPatrolEdMode::DestroyPatrolActorInstance()
{
	bool bDestroyed = false;
    if (ActivePatrolActorInstance)
    {
        bDestroyed = ActivePatrolActorInstance->Destroy();
        if (bDestroyed)
        {
            RefreshFinalizeState();
        }
    }

    ActivePatrolActorInstance = nullptr;
    return bDestroyed;
}

FVector UPatrolEdMode::GetMouseHitLocation() const
{
    FLevelEditorViewportClient* const LevelViewportClient = GCurrentLevelEditingViewportClient;
    if (!LevelViewportClient)
    {
        return FVector::ZeroVector;
    }

    const UWorld* const ViewportWorld = LevelViewportClient->GetWorld();
    if (!ViewportWorld)
    {
        return FVector::ZeroVector;
	}

    FHitResult HitResult;
    FCollisionQueryParams LineParams(SCENE_QUERY_STAT(FocusOnPoint), true);
    LineParams.AddIgnoredActor(ActivePatrolActorInstance);
    FViewportCursorLocation CursorLocation = LevelViewportClient->GetCursorWorldLocationFromMousePos();

	// Perform a line trace to find the hit location in the world
    const bool bHit = ViewportWorld->LineTraceSingleByObjectType(
        HitResult,
        CursorLocation.GetOrigin(),
        CursorLocation.GetOrigin() + CursorLocation.GetDirection() * WORLD_MAX,
        FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects),
        LineParams
    );

    if (!bHit)
    {
        return FVector::ZeroVector;
    }

    return HitResult.ImpactPoint;
}

void UPatrolEdMode::ResetCurrentPath()
{
    CurrentPathIndex = 0;
    CurrentPatrolPath = nullptr;
}
