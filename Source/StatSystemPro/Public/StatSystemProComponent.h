// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "StatLayer/StatTypes.h"
#include "BodyLayer/BodyTypes.h"
#include "StatusEffectLayer/StatusEffectTypes.h"
#include "ProgressionLayer/ProgressionTypes.h"
#include "WeatherSystem/WeatherTypes.h"
#include "TimeSystem/TimeTypes.h"
#include "StatSystemProComponent.generated.h"

// Forward declarations
class UCurveFloat;
class UDataTable;

// ============================================================================
// UNIFIED COMPONENT DELEGATES
// ============================================================================

// Stat Layer Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatChanged, EStatType, StatType, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatMaxChanged, EStatType, StatType, float, NewMaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatReachedZero, EStatType, StatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatReachedMax, EStatType, StatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatCritical, EStatType, StatType, float, CurrentValue);

// Body Layer Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartDamaged, EBodyPart, BodyPart, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBodyPartFractured, EBodyPart, BodyPart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartBleeding, EBodyPart, BodyPart, float, BleedRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartBurned, EBodyPart, BodyPart, EBurnLevel, BurnLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartInfected, EBodyPart, BodyPart, float, InfectionRate);

// Weather Layer Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeatherChanged, EWeatherType, OldWeather, EWeatherType, NewWeather);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClothingEquipped, EClothingSlot, Slot, FClothingItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClothingRemoved, EClothingSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFreezingStageChanged, EFreezingStage, NewStage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverheatingStageChanged, EOverheatingStage, NewStage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTemperatureChanged, float, OldTemp, float, NewTemp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClothingWetnessChanged, float, NewWetness);

// Status Effect Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectApplied, FName, EffectID, int32, Stacks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectRemoved, FName, EffectID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectExpired, FName, EffectID, int32, RemainingStacks);

// Progression Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, int32, OldLevel, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnXPGained, int32, Amount, int32, NewTotal, EXPSource, Source);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUnlocked, FName, SkillID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatPointsAwarded, int32, Amount, int32, NewTotal);

// Time Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourChanged, int32, NewHour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayChanged, int32, NewDay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeOfDayChanged, ETimeOfDay, OldTimeOfDay, ETimeOfDay, NewTimeOfDay);

/**
 * ============================================================================
 * STATSYSTEMPRO UNIFIED COMPONENT
 * ============================================================================
 *
 * ONE COMPONENT TO RULE THEM ALL
 *
 * This is the NEW unified architecture for StatSystemPro.
 * Instead of 6 separate components, everything is now in ONE component.
 *
 * BENEFITS:
 * ✅ ONE component to add to your actor
 * ✅ ONE save/load call saves EVERYTHING
 * ✅ Better replication (one component = less overhead)
 * ✅ Easy layer toggles (checkboxes to enable/disable features)
 * ✅ Cleaner hierarchy
 * ✅ Better performance
 *
 * LAYERS (Toggle Each On/Off):
 * - Stat Layer: Health, stamina, hunger, thirst, etc. (100+ stats)
 * - Body Layer: Body parts, fractures, bleeding, burns, infections
 * - Weather Layer: Temperature, weather, clothing, freezing/overheating
 * - Status Effect Layer: Buffs, debuffs, stackable effects
 * - Progression Layer: XP, levels, skills, stat points
 * - Time Layer: Global time, day/night cycle
 *
 * USAGE:
 * 1. Add this component to your character
 * 2. Enable the layers you want (checkboxes in details panel)
 * 3. Done! Everything works automatically
 *
 * SAVE SYSTEM:
 * QuickSave("MySlot") - Saves EVERYTHING in one call
 * QuickLoad("MySlot") - Loads EVERYTHING in one call
 *
 * MULTIPLAYER:
 * - Full replication support
 * - Server authority on all changes
 * - Bandwidth optimized
 *
 * BACKWARD COMPATIBILITY:
 * - Old individual components still exist
 * - Can be used alongside unified component if needed
 * - Unified component is RECOMMENDED for new projects
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent, DisplayName="StatSystemPro (Unified - All-in-One)"))
class STATSYSTEMPRO_API UStatSystemProComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatSystemProComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ========================================================================
	// LAYER TOGGLES
	// ========================================================================

	/**
	 * Enable/Disable Stat Layer
	 * Manages: Health, Stamina, Hunger, Thirst, Energy, Blood, Temperature, etc.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StatSystemPro|Layer Control", meta=(
		DisplayName = "✓ Enable Stat Layer",
		Tooltip = "Turn ON to track stats like health, stamina, hunger, thirst, etc."
	))
	bool bEnableStatLayer;

	/**
	 * Enable/Disable Body Layer
	 * Manages: Body parts, fractures, bleeding, burns, infections, pain
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StatSystemPro|Layer Control", meta=(
		DisplayName = "✓ Enable Body Layer",
		Tooltip = "Turn ON to track body parts, injuries, fractures, bleeding, etc."
	))
	bool bEnableBodyLayer;

	/**
	 * Enable/Disable Weather Layer
	 * Manages: Weather types, temperature, clothing, freezing, overheating
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StatSystemPro|Layer Control", meta=(
		DisplayName = "✓ Enable Weather Layer",
		Tooltip = "Turn ON to track weather, temperature, clothing, freezing/overheating."
	))
	bool bEnableWeatherLayer;

	/**
	 * Enable/Disable Status Effect Layer
	 * Manages: Buffs, debuffs, stackable effects, timed effects
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StatSystemPro|Layer Control", meta=(
		DisplayName = "✓ Enable Status Effect Layer",
		Tooltip = "Turn ON to track buffs, debuffs, and status effects."
	))
	bool bEnableStatusEffectLayer;

	/**
	 * Enable/Disable Progression Layer
	 * Manages: XP, levels, skills, stat points, talents
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StatSystemPro|Layer Control", meta=(
		DisplayName = "✓ Enable Progression Layer",
		Tooltip = "Turn ON to track XP, levels, skills, and progression."
	))
	bool bEnableProgressionLayer;

	/**
	 * Enable/Disable Time Layer
	 * Manages: Global time, day/night cycle, time of day
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StatSystemPro|Layer Control", meta=(
		DisplayName = "✓ Enable Time Layer",
		Tooltip = "Turn ON to track global time and day/night cycle."
	))
	bool bEnableTimeLayer;

	// ========================================================================
	// STAT LAYER DATA
	// ========================================================================

	/** All stat values (Health, Stamina, Hunger, etc.) */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stats, Category = "StatSystemPro|Stat Layer")
	TMap<EStatType, FStatValue> Stats;

	/** Critical threshold for stats (0.0-1.0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Stat Layer", meta=(
		ClampMin = "0.0", ClampMax = "1.0"
	))
	float CriticalThreshold;

	/** Use simple mode (all stats = 100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Stat Layer")
	bool bUseSimpleMode;

	/** Enable auto regeneration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Stat Layer")
	bool bEnableAutoRegeneration;

	/** Stat configuration data table */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Stat Layer")
	UDataTable* StatConfigTable;

	// ========================================================================
	// BODY LAYER DATA
	// ========================================================================

	/** All body parts and their states */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BodyParts, Category = "StatSystemPro|Body Layer")
	TMap<EBodyPart, FBodyPartState> BodyParts;

	/** Body part configuration data table */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Body Layer")
	UDataTable* BodyPartConfigTable;

	// ========================================================================
	// WEATHER LAYER DATA
	// ========================================================================

	/** Current weather type */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentWeather, Category = "StatSystemPro|Weather Layer")
	EWeatherType CurrentWeather;

	/** Ambient temperature (Celsius) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Weather Layer")
	float AmbientTemperature;

	/** Wind speed (m/s) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Weather Layer")
	float WindSpeed;

	/** Wetness level (0-100%) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Weather Layer")
	float WetnessLevel;

	/** Shelter level (0-100%) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Weather Layer")
	float ShelterLevel;

	/** Currently equipped clothing */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_EquippedClothing, Category = "StatSystemPro|Weather Layer")
	TMap<EClothingSlot, FClothingItem> EquippedClothing;

	/** Current freezing stage */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Weather Layer")
	EFreezingStage CurrentFreezingStage;

	/** Current overheating stage */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Weather Layer")
	EOverheatingStage CurrentOverheatingStage;

	// ========================================================================
	// STATUS EFFECT LAYER DATA
	// ========================================================================

	/** All active status effects */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ActiveEffects, Category = "StatSystemPro|Status Effect Layer")
	TArray<FActiveStatusEffect> ActiveEffects;

	/** Status effect definitions data table */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Status Effect Layer")
	UDataTable* StatusEffectTable;

	// ========================================================================
	// PROGRESSION LAYER DATA
	// ========================================================================

	/** Current level */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Progression Layer")
	int32 CurrentLevel;

	/** Current XP */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Progression Layer")
	int32 CurrentXP;

	/** Available stat points */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Progression Layer")
	int32 StatPoints;

	/** Unlocked skills */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Progression Layer")
	TArray<FName> UnlockedSkills;

	/** Skill tree data table */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Progression Layer")
	UDataTable* SkillTreeTable;

	/** XP curve for leveling */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatSystemPro|Progression Layer")
	UCurveFloat* XPCurve;

	// ========================================================================
	// TIME LAYER DATA
	// ========================================================================

	/** Current game time (in seconds since start) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Time Layer")
	float CurrentGameTime;

	/** Time multiplier (1.0 = real-time, 2.0 = 2x speed) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "StatSystemPro|Time Layer", meta=(
		ClampMin = "0.0", ClampMax = "100.0"
	))
	float TimeMultiplier;

	/** Current hour (0-23) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Time Layer")
	int32 CurrentHour;

	/** Current day */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Time Layer")
	int32 CurrentDay;

	/** Current time of day */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "StatSystemPro|Time Layer")
	ETimeOfDay CurrentTimeOfDay;

	// ========================================================================
	// STAT LAYER EVENTS
	// ========================================================================

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Stat Layer")
	FOnStatChanged OnStatChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Stat Layer")
	FOnStatMaxChanged OnStatMaxChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Stat Layer")
	FOnStatReachedZero OnStatReachedZero;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Stat Layer")
	FOnStatReachedMax OnStatReachedMax;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Stat Layer")
	FOnStatCritical OnStatCritical;

	// ========================================================================
	// BODY LAYER EVENTS
	// ========================================================================

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Body Layer")
	FOnBodyPartDamaged OnBodyPartDamaged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Body Layer")
	FOnBodyPartFractured OnBodyPartFractured;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Body Layer")
	FOnBodyPartBleeding OnBodyPartBleeding;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Body Layer")
	FOnBodyPartBurned OnBodyPartBurned;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Body Layer")
	FOnBodyPartInfected OnBodyPartInfected;

	// ========================================================================
	// WEATHER LAYER EVENTS
	// ========================================================================

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Weather Layer")
	FOnWeatherChanged OnWeatherChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Weather Layer")
	FOnClothingEquipped OnClothingEquipped;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Weather Layer")
	FOnClothingRemoved OnClothingRemoved;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Weather Layer")
	FOnFreezingStageChanged OnFreezingStageChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Weather Layer")
	FOnOverheatingStageChanged OnOverheatingStageChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Weather Layer")
	FOnTemperatureChanged OnTemperatureChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Weather Layer")
	FOnClothingWetnessChanged OnClothingWetnessChanged;

	// ========================================================================
	// STATUS EFFECT LAYER EVENTS
	// ========================================================================

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Status Effect Layer")
	FOnStatusEffectApplied OnStatusEffectApplied;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Status Effect Layer")
	FOnStatusEffectRemoved OnStatusEffectRemoved;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Status Effect Layer")
	FOnStatusEffectExpired OnStatusEffectExpired;

	// ========================================================================
	// PROGRESSION LAYER EVENTS
	// ========================================================================

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Progression Layer")
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Progression Layer")
	FOnXPGained OnXPGained;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Progression Layer")
	FOnSkillUnlocked OnSkillUnlocked;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Progression Layer")
	FOnStatPointsAwarded OnStatPointsAwarded;

	// ========================================================================
	// TIME LAYER EVENTS
	// ========================================================================

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Time Layer")
	FOnHourChanged OnHourChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Time Layer")
	FOnDayChanged OnDayChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatSystemPro|Events|Time Layer")
	FOnTimeOfDayChanged OnTimeOfDayChanged;

	// ========================================================================
	// STAT LAYER FUNCTIONS
	// ========================================================================

	/** Initialize all stats */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer", meta=(
		DisplayName = "Initialize Stats"
	))
	void InitializeStats();

	/** Apply a change to a stat */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer", meta=(
		DisplayName = "Change Stat"
	))
	void ApplyStatChange(EStatType StatType, float Amount, FName Source, FGameplayTag ReasonTag);

	/** Set stat to exact value */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer")
	void SetStatValue(EStatType StatType, float NewValue);

	/** Set stat maximum */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer")
	void SetStatMaxValue(EStatType StatType, float NewMaxValue);

	/** Set regeneration rate */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer")
	void SetStatRegenerationRate(EStatType StatType, float Rate);

	/** Get stat value */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	float GetStatValue(EStatType StatType) const;

	/** Get stat max value */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	float GetStatMaxValue(EStatType StatType) const;

	/** Get stat percentage (0-1) */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	float GetStatPercentage(EStatType StatType) const;

	/** Get complete stat data */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	FStatValue GetStat(EStatType StatType) const;

	/** Check if stat exists */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	bool HasStat(EStatType StatType) const;

	/** Check if stat is at max */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	bool IsStatAtMax(EStatType StatType) const;

	/** Check if stat is at zero */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	bool IsStatAtZero(EStatType StatType) const;

	/** Check if stat is below critical threshold */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	bool IsStatCritical(EStatType StatType) const;

	/** Get lowest stat */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	EStatType GetLowestStat() const;

	/** Get highest stat */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	EStatType GetHighestStat() const;

	/** Check if ANY stat is critical */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	bool IsAnyCritical() const;

	/** Get count of stats below threshold */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Stat Layer")
	int32 GetStatsBelowThresholdCount(float Threshold) const;

	/** Transfer stat value from one stat to another */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer")
	void TransferStatValue(EStatType FromStat, EStatType ToStat, float Amount);

	/** Restore all stats to max */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer")
	void RestoreAllStats();

	/** Deplete all stats to zero */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Stat Layer")
	void DepleteAllStats();

	// ========================================================================
	// BODY LAYER FUNCTIONS
	// ========================================================================

	/** Initialize body parts */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void InitializeBodyParts();

	/** Damage a body part */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void DamageBodyPart(EBodyPart BodyPart, float Damage);

	/** Fracture a limb */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void FractureLimb(EBodyPart BodyPart);

	/** Set bleeding rate */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void SetBleedingRate(EBodyPart BodyPart, float Rate);

	/** Set burn level */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void SetBurnLevel(EBodyPart BodyPart, EBurnLevel BurnLevel);

	/** Apply infection */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void ApplyInfection(EBodyPart BodyPart, float InfectionAmount);

	/** Heal a limb */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void HealLimb(EBodyPart BodyPart, float HealAmount);

	/** Get body part state */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Body Layer")
	FBodyPartState GetBodyPartState(EBodyPart BodyPart) const;

	/** Get total body condition */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Body Layer")
	float GetTotalBodyCondition() const;

	/** Get total bleeding rate */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Body Layer")
	float GetTotalBleedingRate() const;

	/** Get total pain level */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Body Layer")
	float GetTotalPainLevel() const;

	/** Check for critical injury */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Body Layer")
	bool HasCriticalInjury() const;

	/** Calculate effect multipliers */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Body Layer")
	FBodyPartEffectMultipliers CalculateEffectMultipliers() const;

	/** Heal all body parts */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void HealAllBodyParts();

	/** Stop all bleeding */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Body Layer")
	void StopAllBleeding();

	// ========================================================================
	// WEATHER LAYER FUNCTIONS
	// ========================================================================

	/** Set weather type */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void SetWeather(EWeatherType NewWeather);

	/** Set ambient temperature */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void SetAmbientTemperature(float Temperature);

	/** Set wind speed */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void SetWindSpeed(float Speed);

	/** Set wetness level */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void SetWetnessLevel(float Wetness);

	/** Set shelter level */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void SetShelterLevel(float Shelter);

	/** Equip clothing */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void EquipClothing(EClothingSlot Slot, const FClothingItem& Item);

	/** Remove clothing */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void RemoveClothing(EClothingSlot Slot);

	/** Get equipped clothing */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Weather Layer")
	FClothingItem GetEquippedClothing(EClothingSlot Slot) const;

	/** Calculate effective temperature */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Weather Layer")
	FTemperatureResult CalculateEffectiveTemperature() const;

	/** Apply weather preset */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Weather Layer")
	void ApplyWeatherPreset(const FWeatherPreset& Preset);

	/** Get current weather */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Weather Layer")
	EWeatherType GetCurrentWeather() const { return CurrentWeather; }

	/** Get freezing stage */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Weather Layer")
	EFreezingStage GetFreezingStage() const { return CurrentFreezingStage; }

	/** Get overheating stage */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Weather Layer")
	EOverheatingStage GetOverheatingStage() const { return CurrentOverheatingStage; }

	// ========================================================================
	// STATUS EFFECT LAYER FUNCTIONS
	// ========================================================================

	/** Apply status effect */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Status Effect Layer")
	void ApplyStatusEffect(FName EffectID, int32 Stacks = 1);

	/** Remove status effect */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Status Effect Layer")
	void RemoveEffect(FName EffectID);

	/** Check if has status effect */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Status Effect Layer")
	bool HasStatusEffect(FName EffectID) const;

	/** Get status effect stacks */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Status Effect Layer")
	int32 GetStatusEffectStacks(FName EffectID) const;

	/** Get all active effects */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Status Effect Layer")
	TArray<FActiveStatusEffect> GetActiveEffects() const { return ActiveEffects; }

	/** Clear all status effects */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Status Effect Layer")
	void ClearAllStatusEffects();

	// ========================================================================
	// PROGRESSION LAYER FUNCTIONS
	// ========================================================================

	/** Award XP */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Progression Layer")
	void AwardXP(int32 Amount, EXPSource Source);

	/** Set level */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Progression Layer")
	void SetLevel(int32 NewLevel);

	/** Unlock skill */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Progression Layer")
	void UnlockSkill(FName SkillID);

	/** Check if skill is unlocked */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Progression Layer")
	bool IsSkillUnlocked(FName SkillID) const;

	/** Get current level */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Progression Layer")
	int32 GetCurrentLevel() const { return CurrentLevel; }

	/** Get current XP */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Progression Layer")
	int32 GetCurrentXP() const { return CurrentXP; }

	/** Get XP for next level */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Progression Layer")
	int32 GetXPForNextLevel() const;

	/** Get level progress (0-1) */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Progression Layer")
	float GetLevelProgress() const;

	/** Spend stat point */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Progression Layer")
	void SpendStatPoint(EStatType StatType, float Amount);

	/** Get available stat points */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Progression Layer")
	int32 GetStatPoints() const { return StatPoints; }

	// ========================================================================
	// TIME LAYER FUNCTIONS
	// ========================================================================

	/** Set time multiplier */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Time Layer")
	void SetTimeMultiplier(float Multiplier);

	/** Set current time */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Time Layer")
	void SetCurrentTime(int32 Hour, int32 Day);

	/** Advance time */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Time Layer")
	void AdvanceTime(float Hours);

	/** Get current hour */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Time Layer")
	int32 GetCurrentHour() const { return CurrentHour; }

	/** Get current day */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Time Layer")
	int32 GetCurrentDay() const { return CurrentDay; }

	/** Get time of day */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Time Layer")
	ETimeOfDay GetTimeOfDay() const { return CurrentTimeOfDay; }

	/** Get formatted time string */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Time Layer")
	FString GetFormattedTime() const;

	// ========================================================================
	// SAVE/LOAD SYSTEM
	// ========================================================================

	/**
	 * Quick Save - Saves EVERYTHING in one call
	 * Saves all enabled layers to the specified slot
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save System", meta=(
		DisplayName = "Quick Save (Everything)"
	))
	bool QuickSave(const FString& SlotName);

	/**
	 * Quick Load - Loads EVERYTHING in one call
	 * Loads all saved layers from the specified slot
	 */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Save System", meta=(
		DisplayName = "Quick Load (Everything)"
	))
	bool QuickLoad(const FString& SlotName);

	// ========================================================================
	// HELPER FUNCTIONS
	// ========================================================================

	/** Get debug info for all enabled layers */
	UFUNCTION(BlueprintCallable, Category = "StatSystemPro|Debug")
	FString GetDebugInfo() const;

	/** Check if character is in critical condition */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Query")
	bool IsCritical() const;

	/** Get overall health status (0-1) */
	UFUNCTION(BlueprintPure, Category = "StatSystemPro|Query")
	float GetOverallHealthStatus() const;

private:
	// ========================================================================
	// REPLICATION CALLBACKS
	// ========================================================================

	UFUNCTION()
	void OnRep_Stats();

	UFUNCTION()
	void OnRep_BodyParts();

	UFUNCTION()
	void OnRep_CurrentWeather();

	UFUNCTION()
	void OnRep_EquippedClothing();

	UFUNCTION()
	void OnRep_ActiveEffects();

	// ========================================================================
	// INTERNAL UPDATE FUNCTIONS
	// ========================================================================

	/** Update stat layer */
	void UpdateStatLayer(float DeltaTime);

	/** Update body layer */
	void UpdateBodyLayer(float DeltaTime);

	/** Update weather layer */
	void UpdateWeatherLayer(float DeltaTime);

	/** Update status effect layer */
	void UpdateStatusEffectLayer(float DeltaTime);

	/** Update progression layer */
	void UpdateProgressionLayer(float DeltaTime);

	/** Update time layer */
	void UpdateTimeLayer(float DeltaTime);

	/** Initialize all layers */
	void InitializeAllLayers();

	/** Update freezing/overheating stages */
	void UpdateTemperatureStages(float EffectiveTemp);

	/** Update bleeding effects */
	void UpdateBleeding(float DeltaTime);

	/** Update status effect durations */
	void UpdateStatusEffectDurations(float DeltaTime);

	/** Calculate wind chill */
	float CalculateWindChill() const;

	/** Get total clothing insulation */
	float GetTotalClothingInsulation(bool bForCold) const;
};
