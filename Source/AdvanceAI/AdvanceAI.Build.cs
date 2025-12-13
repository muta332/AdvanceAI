// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdvanceAI : ModuleRules
{
	public AdvanceAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags" });
	}
}
