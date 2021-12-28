// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeagueofLegendsGameMode.h"
#include "LeagueofLegendsPlayerController.h"
#include "LeagueofLegendsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALeagueofLegendsGameMode::ALeagueofLegendsGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ALeagueofLegendsPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}