// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimeTypes.generated.h"

/**
 * Time of day periods
 */
UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Night UMETA(DisplayName = "Night (00:00 - 05:59)"),
	Dawn UMETA(DisplayName = "Dawn (06:00 - 08:59)"),
	Morning UMETA(DisplayName = "Morning (09:00 - 11:59)"),
	Noon UMETA(DisplayName = "Noon (12:00 - 14:59)"),
	Afternoon UMETA(DisplayName = "Afternoon (15:00 - 17:59)"),
	Dusk UMETA(DisplayName = "Dusk (18:00 - 20:59)"),
	Evening UMETA(DisplayName = "Evening (21:00 - 23:59)"),

	MAX UMETA(Hidden)
};

/**
 * Day of week
 */
UENUM(BlueprintType)
enum class EDayOfWeek : uint8
{
	Monday UMETA(DisplayName = "Monday"),
	Tuesday UMETA(DisplayName = "Tuesday"),
	Wednesday UMETA(DisplayName = "Wednesday"),
	Thursday UMETA(DisplayName = "Thursday"),
	Friday UMETA(DisplayName = "Friday"),
	Saturday UMETA(DisplayName = "Saturday"),
	Sunday UMETA(DisplayName = "Sunday"),

	MAX UMETA(Hidden)
};

/**
 * Season
 */
UENUM(BlueprintType)
enum class ESeason : uint8
{
	Spring UMETA(DisplayName = "Spring"),
	Summer UMETA(DisplayName = "Summer"),
	Autumn UMETA(DisplayName = "Autumn/Fall"),
	Winter UMETA(DisplayName = "Winter"),

	MAX UMETA(Hidden)
};

/**
 * Struct representing a specific time
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FGameTime
{
	GENERATED_BODY()

	/** Current day (starts at 1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Day;

	/** Current hour (0-23) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Hour;

	/** Current minute (0-59) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int32 Minute;

	/** Current second (0-59) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float Second;

	/** Total elapsed seconds since game start */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TotalSeconds;

	FGameTime()
		: Day(1)
		, Hour(12)  // Start at noon
		, Minute(0)
		, Second(0.0f)
		, TotalSeconds(0.0f)
	{
	}

	/** Get total time in hours */
	float GetTotalHours() const
	{
		return TotalSeconds / 3600.0f;
	}

	/** Get total time in days */
	float GetTotalDays() const
	{
		return TotalSeconds / 86400.0f;  // 86400 seconds in a day
	}

	/** Get as formatted string */
	FString ToString() const
	{
		return FString::Printf(TEXT("Day %d, %02d:%02d"), Day, Hour, Minute);
	}

	/** Get time as 24-hour format */
	FString GetTimeString() const
	{
		return FString::Printf(TEXT("%02d:%02d"), Hour, Minute);
	}

	/** Check if it's daytime (6:00 - 18:00) */
	bool IsDaytime() const
	{
		return Hour >= 6 && Hour < 18;
	}

	/** Check if it's nighttime */
	bool IsNighttime() const
	{
		return !IsDaytime();
	}

	/** Get time of day period */
	ETimeOfDay GetTimeOfDay() const
	{
		if (Hour >= 0 && Hour < 6) return ETimeOfDay::Night;
		if (Hour >= 6 && Hour < 9) return ETimeOfDay::Dawn;
		if (Hour >= 9 && Hour < 12) return ETimeOfDay::Morning;
		if (Hour >= 12 && Hour < 15) return ETimeOfDay::Noon;
		if (Hour >= 15 && Hour < 18) return ETimeOfDay::Afternoon;
		if (Hour >= 18 && Hour < 21) return ETimeOfDay::Dusk;
		return ETimeOfDay::Evening;
	}
};

/**
 * Time progression settings
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FTimeProgressionSettings
{
	GENERATED_BODY()

	/** How many real seconds = 1 game hour (default 60 = 1 minute real time = 1 hour game time) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	float RealSecondsPerGameHour;

	/** Enable day/night cycle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	bool bEnableDayNightCycle;

	/** Enable season system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	bool bEnableSeasons;

	/** Days per season (default 30) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	int32 DaysPerSeason;

	FTimeProgressionSettings()
		: RealSecondsPerGameHour(60.0f)  // 1 real minute = 1 game hour
		, bEnableDayNightCycle(true)
		, bEnableSeasons(false)
		, DaysPerSeason(30)
	{
	}
};
