// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UICore : ModuleRules
{
	public UICore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "UICore/Public" });

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