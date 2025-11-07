// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatSystemProSettings.h"

UStatSystemProSettings::UStatSystemProSettings()
{
	// Stat Layer Defaults
	DefaultCriticalThreshold = 0.15f;  // 15%
	bDefaultEnableAutoRegeneration = true;
	bDefaultUseSimpleMode = true;

	// Body Layer Defaults
	DefaultBodyPartCount = 6;  // Head, Torso, 2 Arms, 2 Legs
	bDefaultEnableBodyDamage = true;
	AutoFractureThreshold = 0.20f;  // 20%

	// Status Effect Defaults
	DefaultMaxEffectStacks = 5;
	DefaultEffectTickRate = 1.0f;  // 1 second
	bEnableImmunitySystem = true;

	// Environment Layer Defaults
	DefaultBodyTemperature = 37.0f;  // 37°C (98.6°F)
	HypothermiaThreshold = 35.0f;  // 35°C
	HyperthermiaThreshold = 40.0f;  // 40°C

	// Time System Defaults
	DefaultRealSecondsPerGameHour = 60.0f;  // 1 real minute = 1 game hour
	bDefaultEnableDayNightCycle = true;
	bDefaultEnableSeasons = false;
	DefaultDaysPerSeason = 30;

	// Progression Defaults
	XPCurveExponent = 1.5f;
	BaseXPForLevelTwo = 100;
	SkillPointsPerLevel = 1;

	// Performance Defaults
	bEnableTickOptimization = true;
	StatUpdateInterval = 0.033f;  // ~30 FPS update rate

	// Debug Defaults
	bEnableDebugLogging = false;
}
