// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeatherTypes.generated.h"

/**
 * Weather condition types
 */
UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	Clear UMETA(DisplayName = "Clear/Sunny"),
	Cloudy UMETA(DisplayName = "Cloudy"),
	Overcast UMETA(DisplayName = "Overcast"),
	LightRain UMETA(DisplayName = "Light Rain"),
	Rain UMETA(DisplayName = "Rain"),
	HeavyRain UMETA(DisplayName = "Heavy Rain"),
	Thunderstorm UMETA(DisplayName = "Thunderstorm"),
	LightSnow UMETA(DisplayName = "Light Snow"),
	Snow UMETA(DisplayName = "Snow"),
	HeavySnow UMETA(DisplayName = "Heavy Snow/Blizzard"),
	Fog UMETA(DisplayName = "Fog"),
	Sandstorm UMETA(DisplayName = "Sandstorm"),
	Heatwave UMETA(DisplayName = "Heatwave"),

	MAX UMETA(Hidden)
};

/**
 * Clothing slot types
 */
UENUM(BlueprintType)
enum class EClothingSlot : uint8
{
	Head UMETA(DisplayName = "Head (Hat/Helmet)"),
	Torso UMETA(DisplayName = "Torso (Shirt/Jacket)"),
	Legs UMETA(DisplayName = "Legs (Pants)"),
	Feet UMETA(DisplayName = "Feet (Boots)"),
	Hands UMETA(DisplayName = "Hands (Gloves)"),
	Back UMETA(DisplayName = "Back (Cloak/Cape)"),

	MAX UMETA(Hidden)
};

/**
 * Freezing severity stages
 */
UENUM(BlueprintType)
enum class EFreezingStage : uint8
{
	None UMETA(DisplayName = "Not Freezing"),
	Chilled UMETA(DisplayName = "Stage 1: Chilled"),
	Cold UMETA(DisplayName = "Stage 2: Cold"),
	Freezing UMETA(DisplayName = "Stage 3: Freezing"),
	Hypothermia UMETA(DisplayName = "Stage 4: Hypothermia"),
	CriticalHypothermia UMETA(DisplayName = "Stage 5: Critical (Near Death)"),

	MAX UMETA(Hidden)
};

/**
 * Overheating severity stages
 */
UENUM(BlueprintType)
enum class EOverheatingStage : uint8
{
	None UMETA(DisplayName = "Not Overheating"),
	Warm UMETA(DisplayName = "Stage 1: Warm"),
	Hot UMETA(DisplayName = "Stage 2: Hot"),
	Overheating UMETA(DisplayName = "Stage 3: Overheating"),
	Heatstroke UMETA(DisplayName = "Stage 4: Heatstroke"),
	CriticalHeatstroke UMETA(DisplayName = "Stage 5: Critical (Near Death)"),

	MAX UMETA(Hidden)
};

/**
 * Clothing item data
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FClothingItem
{
	GENERATED_BODY()

	/** Display name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FText DisplayName;

	/** Which slot this goes in */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EClothingSlot Slot;

	/** Cold insulation (0-100, higher = better cold protection) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Temperature")
	float ColdInsulation;

	/** Heat insulation (0-100, higher = better heat protection) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Temperature")
	float HeatInsulation;

	/** Water resistance (0-100, higher = less wetness absorption) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Weather")
	float WaterResistance;

	/** Wind resistance (0-100, higher = less wind chill) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Weather")
	float WindResistance;

	/** Current wetness (0-100, affects insulation) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|State")
	float CurrentWetness;

	/** Current durability (0-100, degrades over time) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|State")
	float Durability;

	/** How much this weighs (affects movement) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing|Stats")
	float Weight;

	FClothingItem()
		: Slot(EClothingSlot::Torso)
		, ColdInsulation(20.0f)
		, HeatInsulation(10.0f)
		, WaterResistance(10.0f)
		, WindResistance(10.0f)
		, CurrentWetness(0.0f)
		, Durability(100.0f)
		, Weight(1.0f)
	{
	}

	/** Get effective cold insulation (reduced when wet) */
	float GetEffectiveColdInsulation() const
	{
		// Wet clothing is less effective
		float WetnessPenalty = CurrentWetness / 100.0f; // 0-1
		float EffectiveInsulation = ColdInsulation * (1.0f - WetnessPenalty * 0.7f); // Up to 70% less effective when soaked
		return EffectiveInsulation * (Durability / 100.0f); // Damaged clothing less effective
	}

	/** Get effective heat insulation */
	float GetEffectiveHeatInsulation() const
	{
		return HeatInsulation * (Durability / 100.0f);
	}
};

/**
 * Weather preset configuration
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FWeatherPreset
{
	GENERATED_BODY()

	/** Preset name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset")
	FName PresetName;

	/** Weather type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset")
	EWeatherType WeatherType;

	/** Base temperature (Celsius) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Temperature")
	float BaseTemperature;

	/** Temperature variance (+/- random) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Temperature")
	float TemperatureVariance;

	/** Wind speed (m/s) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Wind")
	float WindSpeed;

	/** Wind variance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Wind")
	float WindVariance;

	/** Rain intensity (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Precipitation")
	float RainIntensity;

	/** Snow intensity (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Precipitation")
	float SnowIntensity;

	/** Fog density (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Visibility")
	float FogDensity;

	/** Visibility multiplier (1.0 = normal, 0.5 = half visibility) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Visibility")
	float VisibilityMultiplier;

	/** Minimum duration (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Duration")
	float MinDuration;

	/** Maximum duration (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Preset|Duration")
	float MaxDuration;

	FWeatherPreset()
		: PresetName(NAME_None)
		, WeatherType(EWeatherType::Clear)
		, BaseTemperature(20.0f)
		, TemperatureVariance(5.0f)
		, WindSpeed(5.0f)
		, WindVariance(2.0f)
		, RainIntensity(0.0f)
		, SnowIntensity(0.0f)
		, FogDensity(0.0f)
		, VisibilityMultiplier(1.0f)
		, MinDuration(300.0f)
		, MaxDuration(900.0f)
	{
	}
};

/**
 * Temperature calculation result
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FTemperatureResult
{
	GENERATED_BODY()

	/** Ambient temperature */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float AmbientTemperature;

	/** Effective temperature (feels like) */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float EffectiveTemperature;

	/** Wind chill adjustment */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float WindChillAdjustment;

	/** Wetness penalty */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float WetnessPenalty;

	/** Clothing protection */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float ClothingProtection;

	/** Shelter bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float ShelterBonus;

	/** Total insulation value */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float TotalInsulation;

	/** Freezing risk level (0-1) */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float FreezingRisk;

	/** Overheating risk level (0-1) */
	UPROPERTY(BlueprintReadOnly, Category = "Temperature")
	float OverheatingRisk;

	FTemperatureResult()
		: AmbientTemperature(20.0f)
		, EffectiveTemperature(20.0f)
		, WindChillAdjustment(0.0f)
		, WetnessPenalty(0.0f)
		, ClothingProtection(0.0f)
		, ShelterBonus(0.0f)
		, TotalInsulation(0.0f)
		, FreezingRisk(0.0f)
		, OverheatingRisk(0.0f)
	{
	}
};
