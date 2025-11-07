// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveSystem/StatSystemSaveGame.h"

const FString UStatSystemSaveGame::DEFAULT_SAVE_SLOT = TEXT("StatSystemProSave");

UStatSystemSaveGame::UStatSystemSaveGame()
{
	SaveSlotName = DEFAULT_SAVE_SLOT;
	SaveVersion = CURRENT_SAVE_VERSION;
	SaveTimestamp = FDateTime::Now();
}
