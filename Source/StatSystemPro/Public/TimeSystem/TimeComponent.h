// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimeSystem/TimeTypes.h"
#include "TimeComponent.generated.h"

// Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewDay, int32, DayNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewHour, int32, Hour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeOfDayChanged, ETimeOfDay, OldTimeOfDay, ETimeOfDay, NewTimeOfDay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeasonChanged, ESeason, NewSeason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDusk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMidnight);

/**
 * ============================================================================
 * TIME COMPONENT - Global Time and Day/Night Cycle System
 * ============================================================================
 *
 * Manages game time progression, day/night cycles, and seasons
 *
 * FEATURES:
 * - Day/night cycle tracking
 * - Customizable time speed
 * - Season system (optional)
 * - Time-based events
 * - Multiplayer synchronized
 * - Pause/resume support
 *
 * USAGE:
 * - Add to GameState or GameMode for global time
 * - Configure time speed (real seconds per game hour)
 * - Bind to events for time-based gameplay
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent, DisplayName="Time Component (Day/Night Cycle)"))
class STATSYSTEMPRO_API UTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTimeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	// ========== CONFIGURATION ==========

	/**
	 * Enable/Disable time progression
	 * BLUEPRINT: Toggle to pause/resume time
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Time System|Configuration", meta=(
		DisplayName = "Enable Time Progression",
		Tooltip = "Turn ON to enable time progression. Turn OFF to pause time."
	))
	bool bEnabled;

	/**
	 * Time progression settings
	 * BLUEPRINT: Configure how fast time passes
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time System|Configuration", meta=(
		DisplayName = "Time Settings",
		Tooltip = "Configure time speed, day/night cycle, and seasons"
	))
	FTimeProgressionSettings TimeSettings;

	/**
	 * Current game time - READ ONLY
	 * BLUEPRINT: Use getter functions instead
	 * MULTIPLAYER: Replicated to all clients
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GameTime, Category = "Time System|Time", meta=(
		DisplayName = "Current Time (Read Only)",
		Tooltip = "Current game time. Use getter functions for safe access."
	))
	FGameTime CurrentTime;

	// ========== EVENTS ==========

	/** Fires when a new day begins (at midnight) */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events", meta=(
		DisplayName = "On New Day",
		Tooltip = "Fires at midnight when day changes"
	))
	FOnNewDay OnNewDay;

	/** Fires every hour */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events", meta=(
		DisplayName = "On New Hour",
		Tooltip = "Fires every game hour"
	))
	FOnNewHour OnNewHour;

	/** Fires when time of day period changes (dawn, noon, dusk, etc.) */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events", meta=(
		DisplayName = "On Time of Day Changed",
		Tooltip = "Fires when entering new time period (dawn, noon, etc.)"
	))
	FOnTimeOfDayChanged OnTimeOfDayChanged;

	/** Fires when season changes */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events", meta=(
		DisplayName = "On Season Changed",
		Tooltip = "Fires when season changes (if seasons enabled)"
	))
	FOnSeasonChanged OnSeasonChanged;

	/** Fires at dawn (6:00) */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events|Specific Times", meta=(
		DisplayName = "On Dawn (6:00)",
		Tooltip = "Fires at 6:00 AM"
	))
	FOnDawn OnDawn;

	/** Fires at noon (12:00) */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events|Specific Times", meta=(
		DisplayName = "On Noon (12:00)",
		Tooltip = "Fires at 12:00 PM"
	))
	FOnNoon OnNoon;

	/** Fires at dusk (18:00) */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events|Specific Times", meta=(
		DisplayName = "On Dusk (18:00)",
		Tooltip = "Fires at 6:00 PM"
	))
	FOnDusk OnDusk;

	/** Fires at midnight (00:00) */
	UPROPERTY(BlueprintAssignable, Category = "Time System|Events|Specific Times", meta=(
		DisplayName = "On Midnight (00:00)",
		Tooltip = "Fires at midnight"
	))
	FOnMidnight OnMidnight;

	// ========== TIME CONTROL ==========

	/**
	 * Set time to specific day/hour/minute
	 * BLUEPRINT: Jump to a specific time
	 * MULTIPLAYER: Server only
	 */
	UFUNCTION(BlueprintCallable, Category = "Time System|Control", meta=(
		DisplayName = "Set Time",
		Tooltip = "Set time to specific day, hour, and minute"
	))
	void SetTime(int32 Day, int32 Hour, int32 Minute);

	/**
	 * Advance time by hours
	 * BLUEPRINT: Skip forward in time
	 */
	UFUNCTION(BlueprintCallable, Category = "Time System|Control", meta=(
		DisplayName = "Advance Time (Hours)",
		Tooltip = "Skip forward by specified number of hours"
	))
	void AdvanceTimeByHours(float Hours);

	/**
	 * Advance time by days
	 * BLUEPRINT: Skip forward in time
	 */
	UFUNCTION(BlueprintCallable, Category = "Time System|Control", meta=(
		DisplayName = "Advance Time (Days)",
		Tooltip = "Skip forward by specified number of days"
	))
	void AdvanceTimeByDays(int32 Days);

	/**
	 * Pause time progression
	 * BLUEPRINT: Freeze time
	 */
	UFUNCTION(BlueprintCallable, Category = "Time System|Control", meta=(
		DisplayName = "Pause Time",
		Tooltip = "Pause time progression"
	))
	void PauseTime();

	/**
	 * Resume time progression
	 * BLUEPRINT: Unfreeze time
	 */
	UFUNCTION(BlueprintCallable, Category = "Time System|Control", meta=(
		DisplayName = "Resume Time",
		Tooltip = "Resume time progression"
	))
	void ResumeTime();

	/**
	 * Set time speed multiplier
	 * BLUEPRINT: Make time pass faster or slower
	 *
	 * EXAMPLES:
	 * - 0.5 = Half speed (slower)
	 * - 1.0 = Normal speed
	 * - 2.0 = Double speed (faster)
	 */
	UFUNCTION(BlueprintCallable, Category = "Time System|Control", meta=(
		DisplayName = "Set Time Speed Multiplier",
		Tooltip = "Multiply time speed. 1.0 = normal, 2.0 = double speed, 0.5 = half speed"
	))
	void SetTimeSpeedMultiplier(float Multiplier);

	// ========== TIME GETTERS ==========

	/**
	 * Get current day number
	 * BLUEPRINT: What day is it?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters", meta=(
		DisplayName = "Get Current Day",
		Tooltip = "Get current day number (starts at 1)",
		CompactNodeTitle = "Day"
	))
	int32 GetCurrentDay() const;

	/**
	 * Get current hour (0-23)
	 * BLUEPRINT: What hour is it?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters", meta=(
		DisplayName = "Get Current Hour",
		Tooltip = "Get current hour (0-23)",
		CompactNodeTitle = "Hour"
	))
	int32 GetCurrentHour() const;

	/**
	 * Get current minute (0-59)
	 * BLUEPRINT: What minute is it?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters", meta=(
		DisplayName = "Get Current Minute",
		Tooltip = "Get current minute (0-59)",
		CompactNodeTitle = "Minute"
	))
	int32 GetCurrentMinute() const;

	/**
	 * Get time as formatted string
	 * BLUEPRINT: Display time in UI
	 *
	 * RETURNS: "Day 5, 14:30"
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters", meta=(
		DisplayName = "Get Time String",
		Tooltip = "Get formatted time string (e.g., 'Day 5, 14:30')"
	))
	FString GetTimeString() const;

	/**
	 * Get just time without day
	 * BLUEPRINT: Display clock time
	 *
	 * RETURNS: "14:30"
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters", meta=(
		DisplayName = "Get Clock Time String",
		Tooltip = "Get time as clock string (e.g., '14:30')"
	))
	FString GetClockTimeString() const;

	/**
	 * Get current time of day period
	 * BLUEPRINT: Is it dawn? Noon? Dusk?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters", meta=(
		DisplayName = "Get Time of Day Period",
		Tooltip = "Get current time period (Night, Dawn, Morning, Noon, etc.)"
	))
	ETimeOfDay GetTimeOfDayPeriod() const;

	/**
	 * Get current season
	 * BLUEPRINT: What season is it?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters", meta=(
		DisplayName = "Get Current Season",
		Tooltip = "Get current season (if seasons enabled)"
	))
	ESeason GetCurrentSeason() const;

	/**
	 * Check if it's daytime
	 * BLUEPRINT: Is the sun up?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters|Checks", meta=(
		DisplayName = "Is Daytime?",
		Tooltip = "Check if it's daytime (6:00 - 18:00)"
	))
	bool IsDaytime() const;

	/**
	 * Check if it's nighttime
	 * BLUEPRINT: Is it dark?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters|Checks", meta=(
		DisplayName = "Is Nighttime?",
		Tooltip = "Check if it's nighttime (18:00 - 6:00)"
	))
	bool IsNighttime() const;

	/**
	 * Get day/night cycle progress (0-1)
	 * BLUEPRINT: For sun position, lighting interpolation
	 *
	 * RETURNS:
	 * - 0.0 = Midnight
	 * - 0.25 = 6:00 AM
	 * - 0.5 = Noon
	 * - 0.75 = 6:00 PM
	 * - 1.0 = Midnight again
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters|Advanced", meta=(
		DisplayName = "Get Day Progress (0-1)",
		Tooltip = "Get progress through day as 0-1 value. Perfect for sun position!"
	))
	float GetDayProgress() const;

	/**
	 * Get total elapsed time in hours
	 * BLUEPRINT: How many hours have passed since game start?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters|Advanced", meta=(
		DisplayName = "Get Total Elapsed Hours",
		Tooltip = "Get total elapsed time in hours since game start"
	))
	float GetTotalElapsedHours() const;

	/**
	 * Get total elapsed time in days
	 * BLUEPRINT: How many days have passed since game start?
	 */
	UFUNCTION(BlueprintPure, Category = "Time System|Getters|Advanced", meta=(
		DisplayName = "Get Total Elapsed Days",
		Tooltip = "Get total elapsed time in days since game start"
	))
	float GetTotalElapsedDays() const;

private:
	/** Time speed multiplier */
	UPROPERTY(Replicated)
	float TimeSpeedMultiplier;

	/** Previous time of day for change detection */
	ETimeOfDay PreviousTimeOfDay;

	/** Previous hour for event firing */
	int32 PreviousHour;

	/** Previous day for event firing */
	int32 PreviousDay;

	/** Update time progression */
	void UpdateTime(float DeltaTime);

	/** Check and fire time-based events */
	void CheckTimeEvents();

	/** Replication callback */
	UFUNCTION()
	void OnRep_GameTime();
};
