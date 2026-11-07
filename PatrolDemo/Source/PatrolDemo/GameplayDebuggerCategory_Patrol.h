#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebuggerCategory.h"

class AActor;
class APatrolNode;
class APatrolPath;
class APlayerController;
class FGameplayDebuggerCanvasContext;

class FGameplayDebuggerCategory_Patrol : public FGameplayDebuggerCategory
{
	FGameplayDebuggerCategory_Patrol();

public:
	/** Collects the data we would like to print */
	void CollectData(APlayerController* OwnerPC, AActor* DebugActor) final;

	/** Displays the data we collected in the CollectData function */
	 void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) final;

	 /** Creates an instance of this category - will be used on module startup to include our category in the Editor */
	 static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	//The data we're going to print inside the viewport
	struct FPatrolData
	{
		TWeakObjectPtr<const APatrolPath> CurrentPatrolPathPtr;
		TWeakObjectPtr<const APatrolNode> CurrentPatrolNodePtr;
	};
	FPatrolData Data;
};

#endif