// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnvironmentLayer/EnvironmentComponent.h"
#include "StatLayer/StatComponent.h"
#include "StatusEffectLayer/StatusEffectComponent.h"

UEnvironmentComponent::UEnvironmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
	StatComponent = nullptr;
	StatusEffectComponent = nullptr;
	bHypothermiaTriggered = false;
	bHeatstrokeTriggered = false;
}

void UEnvironmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// Try to find components on the same actor
	if (!StatComponent)
	{
		StatComponent = GetOwner()->FindComponentByClass<UStatComponent>();
	}

	if (!StatusEffectComponent)
	{
		StatusEffectComponent = GetOwner()->FindComponentByClass<UStatusEffectComponent>();
	}
}

void UEnvironmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnabled)
	{
		return;
	}

	UpdateBodyTemperature(DeltaTime);
	UpdateWetness(DeltaTime);
	UpdateRadiation(DeltaTime);
	CheckTemperatureEffects();
}

void UEnvironmentComponent::SetAmbientTemperature(float Temperature)
{
	CurrentEnvironment.AmbientTemperature = Temperature;
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

void UEnvironmentComponent::SetWindIntensity(float Intensity)
{
	CurrentEnvironment.WindIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

void UEnvironmentComponent::SetRainLevel(float Level)
{
	CurrentEnvironment.RainLevel = FMath::Clamp(Level, 0.0f, 1.0f);
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

void UEnvironmentComponent::SetSnowLevel(float Level)
{
	CurrentEnvironment.SnowLevel = FMath::Clamp(Level, 0.0f, 1.0f);
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

void UEnvironmentComponent::SetShelterState(EShelterState NewState)
{
	CurrentEnvironment.ShelterState = NewState;
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

void UEnvironmentComponent::SetClothingInsulation(float Insulation)
{
	CurrentEnvironment.ClothingInsulation = FMath::Clamp(Insulation, 0.0f, 1.0f);
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

void UEnvironmentComponent::SetRadiationFactor(float Factor)
{
	CurrentEnvironment.RadiationFactor = FMath::Clamp(Factor, 0.0f, 1.0f);
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

void UEnvironmentComponent::SetEnvironmentState(const FEnvironmentState& NewState)
{
	CurrentEnvironment = NewState;
	OnEnvironmentChanged.Broadcast(CurrentEnvironment);
}

float UEnvironmentComponent::GetExposureFactor() const
{
	return CurrentEnvironment.CalculateExposureFactor();
}

bool UEnvironmentComponent::IsHypothermic() const
{
	if (!StatComponent)
	{
		return false;
	}

	float BodyTemp = StatComponent->GetStatValue(EStatType::BodyTemperature);
	return BodyTemp < 35.0f; // Hypothermia threshold
}

bool UEnvironmentComponent::HasHeatstroke() const
{
	if (!StatComponent)
	{
		return false;
	}

	float BodyTemp = StatComponent->GetStatValue(EStatType::BodyTemperature);
	return BodyTemp > 40.0f; // Heatstroke threshold
}

void UEnvironmentComponent::UpdateBodyTemperature(float DeltaTime)
{
	if (!StatComponent)
	{
		return;
	}

	float CurrentBodyTemp = StatComponent->GetStatValue(EStatType::BodyTemperature);
	float TargetTemp = CurrentEnvironment.AmbientTemperature;

	// Get current wetness
	float Wetness = StatComponent->GetStatValue(EStatType::Wetness);

	// Calculate exposure factor using the formula from Todo.md:
	// Exposure = (1 - Clothing_Insulation) + Wetness*0.02 + WindIntensity*0.1
	float ExposureFactor = CurrentEnvironment.CalculateExposureFactor();
	ExposureFactor += (Wetness / 100.0f) * EffectConfig.WetnessTemperatureMultiplier;

	// Calculate temperature change rate
	float TempDifference = TargetTemp - CurrentBodyTemp;
	float TempChange = TempDifference * EffectConfig.TemperatureChangeRate * ExposureFactor * DeltaTime;

	// Apply temperature change
	if (!FMath::IsNearlyZero(TempChange, 0.01f))
	{
		StatComponent->ApplyStatChange(
			EStatType::BodyTemperature,
			TempChange,
			TEXT("Environment"),
			FGameplayTag()
		);
	}
}

void UEnvironmentComponent::UpdateWetness(float DeltaTime)
{
	if (!StatComponent)
	{
		return;
	}

	float CurrentWetness = StatComponent->GetStatValue(EStatType::Wetness);
	float WetnessChange = 0.0f;

	// Increase wetness if raining or snowing
	if (CurrentEnvironment.IsRaining() || CurrentEnvironment.IsSnowing())
	{
		// Only get wet if not in full shelter
		if (CurrentEnvironment.ShelterState != EShelterState::FullShelter)
		{
			float RainFactor = CurrentEnvironment.IsRaining() ? CurrentEnvironment.RainLevel : CurrentEnvironment.SnowLevel * 0.5f;
			float ShelterMultiplier = CurrentEnvironment.ShelterState == EShelterState::PartialShelter ? 0.3f : 1.0f;

			WetnessChange = EffectConfig.WetnessGainRateInRain * RainFactor * ShelterMultiplier * DeltaTime;
		}
	}
	else
	{
		// Dry off when not raining
		if (CurrentWetness > 0.0f)
		{
			// Drying rate affected by:
			// - Body temperature (warmer = faster drying)
			// - Wind (more wind = faster drying)
			// - Shelter (inside = slower drying from wind)

			float BodyTemp = StatComponent->GetStatValue(EStatType::BodyTemperature);
			float TempFactor = FMath::Clamp((BodyTemp - 20.0f) / 20.0f, 0.1f, 2.0f);

			float WindFactor = 1.0f + (CurrentEnvironment.WindIntensity * 0.5f);
			if (CurrentEnvironment.ShelterState == EShelterState::FullShelter)
			{
				WindFactor = 0.5f; // Slow drying indoors
			}

			WetnessChange = -EffectConfig.WetnessDryRate * TempFactor * WindFactor * DeltaTime;
		}
	}

	if (!FMath::IsNearlyZero(WetnessChange))
	{
		StatComponent->ApplyStatChange(
			EStatType::Wetness,
			WetnessChange,
			TEXT("Environment"),
			FGameplayTag()
		);
	}
}

void UEnvironmentComponent::UpdateRadiation(float DeltaTime)
{
	if (!StatComponent)
	{
		return;
	}

	if (CurrentEnvironment.HasRadiation())
	{
		// Apply radiation damage
		float RadiationDamage = EffectConfig.RadiationDamageRate * CurrentEnvironment.RadiationFactor * DeltaTime;

		StatComponent->ApplyStatChange(
			EStatType::Health_Core,
			-RadiationDamage,
			TEXT("Radiation"),
			FGameplayTag()
		);

		// Increase toxicity
		StatComponent->ApplyStatChange(
			EStatType::Toxicity,
			RadiationDamage * 0.5f,
			TEXT("Radiation"),
			FGameplayTag()
		);

		OnRadiationExposure.Broadcast(CurrentEnvironment.RadiationFactor);
	}
}

void UEnvironmentComponent::CheckTemperatureEffects()
{
	if (!StatComponent)
	{
		return;
	}

	float BodyTemp = StatComponent->GetStatValue(EStatType::BodyTemperature);

	// Check for hypothermia
	if (BodyTemp < 35.0f)
	{
		if (!bHypothermiaTriggered)
		{
			bHypothermiaTriggered = true;
			OnHypothermia.Broadcast();

			// Apply hypothermia status effect
			if (StatusEffectComponent)
			{
				StatusEffectComponent->ApplyStatusEffect(TEXT("Hypothermia"));
			}

			UE_LOG(LogTemp, Warning, TEXT("Hypothermia! Body temperature: %.2f"), BodyTemp);
		}
	}
	else if (bHypothermiaTriggered && BodyTemp > 36.0f)
	{
		// Recovered from hypothermia
		bHypothermiaTriggered = false;
		if (StatusEffectComponent)
		{
			StatusEffectComponent->RemoveEffect(TEXT("Hypothermia"));
		}
	}

	// Check for heatstroke
	if (BodyTemp > 40.0f)
	{
		if (!bHeatstrokeTriggered)
		{
			bHeatstrokeTriggered = true;
			OnHeatstroke.Broadcast();

			// Apply heatstroke status effect
			if (StatusEffectComponent)
			{
				StatusEffectComponent->ApplyStatusEffect(TEXT("Heatstroke"));
			}

			UE_LOG(LogTemp, Warning, TEXT("Heatstroke! Body temperature: %.2f"), BodyTemp);
		}
	}
	else if (bHeatstrokeTriggered && BodyTemp < 39.0f)
	{
		// Recovered from heatstroke
		bHeatstrokeTriggered = false;
		if (StatusEffectComponent)
		{
			StatusEffectComponent->RemoveEffect(TEXT("Heatstroke"));
		}
	}

	// Apply moderate cold/heat effects
	if (BodyTemp < 36.0f && BodyTemp >= 35.0f)
	{
		// Cold - reduce stamina regen
		if (StatusEffectComponent && !StatusEffectComponent->HasEffect(TEXT("Cold")))
		{
			StatusEffectComponent->ApplyStatusEffect(TEXT("Cold"));
		}
	}
	else if (StatusEffectComponent && StatusEffectComponent->HasEffect(TEXT("Cold")))
	{
		StatusEffectComponent->RemoveEffect(TEXT("Cold"));
	}
}
