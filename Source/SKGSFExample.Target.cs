// Copyright 2023, Dakota Dawe, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class SKGSFExampleTarget : TargetRules
{
	public SKGSFExampleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "SKGSFExample" } );
	}
}
