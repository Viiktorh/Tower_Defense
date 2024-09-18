// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tower_Defense : ModuleRules
{
	public Tower_Defense(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
