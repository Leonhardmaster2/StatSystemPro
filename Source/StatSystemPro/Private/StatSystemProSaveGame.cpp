// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatSystemProSaveGame.h"

UStatSystemProSaveGame::UStatSystemProSaveGame()
{
	// Layer toggles - all enabled by default
	bEnableStatLayer = true;
	bEnableBodyLayer = true;
	bEnableWeatherLayer = true;
	bEnableStatusEffectLayer = true;
	bEnableProgressionLayer = true;
	bEnableTimeLayer = true;

	// Weather Layer defaults
	CurrentWeather = EWeatherType::Clear;
	AmbientTemperature = 20.0f;
	WindSpeed = 5.0f;
	WetnessLevel = 0.0f;
	ShelterLevel = 0.0f;

	// Progression Layer defaults
	CurrentLevel = 1;
	CurrentXP = 0;
	StatPoints = 0;

	// Time Layer defaults
	CurrentGameTime = 0.0f;
	CurrentHour = 8;
	CurrentDay = 1;
}
