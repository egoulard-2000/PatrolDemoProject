#include "PatrolEditorObject.h"
#include "EditorModeManager.h"
#include "PatrolEdMode.h"
#include "PatrolDemo/PatrolNode.h"
#include "PatrolDemo/PatrolPath.h"

#define LOCTEXT_NAMESPACE "UPatrolEditorObject"

bool UPatrolEditorObject::Init()
{
	const FString PatrolNodePath = "/Game/Patrol/BP_PatrolNode.BP_PatrolNode_C";
	PatrolNodeClass = LoadClass<APatrolNode>(GetWorld(), *PatrolNodePath);

	// Could I name this "PatrolPathPath".....? I swear naming things in code is harder than naming your own child
	const FString PatrolPath = "/Game/Patrol/BP_PatrolPath.BP_PatrolPath_C";
	PatrolPathClass = LoadClass<APatrolPath>(GetWorld(), *PatrolPath);

	return PatrolNodeClass != nullptr && PatrolPathClass != nullptr;
}

void UPatrolEditorObject::FinalizePath()
{
	if (!bCanFinalize)
	{
		UE_LOG(LogPatrolEditorMode, Error, TEXT("Patrol: Cannot Finalize Current Path."));
		return;
	}

	UPatrolEdMode* const PatrolEdModeRaw = Cast<UPatrolEdMode>(GLevelEditorModeTools().GetActiveScriptableMode(UPatrolEdMode::EM_PatrolEdModeId));
	if (!PatrolEdModeRaw)
	{
		return;
	}

	PatrolEdModeRaw->FinalizePath();
}

TSubclassOf<APatrolNode> UPatrolEditorObject::GetPatrolNodeClass() const
{
	return PatrolNodeClass;
}

TSubclassOf<APatrolPath> UPatrolEditorObject::GetPatrolPathClass() const
{
	return PatrolPathClass;
}

#if WITH_EDITOR
void UPatrolEditorObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// If the name of the patrol path changes, then try to finalize last path
	const FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UPatrolEditorObject, PatrolPathName))
	{
		FinalizePath();
	}
}
#endif
