// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatLayer/StatTypes.h"
#include "OverrideTypes.generated.h"

/**
 * Data table row for overriding default stat configurations
 * USAGE: Create Data Table → Row Type: FStatOverrideRow
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FStatOverrideRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Which stat to override */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override")
	EStatType StatType;

	/** Override default value? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override")
	bool bOverrideDefaultValue;

	/** Default starting value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override", meta=(EditCondition="bOverrideDefaultValue"))
	float DefaultValue;

	/** Override max value? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override")
	bool bOverrideMaxValue;

	/** Default maximum value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override", meta=(EditCondition="bOverrideMaxValue"))
	float DefaultMaxValue;

	/** Override regeneration rate? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override")
	bool bOverrideRegenerationRate;

	/** Default regeneration rate per second (can be negative for decay) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override", meta=(EditCondition="bOverrideRegenerationRate"))
	float DefaultRegenerationRate;

	/** Override regeneration curve? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override")
	bool bOverrideRegenerationCurve;

	/** Optional regeneration curve (X=percentage, Y=regen rate) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Override", meta=(EditCondition="bOverrideRegenerationCurve"))
	class UCurveFloat* RegenerationCurve;

	FStatOverrideRow()
		: StatType(EStatType::Health_Core)
		, bOverrideDefaultValue(false)
		, DefaultValue(100.0f)
		, bOverrideMaxValue(false)
		, DefaultMaxValue(100.0f)
		, bOverrideRegenerationRate(false)
		, DefaultRegenerationRate(0.0f)
		, bOverrideRegenerationCurve(false)
		, RegenerationCurve(nullptr)
	{
	}
};

/**
 * Data table row for overriding body part configurations
 * USAGE: Create Data Table → Row Type: FBodyPartOverrideRow
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FBodyPartOverrideRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Body part name (e.g., "Head", "Torso", "LeftArm") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part Override")
	FName BodyPartName;

	/** Display name for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part Override")
	FText DisplayName;

	/** Default max condition */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part Override")
	float DefaultMaxCondition;

	/** Damage multiplier (1.0 = normal, 2.0 = double damage) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part Override")
	float DamageMultiplier;

	/** Auto-fracture threshold (0-1, fractures when condition drops below this) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part Override")
	float AutoFractureThreshold;

	/** Can this part be fractured? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part Override")
	bool bCanFracture;

	/** Can this part bleed? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part Override")
	bool bCanBleed;

	FBodyPartOverrideRow()
		: BodyPartName(NAME_None)
		, DisplayName(FText::FromString("Body Part"))
		, DefaultMaxCondition(100.0f)
		, DamageMultiplier(1.0f)
		, AutoFractureThreshold(0.2f)
		, bCanFracture(true)
		, bCanBleed(true)
	{
	}
};

/**
 * Data table row for overriding environment thresholds
 * USAGE: Create Data Table → Row Type: FEnvironmentOverrideRow
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FEnvironmentOverrideRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Override setting name (e.g., "HypothermiaThreshold") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Override")
	FName SettingName;

	/** Override value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Override")
	float OverrideValue;

	/** Description of what this override does */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Override")
	FText Description;

	FEnvironmentOverrideRow()
		: SettingName(NAME_None)
		, OverrideValue(0.0f)
	{
	}
};

/**
 * Data table row for defining custom stats beyond the default set
 * USAGE: For games that need MORE stats than the 18 default ones
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FCustomStatDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Unique stat ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	FName StatID;

	/** Display name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	FText DisplayName;

	/** Description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	FText Description;

	/** Which category does this belong to? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	EStatCategory Category;

	/** Starting value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	float DefaultValue;

	/** Maximum value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	float MaxValue;

	/** Regeneration rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	float RegenerationRate;

	/** Regeneration curve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	class UCurveFloat* RegenerationCurve;

	/** Icon for UI (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	class UTexture2D* Icon;

	/** Color for UI (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Stat")
	FLinearColor UIColor;

	FCustomStatDefinitionRow()
		: StatID(NAME_None)
		, Category(EStatCategory::Core)
		, DefaultValue(100.0f)
		, MaxValue(100.0f)
		, RegenerationRate(0.0f)
		, RegenerationCurve(nullptr)
		, Icon(nullptr)
		, UIColor(FLinearColor::White)
	{
	}
};

/**
 * Data table row for time progression overrides
 * USAGE: Override default time system behavior
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FTimeSystemOverrideRow : public FTableRowBase
{
	GENERATED_BODY()

	/** Real seconds per game hour */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Override")
	float RealSecondsPerGameHour;

	/** Enable day/night cycle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Override")
	bool bEnableDayNightCycle;

	/** Enable seasons */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Override")
	bool bEnableSeasons;

	/** Days per season */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Override")
	int32 DaysPerSeason;

	/** Starting day */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Override")
	int32 StartingDay;

	/** Starting hour (0-23) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Override")
	int32 StartingHour;

	FTimeSystemOverrideRow()
		: RealSecondsPerGameHour(60.0f)
		, bEnableDayNightCycle(true)
		, bEnableSeasons(false)
		, DaysPerSeason(30)
		, StartingDay(1)
		, StartingHour(12)
	{
	}
};
