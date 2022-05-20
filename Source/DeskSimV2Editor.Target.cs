// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DeskSimV2EditorTarget : TargetRules
{
	public DeskSimV2EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "DeskSimV2" } );
	}
}
