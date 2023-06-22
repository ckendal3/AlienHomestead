// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionSystem : ModuleRules
{
	public ActionSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new[] {
				"ActionSystem"
			}
		);
			
		PrivateIncludePaths.AddRange(
			new[] {
				"ActionSystem"
			}
		);
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"AIModule",
			"Core",
			"CoreUObject",
			"DeveloperSettings",
			"Engine",
			"GameplayTasks"
		});
	}
}