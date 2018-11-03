// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Balls_Deep_SSEditorTarget : TargetRules
{
	public Balls_Deep_SSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Balls_Deep_SS");
	}
}
