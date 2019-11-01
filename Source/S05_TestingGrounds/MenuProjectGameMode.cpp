// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MenuProjectGameMode.h"
#include "Character/Mannequin.h"
#include "UObject/ConstructorHelpers.h"

AMenuProjectGameMode::AMenuProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Dynamic/Character/Behavior/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
