// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Curves/CurveFloat.h"
#include "StatTypes.generated.h"

/**
 * Enum for all available stat types
 * BLUEPRINT: Use this enum to select stats - NO TEXT TYPING!
 */
UENUM(BlueprintType)
enum class EStatType : uint8
{
	// Core Stats
	Health_Core UMETA(DisplayName = "Health Core"),
	Stamina UMETA(DisplayName = "Stamina"),
	Energy UMETA(DisplayName = "Energy"),

	// Survival Stats
	Hunger UMETA(DisplayName = "Hunger"),
	Thirst UMETA(DisplayName = "Thirst"),
	Fatigue UMETA(DisplayName = "Fatigue"),

	// Environmental Stats
	BodyTemperature UMETA(DisplayName = "Body Temperature"),
	Wetness UMETA(DisplayName = "Wetness"),

	// Health-Related Stats (Vital Signs)
	HeartRate UMETA(DisplayName = "Heart Rate (BPM)"),
	BloodLevel UMETA(DisplayName = "Blood Level"),
	BloodPressure UMETA(DisplayName = "Blood Pressure"),
	Sanity UMETA(DisplayName = "Sanity"),
	Infection_Level UMETA(DisplayName = "Infection Level"),
	Toxicity UMETA(DisplayName = "Toxicity"),

	// RPG Stats (Optional)
	Strength UMETA(DisplayName = "Strength"),
	Dexterity UMETA(DisplayName = "Dexterity"),
	Intelligence UMETA(DisplayName = "Intelligence"),
	Endurance UMETA(DisplayName = "Endurance"),

	MAX UMETA(Hidden)
};

/**
 * Stat Categories for batch operations
 * BLUEPRINT: Select a category to work with multiple related stats at once!
 */
UENUM(BlueprintType)
enum class EStatCategory : uint8
{
	/** Health, Stamina, Energy - Core combat/gameplay stats */
	Core UMETA(DisplayName = "Core Stats (Health, Stamina, Energy)"),

	/** Hunger, Thirst, Fatigue - Survival needs */
	Survival UMETA(DisplayName = "Survival Stats (Hunger, Thirst, Fatigue)"),

	/** Body Temperature, Wetness - Environmental factors */
	Environmental UMETA(DisplayName = "Environmental Stats (Temperature, Wetness)"),

	/** Heart Rate, Blood Level, Blood Pressure, Sanity, Infection, Toxicity - Health conditions and vital signs */
	HealthConditions UMETA(DisplayName = "Health Conditions (Heart Rate, Blood, Pressure, Sanity, Infection, Toxicity)"),

	/** Strength, Dexterity, Intelligence, Endurance - RPG attributes */
	RPGAttributes UMETA(DisplayName = "RPG Attributes (Str, Dex, Int, End)"),

	/** All stats */
	All UMETA(DisplayName = "All Stats"),

	MAX UMETA(Hidden)
};

/**
 * Struct representing a single stat with current and max values
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FStatValue
{
	GENERATED_BODY()

	/** Current value of the stat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentValue;

	/** Maximum value this stat can reach */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxValue;

	/** Base maximum value (before modifiers) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float BaseMaxValue;

	/** Regeneration rate per second (can be negative for decay) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RegenerationRate;

	/** Optional curve for dynamic regeneration/decay over time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	UCurveFloat* RegenerationCurve;

	FStatValue()
		: CurrentValue(100.0f)
		, MaxValue(100.0f)
		, BaseMaxValue(100.0f)
		, RegenerationRate(0.0f)
		, RegenerationCurve(nullptr)
	{
	}

	FStatValue(float InValue)
		: CurrentValue(InValue)
		, MaxValue(InValue)
		, BaseMaxValue(InValue)
		, RegenerationRate(0.0f)
		, RegenerationCurve(nullptr)
	{
	}

	/** Clamp current value to [0, MaxValue] */
	void Clamp()
	{
		CurrentValue = FMath::Clamp(CurrentValue, 0.0f, MaxValue);
	}

	/** Get percentage (0-1) */
	float GetPercentage() const
	{
		return MaxValue > 0.0f ? CurrentValue / MaxValue : 0.0f;
	}

	/** Check if stat is at max */
	bool IsAtMax() const
	{
		return FMath::IsNearlyEqual(CurrentValue, MaxValue);
	}

	/** Check if stat is at zero */
	bool IsAtZero() const
	{
		return FMath::IsNearlyZero(CurrentValue);
	}
};

/**
 * Data table row for configuring default stat values
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FStatConfigRow : public FTableRowBase
{
	GENERATED_BODY()

	/** The stat type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	EStatType StatType;

	/** Default maximum value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float DefaultMaxValue;

	/** Default regeneration rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float DefaultRegenerationRate;

	/** Optional regeneration curve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UCurveFloat* RegenerationCurve;

	FStatConfigRow()
		: StatType(EStatType::Health_Core)
		, DefaultMaxValue(100.0f)
		, DefaultRegenerationRate(0.0f)
		, RegenerationCurve(nullptr)
	{
	}
};

/**
 * Reason tag for stat changes (for logging and debugging)
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FStatChangeReason
{
	GENERATED_BODY()

	/** Source of the change (e.g., "Combat", "Environment", "Item") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FName Source;

	/** Specific tag for the change */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FGameplayTag ReasonTag;

	FStatChangeReason()
		: Source(NAME_None)
	{
	}

	FStatChangeReason(FName InSource, FGameplayTag InTag)
		: Source(InSource)
		, ReasonTag(InTag)
	{
	}
};
