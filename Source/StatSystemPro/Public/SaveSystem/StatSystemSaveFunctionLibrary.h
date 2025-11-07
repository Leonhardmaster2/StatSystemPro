// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveSystem/StatSystemSaveGame.h"
#include "StatLayer/StatComponent.h"
#include "StatSystemSaveFunctionLibrary.generated.h"

/**
 * ============================================================================
 * STAT SYSTEM PRO - SAVE/LOAD FUNCTION LIBRARY
 * ============================================================================
 *
 * Blueprint-friendly save/load system for ALL StatSystemPro components
 *
 * FEATURES:
 * - Save ALL stats, body parts, status effects, environment, progression, time
 * - Load everything in one call
 * - Multiple save slots support
 * - Automatic timestamping
 * - Version control for migration
 *
 * USAGE:
 * 1. Call "Save Stat System (Complete)" to save everything
 * 2. Call "Load Stat System (Complete)" to restore everything
 */
UCLASS()
class STATSYSTEMPRO_API UStatSystemSaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========== COMPLETE SAVE/LOAD (ALL SYSTEMS) ==========

	/**
	 * Save ALL StatSystemPro data to a save slot
	 * BLUEPRINT: Complete save in one function call
	 *
	 * Saves:
	 * - All stats (health, stamina, etc.)
	 * - All body parts (fractures, bleeding, etc.)
	 * - All active status effects
	 * - Environment state
	 * - Progression (XP, levels, skills)
	 * - Current time
	 *
	 * @param Actor - Actor with StatSystemPro components
	 * @param SaveSlotName - Name of save slot (default: "StatSystemProSave")
	 * @param UserIndex - User index for multiplayer (default: 0)
	 * @return True if saved successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save/Load", meta=(
		DisplayName = "Save Stat System (Complete)",
		Tooltip = "Save ALL StatSystemPro data (stats, body, effects, environment, progression, time)",
		Keywords = "save file write serialize"
	))
	static bool SaveStatSystem(AActor* Actor, const FString& SaveSlotName = "StatSystemProSave", int32 UserIndex = 0);

	/**
	 * Load ALL StatSystemPro data from a save slot
	 * BLUEPRINT: Complete load in one function call
	 *
	 * Restores:
	 * - All stats
	 * - All body parts
	 * - All status effects
	 * - Environment state
	 * - Progression
	 * - Time
	 *
	 * @param Actor - Actor with StatSystemPro components
	 * @param SaveSlotName - Name of save slot
	 * @param UserIndex - User index for multiplayer
	 * @return True if loaded successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save/Load", meta=(
		DisplayName = "Load Stat System (Complete)",
		Tooltip = "Load ALL StatSystemPro data from save file",
		Keywords = "load file read deserialize"
	))
	static bool LoadStatSystem(AActor* Actor, const FString& SaveSlotName = "StatSystemProSave", int32 UserIndex = 0);

	// ========== UTILITIES ==========

	/**
	 * Check if a save file exists
	 * BLUEPRINT: Check before loading
	 *
	 * @param SaveSlotName - Name of save slot
	 * @param UserIndex - User index
	 * @return True if save file exists
	 */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Save/Load", meta=(
		DisplayName = "Does Save Exist?",
		Tooltip = "Check if a save file exists",
		Keywords = "check exists save file"
	))
	static bool DoesSaveExist(const FString& SaveSlotName = "StatSystemProSave", int32 UserIndex = 0);

	/**
	 * Delete a save file
	 * BLUEPRINT: Remove a save slot
	 *
	 * @param SaveSlotName - Name of save slot to delete
	 * @param UserIndex - User index
	 * @return True if deleted successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save/Load", meta=(
		DisplayName = "Delete Save File",
		Tooltip = "Delete a save file",
		Keywords = "delete remove save file"
	))
	static bool DeleteSave(const FString& SaveSlotName = "StatSystemProSave", int32 UserIndex = 0);

	/**
	 * Get save file metadata without loading everything
	 * BLUEPRINT: Quick info about a save file
	 *
	 * @param SaveSlotName - Name of save slot
	 * @param UserIndex - User index
	 * @param OutPlayerName - Player name from save
	 * @param OutSaveTimestamp - When save was created
	 * @param OutSaveVersion - Save version number
	 * @return True if save exists and metadata read successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save/Load", meta=(
		DisplayName = "Get Save Metadata",
		Tooltip = "Get save file info without loading everything",
		Keywords = "info metadata save file"
	))
	static bool GetSaveMetadata(
		const FString& SaveSlotName,
		int32 UserIndex,
		FString& OutPlayerName,
		FDateTime& OutSaveTimestamp,
		int32& OutSaveVersion);

	// ========== INDIVIDUAL COMPONENT SAVE/LOAD ==========

	/**
	 * Save only stats (for selective saving)
	 * BLUEPRINT: Save just the stat component
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save/Load|Individual", meta=(
		DisplayName = "Save Stats Only",
		Tooltip = "Save only stat component data"
	))
	static bool SaveStatsOnly(UStatComponent* StatComp, const FString& SaveSlotName = "StatSystemProSave", int32 UserIndex = 0);

	/**
	 * Load only stats (for selective loading)
	 * BLUEPRINT: Load just the stat component
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save/Load|Individual", meta=(
		DisplayName = "Load Stats Only",
		Tooltip = "Load only stat component data"
	))
	static bool LoadStatsOnly(UStatComponent* StatComp, const FString& SaveSlotName = "StatSystemProSave", int32 UserIndex = 0);

	/**
	 * Create a new empty save game object
	 * BLUEPRINT: Advanced - create save game manually
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save/Load|Advanced", meta=(
		DisplayName = "Create Save Game Object",
		Tooltip = "Create a new save game object (advanced users)"
	))
	static UStatSystemSaveGame* CreateSaveGameObject();

private:
	// Helper functions for serialization
	static void SerializeStatComponent(UStatComponent* StatComp, FSavedStatData& OutData);
	static void DeserializeStatComponent(UStatComponent* StatComp, const FSavedStatData& InData);
};
