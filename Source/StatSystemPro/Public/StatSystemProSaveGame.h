// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StatLayer/StatTypes.h"
#include "BodyLayer/BodyTypes.h"
#include "StatusEffectLayer/StatusEffectTypes.h"
#include "WeatherSystem/WeatherTypes.h"
#include "StatSystemProSaveGame.generated.h"

/**
 * ============================================================================
 * STATSYSTEMPRO SAVE GAME
 * ============================================================================
 *
 * Enhanced save system that saves ALL layers in one call.
 * Works seamlessly with the unified StatSystemProComponent.
 *
 * USAGE:
 * Component->QuickSave("MySlot");  // Saves everything
 * Component->QuickLoad("MySlot");  // Loads everything
 *
 * WHAT GETS SAVED:
 * - Layer toggles (which layers are enabled)
 * - All stat values, max values, regen rates
 * - All body part states (health, fractures, bleeding, etc.)
 * - Current weather, temperature, wind, clothing
 * - All active status effects
 * - Progression data (level, XP, skills, stat points)
 * - Time system state (current day, hour, time of day)
 */
UCLASS()
class STATSYSTEMPRO_API UStatSystemProSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UStatSystemProSaveGame();

	// ========================================================================
	// LAYER TOGGLES
	// ========================================================================

	/** Is Stat Layer enabled? */
	UPROPERTY(SaveGame)
	bool bEnableStatLayer;

	/** Is Body Layer enabled? */
	UPROPERTY(SaveGame)
	bool bEnableBodyLayer;

	/** Is Weather Layer enabled? */
	UPROPERTY(SaveGame)
	bool bEnableWeatherLayer;

	/** Is Status Effect Layer enabled? */
	UPROPERTY(SaveGame)
	bool bEnableStatusEffectLayer;

	/** Is Progression Layer enabled? */
	UPROPERTY(SaveGame)
	bool bEnableProgressionLayer;

	/** Is Time Layer enabled? */
	UPROPERTY(SaveGame)
	bool bEnableTimeLayer;

	// ========================================================================
	// STAT LAYER DATA
	// ========================================================================

	/** All saved stats */
	UPROPERTY(SaveGame)
	TMap<EStatType, FStatValue> Stats;

	// ========================================================================
	// BODY LAYER DATA
	// ========================================================================

	/** All saved body parts */
	UPROPERTY(SaveGame)
	TMap<EBodyPart, FBodyPartState> BodyParts;

	// ========================================================================
	// WEATHER LAYER DATA
	// ========================================================================

	/** Current weather type */
	UPROPERTY(SaveGame)
	EWeatherType CurrentWeather;

	/** Ambient temperature (Celsius) */
	UPROPERTY(SaveGame)
	float AmbientTemperature;

	/** Wind speed (m/s) */
	UPROPERTY(SaveGame)
	float WindSpeed;

	/** Wetness level (0-100%) */
	UPROPERTY(SaveGame)
	float WetnessLevel;

	/** Shelter level (0-100%) */
	UPROPERTY(SaveGame)
	float ShelterLevel;

	/** Equipped clothing */
	UPROPERTY(SaveGame)
	TMap<EClothingSlot, FClothingItem> EquippedClothing;

	// ========================================================================
	// STATUS EFFECT LAYER DATA
	// ========================================================================

	/** All active status effects */
	UPROPERTY(SaveGame)
	TArray<FActiveStatusEffect> ActiveEffects;

	// ========================================================================
	// PROGRESSION LAYER DATA
	// ========================================================================

	/** Current level */
	UPROPERTY(SaveGame)
	int32 CurrentLevel;

	/** Current XP */
	UPROPERTY(SaveGame)
	int32 CurrentXP;

	/** Available stat points */
	UPROPERTY(SaveGame)
	int32 StatPoints;

	/** Unlocked skills */
	UPROPERTY(SaveGame)
	TArray<FName> UnlockedSkills;

	// ========================================================================
	// TIME LAYER DATA
	// ========================================================================

	/** Current game time (in seconds) */
	UPROPERTY(SaveGame)
	float CurrentGameTime;

	/** Current hour (0-23) */
	UPROPERTY(SaveGame)
	int32 CurrentHour;

	/** Current day */
	UPROPERTY(SaveGame)
	int32 CurrentDay;
};
