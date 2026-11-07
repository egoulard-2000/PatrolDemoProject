using UnrealBuildTool;

public class PatrolDemoEditor : ModuleRules
{
    public PatrolDemoEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "PatrolDemo", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UnrealEd", "EditorFramework", "InteractiveToolsFramework", "EditorInteractiveToolsFramework" });
    }
}