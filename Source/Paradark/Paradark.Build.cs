// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Paradark : ModuleRules
{
	public Paradark(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}
