// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentTypes.generated.h"

/**
 * Enum for shelter state
 */
UENUM(BlueprintType)
enum class EShelterState : uint8
{
	Outside UMETA(DisplayName = "Outside"),
	PartialShelter UMETA(DisplayName = "Partial Shelter"),
	FullShelter UMETA(DisplayName = "Full Shelter"),

	MAX UMETA(Hidden)
};

/**
 * Struct representing environmental conditions
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FEnvironmentState
{
	GENERATED_BODY()

	/** Ambient temperature in Celsius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float AmbientTemperature;

	/** Wind intensity (0-1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WindIntensity;

	/** Rain level (0-1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float RainLevel;

	/** Snow level (0-1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float SnowLevel;

	/** Radiation factor (0-1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float RadiationFactor;

	/** Current shelter state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	EShelterState ShelterState;

	/** Clothing insulation value (0-1, 0 = naked, 1 = fully insulated) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float ClothingInsulation;

	FEnvironmentState()
		: AmbientTemperature(20.0f)
		, WindIntensity(0.0f)
		, RainLevel(0.0f)
		, SnowLevel(0.0f)
		, RadiationFactor(0.0f)
		, ShelterState(EShelterState::Outside)
		, ClothingInsulation(0.5f)
	{
	}

	/**
	 * Calculate exposure factor based on shelter and clothing
	 * Returns value 0-1 where 1 = fully exposed
	 */
	float CalculateExposureFactor() const
	{
		float BaseShelterProtection = 0.0f;

		switch (ShelterState)
		{
		case EShelterState::FullShelter:
			BaseShelterProtection = 0.9f;
			break;
		case EShelterState::PartialShelter:
			BaseShelterProtection = 0.5f;
			break;
		case EShelterState::Outside:
		default:
			BaseShelterProtection = 0.0f;
			break;
		}

		// Exposure formula from Todo.md
		float Exposure = (1.0f - ClothingInsulation) * (1.0f - BaseShelterProtection);
		Exposure += WindIntensity * 0.1f;

		return FMath::Clamp(Exposure, 0.0f, 1.0f);
	}

	/**
	 * Check if it's currently raining
	 */
	bool IsRaining() const
	{
		return RainLevel > 0.1f;
	}

	/**
	 * Check if it's currently snowing
	 */
	bool IsSnowing() const
	{
		return SnowLevel > 0.1f;
	}

	/**
	 * Check if there's dangerous radiation
	 */
	bool HasRadiation() const
	{
		return RadiationFactor > 0.1f;
	}
};

/**
 * Configuration for environment effects
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FEnvironmentEffectConfig
{
	GENERATED_BODY()

	/** Rate at which body temperature moves toward ambient (per second) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Config")
	float TemperatureChangeRate;

	/** Rate at which wetness increases in rain (per second) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Config")
	float WetnessGainRateInRain;

	/** Rate at which wetness decreases when dry (per second) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Config")
	float WetnessDryRate;

	/** Wetness multiplier for temperature change */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Config")
	float WetnessTemperatureMultiplier;

	/** Wind multiplier for temperature change */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Config")
	float WindTemperatureMultiplier;

	/** Radiation damage per second at full radiation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Config")
	float RadiationDamageRate;

	FEnvironmentEffectConfig()
		: TemperatureChangeRate(0.1f)
		, WetnessGainRateInRain(5.0f)
		, WetnessDryRate(2.0f)
		, WetnessTemperatureMultiplier(0.02f)
		, WindTemperatureMultiplier(0.1f)
		, RadiationDamageRate(1.0f)
	{
	}
};
