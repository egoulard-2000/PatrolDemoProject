// Copyright Epic Games, Inc. All Rights Reserved.

#include "PatrolDemo.h"
#include "Modules/ModuleManager.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "GameplayDebuggerCategory_Patrol.h"
#endif // WITH_GAMEPLAY_DEBUGGER

IMPLEMENT_PRIMARY_GAME_MODULE( FPatrolDemoModule, PatrolDemo, "PatrolDemo" );

void FPatrolDemoModule::StartupModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("Patrol", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_Patrol::MakeInstance), EGameplayDebuggerCategoryState::EnabledInGameAndSimulate);
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FPatrolDemoModule::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("Patrol");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
#endif
}
