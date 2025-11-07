// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveSystem/StatSystemSaveFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "StatLayer/StatComponent.h"

bool UStatSystemSaveFunctionLibrary::SaveStatSystem(AActor* Actor, const FString& SaveSlotName, int32 UserIndex)
{
	if (!Actor)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveStatSystem: Actor is null!"));
		return false;
	}

	// Create save game object
	UStatSystemSaveGame* SaveGameInstance = Cast<UStatSystemSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UStatSystemSaveGame::StaticClass()));

	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveStatSystem: Failed to create save game object!"));
		return false;
	}

	// Set metadata
	SaveGameInstance->SaveSlotName = SaveSlotName;
	SaveGameInstance->SaveTimestamp = FDateTime::Now();
	SaveGameInstance->SaveVersion = UStatSystemSaveGame::CURRENT_SAVE_VERSION;

	// Save Stat Component
	if (UStatComponent* StatComp = Actor->FindComponentByClass<UStatComponent>())
	{
		SerializeStatComponent(StatComp, SaveGameInstance->StatData);
	}

	// TODO: Add other component serialization when they're implemented
	// SaveBodyComponent(Actor, SaveGameInstance->BodyData);
	// SaveStatusEffectComponent(Actor, SaveGameInstance->StatusEffectsData);
	// SaveEnvironmentComponent(Actor, SaveGameInstance->EnvironmentData);
	// SaveProgressionComponent(Actor, SaveGameInstance->ProgressionData);
	// SaveTimeComponent(Actor, SaveGameInstance->TimeData);

	// Save to disk
	bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("SaveStatSystem: Successfully saved to slot '%s'"), *SaveSlotName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveStatSystem: Failed to save to slot '%s'"), *SaveSlotName);
	}

	return bSuccess;
}

bool UStatSystemSaveFunctionLibrary::LoadStatSystem(AActor* Actor, const FString& SaveSlotName, int32 UserIndex)
{
	if (!Actor)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadStatSystem: Actor is null!"));
		return false;
	}

	// Load save game
	UStatSystemSaveGame* LoadedGame = Cast<UStatSystemSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

	if (!LoadedGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadStatSystem: No save file found in slot '%s'"), *SaveSlotName);
		return false;
	}

	// Version check (for future migration)
	if (LoadedGame->SaveVersion != UStatSystemSaveGame::CURRENT_SAVE_VERSION)
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadStatSystem: Save version mismatch (saved: %d, current: %d)"),
			LoadedGame->SaveVersion, UStatSystemSaveGame::CURRENT_SAVE_VERSION);
		// Continue loading anyway, but warn
	}

	// Load Stat Component
	if (UStatComponent* StatComp = Actor->FindComponentByClass<UStatComponent>())
	{
		DeserializeStatComponent(StatComp, LoadedGame->StatData);
	}

	// TODO: Add other component deserialization
	// LoadBodyComponent(Actor, LoadedGame->BodyData);
	// LoadStatusEffectComponent(Actor, LoadedGame->StatusEffectsData);
	// LoadEnvironmentComponent(Actor, LoadedGame->EnvironmentData);
	// LoadProgressionComponent(Actor, LoadedGame->ProgressionData);
	// LoadTimeComponent(Actor, LoadedGame->TimeData);

	UE_LOG(LogTemp, Log, TEXT("LoadStatSystem: Successfully loaded from slot '%s' (saved on %s)"),
		*SaveSlotName, *LoadedGame->SaveTimestamp.ToString());

	return true;
}

bool UStatSystemSaveFunctionLibrary::DoesSaveExist(const FString& SaveSlotName, int32 UserIndex)
{
	return UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex);
}

bool UStatSystemSaveFunctionLibrary::DeleteSave(const FString& SaveSlotName, int32 UserIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		bool bSuccess = UGameplayStatics::DeleteGameInSlot(SaveSlotName, UserIndex);
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("DeleteSave: Deleted save slot '%s'"), *SaveSlotName);
		}
		return bSuccess;
	}

	UE_LOG(LogTemp, Warning, TEXT("DeleteSave: Save slot '%s' does not exist"), *SaveSlotName);
	return false;
}

bool UStatSystemSaveFunctionLibrary::GetSaveMetadata(
	const FString& SaveSlotName,
	int32 UserIndex,
	FString& OutPlayerName,
	FDateTime& OutSaveTimestamp,
	int32& OutSaveVersion)
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		return false;
	}

	UStatSystemSaveGame* LoadedGame = Cast<UStatSystemSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

	if (LoadedGame)
	{
		OutPlayerName = LoadedGame->PlayerName;
		OutSaveTimestamp = LoadedGame->SaveTimestamp;
		OutSaveVersion = LoadedGame->SaveVersion;
		return true;
	}

	return false;
}

bool UStatSystemSaveFunctionLibrary::SaveStatsOnly(UStatComponent* StatComp, const FString& SaveSlotName, int32 UserIndex)
{
	if (!StatComp)
	{
		return false;
	}

	UStatSystemSaveGame* SaveGameInstance = Cast<UStatSystemSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UStatSystemSaveGame::StaticClass()));

	if (!SaveGameInstance)
	{
		return false;
	}

	SerializeStatComponent(StatComp, SaveGameInstance->StatData);

	return UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);
}

bool UStatSystemSaveFunctionLibrary::LoadStatsOnly(UStatComponent* StatComp, const FString& SaveSlotName, int32 UserIndex)
{
	if (!StatComp)
	{
		return false;
	}

	UStatSystemSaveGame* LoadedGame = Cast<UStatSystemSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

	if (!LoadedGame)
	{
		return false;
	}

	DeserializeStatComponent(StatComp, LoadedGame->StatData);

	return true;
}

UStatSystemSaveGame* UStatSystemSaveFunctionLibrary::CreateSaveGameObject()
{
	return Cast<UStatSystemSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UStatSystemSaveGame::StaticClass()));
}

// ========== HELPER FUNCTIONS ==========

void UStatSystemSaveFunctionLibrary::SerializeStatComponent(UStatComponent* StatComp, FSavedStatData& OutData)
{
	if (!StatComp)
	{
		return;
	}

	// Save all stats
	OutData.Stats = StatComp->Stats;
	OutData.bEnabled = StatComp->bEnabled;
	OutData.bUseSimpleMode = StatComp->bUseSimpleMode;
	OutData.bEnableAutoRegeneration = StatComp->bEnableAutoRegeneration;
	OutData.CriticalThreshold = StatComp->CriticalThreshold;

	UE_LOG(LogTemp, Verbose, TEXT("SerializeStatComponent: Saved %d stats"), OutData.Stats.Num());
}

void UStatSystemSaveFunctionLibrary::DeserializeStatComponent(UStatComponent* StatComp, const FSavedStatData& InData)
{
	if (!StatComp)
	{
		return;
	}

	// Restore all stats
	StatComp->Stats = InData.Stats;
	StatComp->bEnabled = InData.bEnabled;
	StatComp->bUseSimpleMode = InData.bUseSimpleMode;
	StatComp->bEnableAutoRegeneration = InData.bEnableAutoRegeneration;
	StatComp->CriticalThreshold = InData.CriticalThreshold;

	UE_LOG(LogTemp, Verbose, TEXT("DeserializeStatComponent: Loaded %d stats"), InData.Stats.Num());
}
