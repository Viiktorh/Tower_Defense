// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tower_DefenseGameMode.h"
#include "Tower_DefenseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATower_DefenseGameMode::ATower_DefenseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
