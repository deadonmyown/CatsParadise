// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatsParadiseGameMode.h"
#include "UObject/ConstructorHelpers.h"

ACatsParadiseGameMode::ACatsParadiseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
