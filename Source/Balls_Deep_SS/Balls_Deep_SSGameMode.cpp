// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Balls_Deep_SSGameMode.h"
#include "Balls_Deep_SSCharacter.h"

ABalls_Deep_SSGameMode::ABalls_Deep_SSGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ABalls_Deep_SSCharacter::StaticClass();	
}
