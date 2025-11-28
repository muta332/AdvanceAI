// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvanceAIGameMode.h"
#include "ABasePlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAdvanceAIGameMode::AAdvanceAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
