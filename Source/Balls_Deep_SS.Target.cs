// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Balls_Deep_SSTarget : TargetRules
{
	public Balls_Deep_SSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Balls_Deep_SS");
	}
}
