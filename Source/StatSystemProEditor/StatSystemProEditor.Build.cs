// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StatSystemProEditor : ModuleRules
{
	public StatSystemProEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"StatSystemPro",
			"UnrealEd",
			"Slate",
			"SlateCore",
			"PropertyEditor",
			"EditorStyle",
			"Projects",
			"InputCore",
			"EditorSubsystem",
			"Blutility",
			"UMGEditor",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EditorWidgets",
			"ToolMenus",
			"WorkspaceMenuStructure"
		});
	}
}
