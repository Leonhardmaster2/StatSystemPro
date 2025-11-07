// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "StatSystemProSettings.generated.h"

/**
 * ============================================================================
 * STAT SYSTEM PRO SETTINGS
 * ============================================================================
 *
 * Project-wide settings for StatSystemPro plugin
 *
 * ACCESS: Edit → Project Settings → Plugins → StatSystemPro
 *
 * Configure default behaviors, thresholds, and customizations here
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="StatSystemPro Settings"))
class STATSYSTEMPRO_API UStatSystemProSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UStatSystemProSettings();

	// ========== STAT LAYER SETTINGS ==========

	/**
	 * Default critical threshold for all stats
	 * CUSTOMIZATION: Change default critical percentage (0.0-1.0)
	 */
	UPROPERTY(config, EditAnywhere, Category = "Stat Layer", meta=(
		DisplayName = "Default Critical Threshold",
		Tooltip = "Default percentage threshold for critical stat warnings (0.15 = 15%)",
		ClampMin = "0.0",
		ClampMax = "1.0"
	))
	float DefaultCriticalThreshold;

	/**
	 * Enable auto-regeneration by default
	 * CUSTOMIZATION: Set default behavior for stat regeneration
	 */
	UPROPERTY(config, EditAnywhere, Category = "Stat Layer", meta=(
		DisplayName = "Enable Auto Regeneration by Default",
		Tooltip = "Should stats auto-regenerate by default?"
	))
	bool bDefaultEnableAutoRegeneration;

	/**
	 * Use simple mode by default
	 * CUSTOMIZATION: Set default initialization mode
	 */
	UPROPERTY(config, EditAnywhere, Category = "Stat Layer", meta=(
		DisplayName = "Use Simple Mode by Default",
		Tooltip = "Should components use simple mode (all stats = 100) by default?"
	))
	bool bDefaultUseSimpleMode;

	// ========== BODY LAYER SETTINGS ==========

	/**
	 * Default body part count
	 * CUSTOMIZATION: Change number of body parts
	 */
	UPROPERTY(config, EditAnywhere, Category = "Body Layer", meta=(
		DisplayName = "Default Body Part Count",
		Tooltip = "How many body parts to track (default 6: Head, Torso, Arms x2, Legs x2)",
		ClampMin = "1",
		ClampMax = "20"
	))
	int32 DefaultBodyPartCount;

	/**
	 * Enable body part damage by default
	 * CUSTOMIZATION: Set default behavior for locational damage
	 */
	UPROPERTY(config, EditAnywhere, Category = "Body Layer", meta=(
		DisplayName = "Enable Body Damage by Default",
		Tooltip = "Should body part damage be enabled by default?"
	))
	bool bDefaultEnableBodyDamage;

	/**
	 * Auto-fracture threshold (percentage)
	 * CUSTOMIZATION: When body part condition drops below this, it auto-fractures
	 */
	UPROPERTY(config, EditAnywhere, Category = "Body Layer", meta=(
		DisplayName = "Auto-Fracture Threshold",
		Tooltip = "Body parts auto-fracture when condition drops below this percentage",
		ClampMin = "0.0",
		ClampMax = "1.0"
	))
	float AutoFractureThreshold;

	// ========== STATUS EFFECT SETTINGS ==========

	/**
	 * Default max effect stacks
	 * CUSTOMIZATION: Default maximum stacks for stackable effects
	 */
	UPROPERTY(config, EditAnywhere, Category = "Status Effects", meta=(
		DisplayName = "Default Max Effect Stacks",
		Tooltip = "Default maximum stacks for stackable status effects",
		ClampMin = "1",
		ClampMax = "100"
	))
	int32 DefaultMaxEffectStacks;

	/**
	 * Default effect tick rate (seconds)
	 * CUSTOMIZATION: How often status effects tick
	 */
	UPROPERTY(config, EditAnywhere, Category = "Status Effects", meta=(
		DisplayName = "Default Effect Tick Rate",
		Tooltip = "How often (in seconds) status effects apply their effects",
		ClampMin = "0.1",
		ClampMax = "10.0"
	))
	float DefaultEffectTickRate;

	/**
	 * Enable status effect immunity system
	 * CUSTOMIZATION: Allow effects to grant immunity to other effects
	 */
	UPROPERTY(config, EditAnywhere, Category = "Status Effects", meta=(
		DisplayName = "Enable Immunity System",
		Tooltip = "Enable status effect immunity system (effects can grant immunity to others)"
	))
	bool bEnableImmunitySystem;

	// ========== ENVIRONMENT LAYER SETTINGS ==========

	/**
	 * Default body temperature (Celsius)
	 * CUSTOMIZATION: Normal body temperature
	 */
	UPROPERTY(config, EditAnywhere, Category = "Environment Layer", meta=(
		DisplayName = "Default Body Temperature",
		Tooltip = "Default/normal body temperature in Celsius (default 37°C)",
		ClampMin = "30.0",
		ClampMax = "45.0"
	))
	float DefaultBodyTemperature;

	/**
	 * Hypothermia threshold (Celsius)
	 * CUSTOMIZATION: Temperature below this triggers hypothermia
	 */
	UPROPERTY(config, EditAnywhere, Category = "Environment Layer", meta=(
		DisplayName = "Hypothermia Threshold",
		Tooltip = "Body temperature below this triggers hypothermia (default 35°C)",
		ClampMin = "25.0",
		ClampMax = "37.0"
	))
	float HypothermiaThreshold;

	/**
	 * Hyperthermia threshold (Celsius)
	 * CUSTOMIZATION: Temperature above this triggers heatstroke
	 */
	UPROPERTY(config, EditAnywhere, Category = "Environment Layer", meta=(
		DisplayName = "Hyperthermia Threshold",
		Tooltip = "Body temperature above this triggers heatstroke (default 40°C)",
		ClampMin = "38.0",
		ClampMax = "45.0"
	))
	float HyperthermiaThreshold;

	// ========== TIME SYSTEM SETTINGS ==========

	/**
	 * Default time speed (real seconds per game hour)
	 * CUSTOMIZATION: How fast time passes
	 */
	UPROPERTY(config, EditAnywhere, Category = "Time System", meta=(
		DisplayName = "Default Time Speed",
		Tooltip = "How many real seconds equal 1 game hour (default 60 = 1 minute real = 1 hour game)",
		ClampMin = "1.0",
		ClampMax = "3600.0"
	))
	float DefaultRealSecondsPerGameHour;

	/**
	 * Enable day/night cycle by default
	 * CUSTOMIZATION: Should time progress by default?
	 */
	UPROPERTY(config, EditAnywhere, Category = "Time System", meta=(
		DisplayName = "Enable Day/Night Cycle by Default",
		Tooltip = "Should day/night cycle be enabled by default?"
	))
	bool bDefaultEnableDayNightCycle;

	/**
	 * Enable seasons by default
	 * CUSTOMIZATION: Should season system be enabled?
	 */
	UPROPERTY(config, EditAnywhere, Category = "Time System", meta=(
		DisplayName = "Enable Seasons by Default",
		Tooltip = "Should season system be enabled by default?"
	))
	bool bDefaultEnableSeasons;

	/**
	 * Days per season
	 * CUSTOMIZATION: How many days in each season
	 */
	UPROPERTY(config, EditAnywhere, Category = "Time System", meta=(
		DisplayName = "Days Per Season",
		Tooltip = "How many game days in each season (default 30)",
		ClampMin = "1",
		ClampMax = "365"
	))
	int32 DefaultDaysPerSeason;

	// ========== PROGRESSION LAYER SETTINGS ==========

	/**
	 * XP curve exponent
	 * CUSTOMIZATION: How much XP increases per level
	 */
	UPROPERTY(config, EditAnywhere, Category = "Progression", meta=(
		DisplayName = "XP Curve Exponent",
		Tooltip = "XP formula exponent (1.5 = moderate increase, 2.0 = steep increase)",
		ClampMin = "1.0",
		ClampMax = "3.0"
	))
	float XPCurveExponent;

	/**
	 * Base XP for level 2
	 * CUSTOMIZATION: XP required for first level up
	 */
	UPROPERTY(config, EditAnywhere, Category = "Progression", meta=(
		DisplayName = "Base XP for Level 2",
		Tooltip = "XP required to reach level 2 (default 100)",
		ClampMin = "10",
		ClampMax = "10000"
	))
	int32 BaseXPForLevelTwo;

	/**
	 * Skill points per level
	 * CUSTOMIZATION: How many skill points gained per level
	 */
	UPROPERTY(config, EditAnywhere, Category = "Progression", meta=(
		DisplayName = "Skill Points Per Level",
		Tooltip = "Skill points gained per character level (default 1)",
		ClampMin = "0",
		ClampMax = "10"
	))
	int32 SkillPointsPerLevel;

	// ========== PERFORMANCE SETTINGS ==========

	/**
	 * Enable tick optimization
	 * CUSTOMIZATION: Use optimized tick intervals for better performance
	 */
	UPROPERTY(config, EditAnywhere, Category = "Performance", meta=(
		DisplayName = "Enable Tick Optimization",
		Tooltip = "Enable optimized tick intervals to improve performance (recommended: ON)"
	))
	bool bEnableTickOptimization;

	/**
	 * Stat tick interval (seconds)
	 * CUSTOMIZATION: How often stats update (lower = more accurate, higher = better performance)
	 */
	UPROPERTY(config, EditAnywhere, Category = "Performance", meta=(
		DisplayName = "Stat Update Interval",
		Tooltip = "How often (in seconds) stats update. Lower = smoother, Higher = better performance",
		ClampMin = "0.016",
		ClampMax = "1.0"
	))
	float StatUpdateInterval;

	/**
	 * Enable debug logging
	 * CUSTOMIZATION: Show debug messages in log
	 */
	UPROPERTY(config, EditAnywhere, Category = "Debug", meta=(
		DisplayName = "Enable Debug Logging",
		Tooltip = "Enable detailed debug logging for StatSystemPro"
	))
	bool bEnableDebugLogging;

	// Utility function to get settings
	static const UStatSystemProSettings* Get()
	{
		return GetDefault<UStatSystemProSettings>();
	}
};
