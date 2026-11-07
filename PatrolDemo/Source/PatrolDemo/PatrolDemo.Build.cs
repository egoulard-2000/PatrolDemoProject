// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PatrolDemo : ModuleRules
{
	public PatrolDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        if (Target.bBuildDeveloperTools || (Target.Configuration != UnrealTargetConfiguration.Shipping && Target.Configuration != UnrealTargetConfiguration.Test))
        {
            PublicDependencyModuleNames.Add("GameplayDebugger");
            PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
        }
        else
        {
            PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=0");
        }
    }
}
