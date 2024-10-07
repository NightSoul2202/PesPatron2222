// Copyright 2023, Dakota Dawe, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class SKGSFExampleEditorTarget : TargetRules
{
	public SKGSFExampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "SKGSFExample" } );
	}
}
