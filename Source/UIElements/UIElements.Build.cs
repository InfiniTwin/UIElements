// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UIElements : ModuleRules
{
	public UIElements(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"FlecsLibrary",
			"ECSCore",
			"UIMCU",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
        });

        if (Target.bBuildEditor) {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}