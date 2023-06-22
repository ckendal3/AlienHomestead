// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlienHomestead : ModuleRules
{
	public AlienHomestead(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", "CoreUObject", "Engine", 
			"InputCore", "HeadMountedDisplay", 
			"AIModule", "UMG", "DeveloperSettings",
			"ActionSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
	}
}
