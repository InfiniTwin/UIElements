// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UICore : ModuleRules
{
	public UICore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
				"ECSCore",
				"FlecsLibrary",
			});


		PrivateDependencyModuleNames.AddRange(new string[] {
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			});
	}
}