// Copyright 2023, Dakota Dawe, All rights reserved

using UnrealBuildTool;

public class SKGSFExample : ModuleRules
{
	public SKGSFExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils", "HTTP", "Json", "UMG" });

        PrivateDependencyModuleNames.AddRange(System.Array.Empty<string>());

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
