// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StatLayer/StatTypes.h"
#include "TimeSystem/TimeTypes.h"
#include "StatSystemSaveGame.generated.h"

/**
 * Save data for all stats
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSavedStatData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TMap<EStatType, FStatValue> Stats;

	UPROPERTY(SaveGame)
	bool bEnabled;

	UPROPERTY(SaveGame)
	bool bUseSimpleMode;

	UPROPERTY(SaveGame)
	bool bEnableAutoRegeneration;

	UPROPERTY(SaveGame)
	float CriticalThreshold;
};

/**
 * Save data for body layer
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSavedBodyData
{
	GENERATED_BODY()

	// We'll serialize the body parts as arrays
	UPROPERTY(SaveGame)
	TArray<float> BodyPartConditions;

	UPROPERTY(SaveGame)
	TArray<bool> BodyPartFractures;

	UPROPERTY(SaveGame)
	TArray<float> BodyPartBleedingRates;

	UPROPERTY(SaveGame)
	TArray<uint8> BodyPartBurnLevels;

	UPROPERTY(SaveGame)
	TArray<float> BodyPartInfectionRates;

	UPROPERTY(SaveGame)
	TArray<float> BodyPartPainLevels;

	UPROPERTY(SaveGame)
	bool bEnabled;
};

/**
 * Save data for status effects
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSavedStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FName EffectID;

	UPROPERTY(SaveGame)
	int32 CurrentStacks;

	UPROPERTY(SaveGame)
	float TimeRemaining;

	UPROPERTY(SaveGame)
	bool bIsActive;
};

/**
 * Save data for status effect component
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSavedStatusEffectsData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FSavedStatusEffectData> ActiveEffects;

	UPROPERTY(SaveGame)
	bool bEnabled;
};

/**
 * Save data for environment layer
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSavedEnvironmentData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	float AmbientTemperature;

	UPROPERTY(SaveGame)
	float WindSpeed;

	UPROPERTY(SaveGame)
	float RainIntensity;

	UPROPERTY(SaveGame)
	float SnowIntensity;

	UPROPERTY(SaveGame)
	float ShelterLevel;

	UPROPERTY(SaveGame)
	float ClothingInsulation;

	UPROPERTY(SaveGame)
	float RadiationLevel;

	UPROPERTY(SaveGame)
	bool bEnabled;
};

/**
 * Save data for progression layer
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSavedProgressionData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	int32 CurrentLevel;

	UPROPERTY(SaveGame)
	int32 CurrentXP;

	UPROPERTY(SaveGame)
	int32 XPToNextLevel;

	UPROPERTY(SaveGame)
	int32 AvailableSkillPoints;

	// Skill name → Skill level
	UPROPERTY(SaveGame)
	TMap<FName, int32> LearnedSkills;

	UPROPERTY(SaveGame)
	bool bEnabled;
};

/**
 * Save data for time system
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSavedTimeData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FGameTime CurrentTime;

	UPROPERTY(SaveGame)
	FTimeProgressionSettings TimeSettings;

	UPROPERTY(SaveGame)
	bool bEnabled;

	UPROPERTY(SaveGame)
	float TimeSpeedMultiplier;
};

/**
 * Main save game class - saves EVERYTHING
 */
UCLASS()
class STATSYSTEMPRO_API UStatSystemSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UStatSystemSaveGame();

	// ========== SAVE DATA ==========

	/** Saved stat data */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FSavedStatData StatData;

	/** Saved body data */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FSavedBodyData BodyData;

	/** Saved status effects data */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FSavedStatusEffectsData StatusEffectsData;

	/** Saved environment data */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FSavedEnvironmentData EnvironmentData;

	/** Saved progression data */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FSavedProgressionData ProgressionData;

	/** Saved time data */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FSavedTimeData TimeData;

	// ========== METADATA ==========

	/** Save slot name */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FString SaveSlotName;

	/** When this save was created */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FDateTime SaveTimestamp;

	/** Player name (optional) */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	FString PlayerName;

	/** Save game version (for migration) */
	UPROPERTY(SaveGame, Category = "StatSystemPro")
	int32 SaveVersion;

	// ========== CONSTANTS ==========

	static const int32 CURRENT_SAVE_VERSION = 1;
	static const FString DEFAULT_SAVE_SLOT;
};
