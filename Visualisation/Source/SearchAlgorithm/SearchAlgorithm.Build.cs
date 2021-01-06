// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SearchAlgorithm : ModuleRules
{
	public SearchAlgorithm(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine" });
    }
}
