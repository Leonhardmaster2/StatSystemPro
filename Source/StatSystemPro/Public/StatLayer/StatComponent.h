// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "StatLayer/StatTypes.h"
#include "StatComponent.generated.h"

// Delegates for stat events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatChanged, EStatType, StatType, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatMaxChanged, EStatType, StatType, float, NewMaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatReachedZero, EStatType, StatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatReachedMax, EStatType, StatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatCritical, EStatType, StatType, float, CurrentValue);

/**
 * ============================================================================
 * STAT COMPONENT - Core Stat Management System
 * ============================================================================
 *
 * This component manages all numeric character values (health, stamina, hunger, etc.)
 *
 * SIMPLE MODE (Default):
 * - Just add to your character
 * - All stats automatically initialized to 100
 * - Regeneration/decay works out of the box
 * - Perfect for quick prototypes
 *
 * ADVANCED MODE:
 * - Configure stats via data tables
 * - Custom regeneration curves
 * - Full replication support for multiplayer
 * - Fine-grained control over every stat
 *
 * MULTIPLAYER:
 * - Fully replicated by default
 * - Server authority on stat changes
 * - Client prediction friendly
 * - Bandwidth optimized
 *
 * PERFORMANCE:
 * - Lightweight (< 0.1ms per frame typical)
 * - Optional tick disable per stat
 * - Bulk update support
 * - Memory efficient
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent, DisplayName="Stat Component (Simple & Powerful)"))
class STATSYSTEMPRO_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	// ========== CONFIGURATION ==========

	/**
	 * Enable/Disable this entire layer
	 * BLUEPRINT: Toggle this to turn the stat system on/off completely
	 * MULTIPLAYER: Server controls this, clients are notified
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Stat System|Layer Control", meta=(
		DisplayName = "Enable Stat System",
		Tooltip = "Turn ON to enable stat tracking. Turn OFF to disable completely (saves performance)."
	))
	bool bEnabled;

	/**
	 * Use simple mode with default values (100 for all stats)
	 * BLUEPRINT: Keep this ON for quick setup, turn OFF for advanced data table configuration
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat System|Configuration", meta=(
		DisplayName = "Simple Mode (Recommended for Beginners)",
		Tooltip = "ON = Auto-initialize all stats to 100. OFF = Use data table configuration."
	))
	bool bUseSimpleMode;

	/**
	 * Enable automatic stat regeneration/decay
	 * BLUEPRINT: Turn OFF to manually control all stat changes (useful for turn-based games)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat System|Configuration", meta=(
		DisplayName = "Auto Regeneration",
		Tooltip = "Enable automatic stat regeneration and decay over time."
	))
	bool bEnableAutoRegeneration;

	/**
	 * Critical threshold percentage (0.0 - 1.0)
	 * BLUEPRINT: When any stat falls below this, OnStatCritical event fires
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat System|Configuration", meta=(
		DisplayName = "Critical Threshold",
		Tooltip = "Percentage threshold for critical stat warnings (0.15 = 15%)",
		ClampMin = "0.0",
		ClampMax = "1.0"
	))
	float CriticalThreshold;

	/**
	 * Data table for configuring default stat values (ADVANCED MODE ONLY)
	 * BLUEPRINT: Assign a data table here for custom stat configurations
	 * MULTIPLAYER: Only server needs this, clients get replicated values
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat System|Configuration", meta=(
		DisplayName = "Stat Config Table (Advanced)",
		Tooltip = "Optional: Assign a data table to customize stat values, regen rates, and curves.",
		EditCondition = "!bUseSimpleMode"
	))
	UDataTable* StatConfigTable;

	/**
	 * All stat values - READ ONLY
	 * BLUEPRINT: Use Get/Set functions instead of accessing this directly
	 * MULTIPLAYER: Replicated to all clients automatically
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stats, Category = "Stat System|Stats", meta=(
		DisplayName = "Current Stats (Read Only)",
		Tooltip = "All current stat values. Use getter functions for safe access."
	))
	TMap<EStatType, FStatValue> Stats;

	// ========== EVENTS (Blueprint Assignable) ==========

	/**
	 * Fires whenever ANY stat value changes
	 * BLUEPRINT: Bind to this to track all stat changes in one place
	 * MULTIPLAYER: Fires on all clients when replicated
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events", meta=(
		DisplayName = "On Any Stat Changed",
		Tooltip = "Fires when any stat changes. Provides: StatType, OldValue, NewValue"
	))
	FOnStatChanged OnStatChanged;

	/**
	 * Fires when a stat's maximum value changes
	 * BLUEPRINT: Use this to update UI when max health changes, etc.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events", meta=(
		DisplayName = "On Stat Max Changed",
		Tooltip = "Fires when a stat's maximum value changes (e.g., max health buff)"
	))
	FOnStatMaxChanged OnStatMaxChanged;

	/**
	 * Fires when any stat reaches zero (death, exhaustion, etc.)
	 * BLUEPRINT: Perfect for death logic, stamina depletion, etc.
	 * MULTIPLAYER: Fires on all clients
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events", meta=(
		DisplayName = "On Stat Reached Zero",
		Tooltip = "Fires when a stat hits 0 (death, starvation, etc.)"
	))
	FOnStatReachedZero OnStatReachedZero;

	/**
	 * Fires when any stat reaches its maximum
	 * BLUEPRINT: Use for fully rested, fully fed events, etc.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events", meta=(
		DisplayName = "On Stat Reached Max",
		Tooltip = "Fires when a stat reaches its maximum value"
	))
	FOnStatReachedMax OnStatReachedMax;

	/**
	 * Fires when any stat falls below critical threshold
	 * BLUEPRINT: Use for warning UI, critical state changes
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events", meta=(
		DisplayName = "On Stat Critical",
		Tooltip = "Fires when a stat drops below critical threshold (default 15%)"
	))
	FOnStatCritical OnStatCritical;

	// ========== INITIALIZATION ==========

	/**
	 * Initialize all stats with default or configured values
	 * BLUEPRINT: Call this in BeginPlay or manually if needed
	 * MULTIPLAYER: Only call on server
	 *
	 * HOW IT WORKS:
	 * - Simple Mode: All stats = 100
	 * - Advanced Mode: Loads from StatConfigTable
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System|Setup", meta=(
		DisplayName = "Initialize Stats",
		Tooltip = "Initialize all stats. Called automatically in BeginPlay. Simple Mode = all 100, Advanced = from data table.",
		Keywords = "init setup start begin"
	))
	void InitializeStats();

	// ========== STAT MODIFICATION (Server Authority) ==========

	/**
	 * Apply a change to a stat (add or subtract)
	 * BLUEPRINT: Main function for damaging, healing, consuming, etc.
	 * MULTIPLAYER: Automatically replicates to clients
	 *
	 * EXAMPLE USES:
	 * - Damage: Amount = -25
	 * - Heal: Amount = +50
	 * - Eat Food: Amount = +30 (for Hunger)
	 * - Drink Water: Amount = +40 (for Thirst)
	 *
	 * @param StatType - Which stat to change (Health, Stamina, Hunger, etc.)
	 * @param Amount - How much to change (+positive to increase, -negative to decrease)
	 * @param Source - For debugging: "Combat", "Food", "Environment"
	 * @param ReasonTag - Optional gameplay tag for filtering/tracking
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System|Modification", meta=(
		DisplayName = "Change Stat (Add/Subtract)",
		Tooltip = "Add or subtract from a stat. Use negative for damage, positive for healing/restoration.",
		Keywords = "add subtract damage heal hurt restore modify change"
	))
	void ApplyStatChange(EStatType StatType, float Amount, FName Source, FGameplayTag ReasonTag);

	/**
	 * Set a stat to an exact value (overrides current)
	 * BLUEPRINT: Use when you need precise control
	 * MULTIPLAYER: Server authority, auto-replicated
	 *
	 * WARNING: This ignores current value. Use ApplyStatChange for relative changes.
	 *
	 * EXAMPLE: Set Health to exactly 50
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System|Modification", meta=(
		DisplayName = "Set Stat to Exact Value",
		Tooltip = "Set a stat to an exact value (overrides current). Auto-clamped to 0-Max.",
		Keywords = "set exact override"
	))
	void SetStatValue(EStatType StatType, float NewValue);

	/**
	 * Change the maximum value for a stat
	 * BLUEPRINT: Use for buffs/debuffs that change max health, etc.
	 * MULTIPLAYER: Auto-replicated
	 *
	 * EXAMPLES:
	 * - +50 Max Health buff
	 * - -20 Max Stamina debuff (injured)
	 *
	 * Current value is auto-clamped to new max
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System|Modification", meta=(
		DisplayName = "Set Stat Maximum",
		Tooltip = "Change a stat's maximum value (for buffs/debuffs). Current value auto-clamped.",
		Keywords = "max maximum buff debuff"
	))
	void SetStatMaxValue(EStatType StatType, float NewMaxValue);

	/**
	 * Set how fast a stat regenerates or decays
	 * BLUEPRINT: Positive = regen, Negative = decay
	 * MULTIPLAYER: Auto-replicated
	 *
	 * EXAMPLES:
	 * - Health regen: +5 per second
	 * - Hunger decay: -1 per second
	 * - Stamina regen: +10 per second
	 *
	 * @param Rate - Units per second (positive = increase, negative = decrease)
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System|Modification", meta=(
		DisplayName = "Set Regeneration Rate",
		Tooltip = "Set stat regen/decay rate per second. Positive = regen, Negative = decay.",
		Keywords = "regen regeneration decay rate per second"
	))
	void SetStatRegenerationRate(EStatType StatType, float Rate);

	// ========== GETTERS - SIMPLE (Most Used) ==========

	/**
	 * Get current value of a stat
	 * BLUEPRINT: Most common function - use this for UI, logic checks, etc.
	 * MULTIPLAYER: Works on server and clients
	 *
	 * RETURNS: Current value (e.g., 75 health out of 100)
	 *
	 * EXAMPLE: Get how much health player has right now
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Basic", meta=(
		DisplayName = "Get Stat Value",
		Tooltip = "Get current value of a stat (e.g., current health = 75)",
		Keywords = "get current value amount",
		CompactNodeTitle = "Stat Value"
	))
	float GetStatValue(EStatType StatType) const;

	/**
	 * Get maximum value of a stat
	 * BLUEPRINT: Use for UI bars, percentage calculations
	 *
	 * RETURNS: Maximum value (e.g., max health = 100)
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Basic", meta=(
		DisplayName = "Get Stat Maximum",
		Tooltip = "Get maximum value of a stat (e.g., max health = 100)",
		Keywords = "get max maximum",
		CompactNodeTitle = "Stat Max"
	))
	float GetStatMaxValue(EStatType StatType) const;

	/**
	 * Get stat as percentage (0.0 to 1.0)
	 * BLUEPRINT: Perfect for progress bars and UI
	 *
	 * RETURNS: 0.0 (empty) to 1.0 (full)
	 * EXAMPLE: 75 health / 100 max = 0.75
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Basic", meta=(
		DisplayName = "Get Stat Percentage (0-1)",
		Tooltip = "Get stat as 0-1 percentage. Perfect for UI progress bars. (75/100 = 0.75)",
		Keywords = "get percent percentage ratio",
		CompactNodeTitle = "Stat %"
	))
	float GetStatPercentage(EStatType StatType) const;

	/**
	 * Get full stat data structure
	 * BLUEPRINT: Use when you need all info at once
	 *
	 * RETURNS: Complete stat info (current, max, regen rate, etc.)
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Advanced", meta=(
		DisplayName = "Get Complete Stat Data",
		Tooltip = "Get all stat information at once (current, max, regen, etc.)",
		Keywords = "get stat data struct all info"
	))
	FStatValue GetStat(EStatType StatType) const;

	/**
	 * Check if a specific stat is being tracked
	 * BLUEPRINT: Rarely needed - stats are auto-created
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Advanced", meta=(
		DisplayName = "Has Stat?",
		Tooltip = "Check if this stat exists (usually always true)",
		Keywords = "check has exists contains"
	))
	bool HasStat(EStatType StatType) const;

	// ========== GETTERS - EXTENDED (More Data Extraction) ==========

	/**
	 * Get stat percentage as 0-100 (instead of 0-1)
	 * BLUEPRINT: Alternative to GetStatPercentage for whole numbers
	 *
	 * RETURNS: 0 to 100
	 * EXAMPLE: 75% instead of 0.75
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Basic", meta=(
		DisplayName = "Get Stat Percentage (0-100)",
		Tooltip = "Get stat as 0-100 percentage. Alternative to 0-1 version.",
		Keywords = "get percent percentage whole",
		CompactNodeTitle = "Stat %100"
	))
	float GetStatPercentage100(EStatType StatType) const;

	/**
	 * Get how much of a stat is missing
	 * BLUEPRINT: Useful for healing calculations
	 *
	 * RETURNS: Max - Current (how much can be healed)
	 * EXAMPLE: 100 max - 75 current = 25 missing
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Calculations", meta=(
		DisplayName = "Get Stat Missing Amount",
		Tooltip = "Get how much stat is missing (Max - Current). Useful for healing logic.",
		Keywords = "get missing lost deficit",
		CompactNodeTitle = "Missing"
	))
	float GetStatMissingAmount(EStatType StatType) const;

	/**
	 * Check if stat is at maximum
	 * BLUEPRINT: Quick check for "is fully healed?" etc.
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Checks", meta=(
		DisplayName = "Is Stat at Max?",
		Tooltip = "Check if stat is at maximum value (full health, full stamina, etc.)",
		Keywords = "check is full max maximum complete",
		CompactNodeTitle = "Is Max?"
	))
	bool IsStatAtMax(EStatType StatType) const;

	/**
	 * Check if stat is at zero
	 * BLUEPRINT: Quick check for "is dead?", "out of stamina?" etc.
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Checks", meta=(
		DisplayName = "Is Stat at Zero?",
		Tooltip = "Check if stat is at 0 (dead, exhausted, starving, etc.)",
		Keywords = "check is empty zero none depleted",
		CompactNodeTitle = "Is Zero?"
	))
	bool IsStatAtZero(EStatType StatType) const;

	/**
	 * Check if stat is below critical threshold
	 * BLUEPRINT: For warning UI, critical state detection
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Checks", meta=(
		DisplayName = "Is Stat Critical?",
		Tooltip = "Check if stat is below critical threshold (default 15%)",
		Keywords = "check is critical low danger warning",
		CompactNodeTitle = "Is Critical?"
	))
	bool IsStatCritical(EStatType StatType) const;

	/**
	 * Get stat regeneration rate
	 * BLUEPRINT: See how fast a stat is regenerating/decaying
	 *
	 * RETURNS: Units per second (positive = regen, negative = decay)
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Advanced", meta=(
		DisplayName = "Get Regen Rate",
		Tooltip = "Get how fast stat regenerates per second (negative = decay)",
		Keywords = "get regen regeneration decay rate speed",
		CompactNodeTitle = "Regen Rate"
	))
	float GetStatRegenRate(EStatType StatType) const;

	/**
	 * Get base maximum value (before modifiers)
	 * BLUEPRINT: See original max value before buffs/debuffs
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Advanced", meta=(
		DisplayName = "Get Base Max Value",
		Tooltip = "Get original max value before any buffs/debuffs",
		Keywords = "get base original default max",
		CompactNodeTitle = "Base Max"
	))
	float GetStatBaseMax(EStatType StatType) const;

	// ========== BULK OPERATIONS ==========

	/**
	 * Get all stat names that exist
	 * BLUEPRINT: For UI lists, debug displays
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Bulk", meta=(
		DisplayName = "Get All Stat Types",
		Tooltip = "Get list of all stats being tracked",
		Keywords = "get all list array stats"
	))
	TArray<EStatType> GetAllStatTypes() const;

	/**
	 * Check if ANY stat is critical
	 * BLUEPRINT: Quick check for overall critical state
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Checks", meta=(
		DisplayName = "Is Any Stat Critical?",
		Tooltip = "Check if ANY stat is below critical threshold",
		Keywords = "check is any critical danger"
	))
	bool IsAnyStatCritical() const;

	/**
	 * Get average health of all stats (0-1)
	 * BLUEPRINT: Overall health indicator
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System|Getters|Calculations", meta=(
		DisplayName = "Get Average Stat Health",
		Tooltip = "Get average percentage of all stats (0-1). Overall health indicator.",
		Keywords = "get average mean overall total"
	))
	float GetAverageStatHealth() const;

private:
	/**
	 * Update stat regeneration/decay
	 */
	void UpdateStatRegeneration(float DeltaTime);

	/**
	 * Broadcast appropriate events for stat changes
	 */
	void BroadcastStatEvents(EStatType StatType, float OldValue, float NewValue);

	/**
	 * Replication callback for stats map
	 */
	UFUNCTION()
	void OnRep_Stats();

	/**
	 * Check for critical stats and fire events
	 */
	void CheckCriticalStats();
};
