// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeatherSystem/WeatherComponent.h"
#include "StatLayer/StatComponent.h"
#include "Net/UnrealNetwork.h"

UWeatherComponent::UWeatherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
	CurrentWeather = EWeatherType::Clear;
	AmbientTemperature = 20.0f;
	WindSpeed = 5.0f;
	PrecipitationIntensity = 0.0f;
	ShelterLevel = 0.0f;
	CurrentFreezingStage = EFreezingStage::None;
	CurrentOverheatingStage = EOverheatingStage::None;
	PreviousWeather = EWeatherType::Clear;
	PreviousFreezingStage = EFreezingStage::None;
	PreviousOverheatingStage = EOverheatingStage::None;

	SetIsReplicatedByDefault(true);
}

void UWeatherComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeatherComponent, bEnabled);
	DOREPLIFETIME(UWeatherComponent, CurrentWeather);
	DOREPLIFETIME(UWeatherComponent, AmbientTemperature);
	DOREPLIFETIME(UWeatherComponent, WindSpeed);
	DOREPLIFETIME(UWeatherComponent, PrecipitationIntensity);
	DOREPLIFETIME(UWeatherComponent, ShelterLevel);
	DOREPLIFETIME(UWeatherComponent, ClothingSlots);
	DOREPLIFETIME(UWeatherComponent, CurrentFreezingStage);
	DOREPLIFETIME(UWeatherComponent, CurrentOverheatingStage);
}

void UWeatherComponent::BeginPlay()
{
	Super::BeginPlay();
	PreviousWeather = CurrentWeather;
	PreviousFreezingStage = CurrentFreezingStage;
	PreviousOverheatingStage = CurrentOverheatingStage;
}

void UWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnabled)
	{
		return;
	}

	// Server updates everything
	if (GetOwnerRole() == ROLE_Authority)
	{
		UpdateClothingWetness(DeltaTime);
		UpdateTemperatureEffects(DeltaTime);
		UpdateTemperatureStages();
		ApplyTemperatureEffects(DeltaTime);
	}
}

// ========== WEATHER CONTROL ==========

void UWeatherComponent::SetWeatherType(EWeatherType NewWeather)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	EWeatherType OldWeather = CurrentWeather;
	CurrentWeather = NewWeather;

	// Auto-configure based on weather type
	switch (NewWeather)
	{
	case EWeatherType::Clear:
		PrecipitationIntensity = 0.0f;
		break;
	case EWeatherType::LightRain:
		PrecipitationIntensity = 20.0f;
		break;
	case EWeatherType::Rain:
		PrecipitationIntensity = 50.0f;
		break;
	case EWeatherType::HeavyRain:
	case EWeatherType::Thunderstorm:
		PrecipitationIntensity = 80.0f;
		break;
	case EWeatherType::LightSnow:
		PrecipitationIntensity = 15.0f;
		break;
	case EWeatherType::Snow:
		PrecipitationIntensity = 40.0f;
		break;
	case EWeatherType::HeavySnow:
		PrecipitationIntensity = 70.0f;
		break;
	default:
		break;
	}

	if (OldWeather != NewWeather)
	{
		OnWeatherChanged.Broadcast(OldWeather, NewWeather);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponent: Weather changed to %d"), (int32)NewWeather);
	}
}

void UWeatherComponent::ApplyWeatherPreset(const FWeatherPreset& Preset)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	SetWeatherType(Preset.WeatherType);

	// Apply temperature with variance
	float TempVariance = FMath::FRandRange(-Preset.TemperatureVariance, Preset.TemperatureVariance);
	AmbientTemperature = Preset.BaseTemperature + TempVariance;

	// Apply wind with variance
	float WindVariance = FMath::FRandRange(-Preset.WindVariance, Preset.WindVariance);
	WindSpeed = FMath::Max(0.0f, Preset.WindSpeed + WindVariance);

	PrecipitationIntensity = FMath::Max(Preset.RainIntensity, Preset.SnowIntensity);

	UE_LOG(LogTemp, Log, TEXT("WeatherComponent: Applied preset '%s' - Temp: %.1f°C, Wind: %.1f m/s"),
		*Preset.PresetName.ToString(), AmbientTemperature, WindSpeed);
}

void UWeatherComponent::SetTemperature(float Temperature)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		AmbientTemperature = Temperature;
	}
}

void UWeatherComponent::SetWindSpeed(float Speed)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		WindSpeed = FMath::Max(0.0f, Speed);
	}
}

// ========== CLOTHING SYSTEM ==========

void UWeatherComponent::EquipClothing(EClothingSlot Slot, const FClothingItem& Item)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	ClothingSlots.Add(Slot, Item);
	OnClothingEquipped.Broadcast(Slot, Item);

	UE_LOG(LogTemp, Log, TEXT("WeatherComponent: Equipped %s in slot %d"),
		*Item.DisplayName.ToString(), (int32)Slot);
}

void UWeatherComponent::RemoveClothing(EClothingSlot Slot)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (ClothingSlots.Remove(Slot) > 0)
	{
		OnClothingRemoved.Broadcast(Slot);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponent: Removed clothing from slot %d"), (int32)Slot);
	}
}

FClothingItem UWeatherComponent::GetClothingInSlot(EClothingSlot Slot, bool& bHasClothing) const
{
	if (ClothingSlots.Contains(Slot))
	{
		bHasClothing = true;
		return ClothingSlots[Slot];
	}

	bHasClothing = false;
	return FClothingItem();
}

void UWeatherComponent::AddClothingWetness(float Amount)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	for (auto& ClothingPair : ClothingSlots)
	{
		FClothingItem& Item = ClothingPair.Value;

		// Water resistance reduces wetness gain
		float WetnessPenalty = Amount * (1.0f - Item.WaterResistance / 100.0f);
		Item.CurrentWetness = FMath::Clamp(Item.CurrentWetness + WetnessPenalty, 0.0f, 100.0f);
	}
}

void UWeatherComponent::DryClothing(float Amount)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	for (auto& ClothingPair : ClothingSlots)
	{
		FClothingItem& Item = ClothingPair.Value;
		Item.CurrentWetness = FMath::Max(0.0f, Item.CurrentWetness - Amount);
	}
}

float UWeatherComponent::GetTotalColdInsulation() const
{
	float TotalInsulation = 0.0f;

	for (const auto& ClothingPair : ClothingSlots)
	{
		TotalInsulation += ClothingPair.Value.GetEffectiveColdInsulation();
	}

	return TotalInsulation;
}

float UWeatherComponent::GetTotalHeatProtection() const
{
	float TotalProtection = 0.0f;

	for (const auto& ClothingPair : ClothingSlots)
	{
		TotalProtection += ClothingPair.Value.GetEffectiveHeatInsulation();
	}

	return TotalProtection;
}

float UWeatherComponent::GetTotalClothingWeight() const
{
	float TotalWeight = 0.0f;

	for (const auto& ClothingPair : ClothingSlots)
	{
		TotalWeight += ClothingPair.Value.Weight;
	}

	return TotalWeight;
}

// ========== TEMPERATURE CALCULATIONS ==========

FTemperatureResult UWeatherComponent::CalculateEffectiveTemperature() const
{
	FTemperatureResult Result;
	Result.AmbientTemperature = AmbientTemperature;

	// Calculate wind chill
	Result.WindChillAdjustment = CalculateWindChill();

	// Get clothing protection
	float ColdInsulation = GetTotalColdInsulation();
	float HeatProtection = GetTotalHeatProtection();

	// Calculate wetness penalty (average of all clothing)
	float TotalWetness = 0.0f;
	int32 ClothingCount = ClothingSlots.Num();
	if (ClothingCount > 0)
	{
		for (const auto& ClothingPair : ClothingSlots)
		{
			TotalWetness += ClothingPair.Value.CurrentWetness;
		}
		TotalWetness /= ClothingCount;
	}
	Result.WetnessPenalty = -(TotalWetness / 100.0f) * 10.0f; // Up to -10°C when soaked

	// Calculate shelter bonus
	Result.ShelterBonus = (ShelterLevel / 100.0f) * 5.0f; // Up to +5°C when fully sheltered

	// Temperature adjustment based on clothing
	if (AmbientTemperature < 20.0f) // Cold conditions
	{
		Result.ClothingProtection = (ColdInsulation / 100.0f) * 15.0f; // Up to +15°C from clothing
		Result.TotalInsulation = ColdInsulation;
	}
	else // Hot conditions
	{
		Result.ClothingProtection = -(100.0f - HeatProtection) / 100.0f * 10.0f; // Heavy clothing makes you hotter
		Result.TotalInsulation = HeatProtection;
	}

	// Calculate effective temperature
	Result.EffectiveTemperature = AmbientTemperature
		+ Result.WindChillAdjustment
		+ Result.WetnessPenalty
		+ Result.ClothingProtection
		+ Result.ShelterBonus;

	// Calculate risks
	if (Result.EffectiveTemperature < 10.0f)
	{
		Result.FreezingRisk = FMath::Clamp((10.0f - Result.EffectiveTemperature) / 30.0f, 0.0f, 1.0f);
	}

	if (Result.EffectiveTemperature > 30.0f)
	{
		Result.OverheatingRisk = FMath::Clamp((Result.EffectiveTemperature - 30.0f) / 20.0f, 0.0f, 1.0f);
	}

	return Result;
}

float UWeatherComponent::CalculateWindChill() const
{
	if (WindSpeed < 5.0f || AmbientTemperature > 10.0f)
	{
		return 0.0f; // No significant wind chill
	}

	// Wind chill formula (simplified)
	// Stronger wind = feels colder
	float WindChillFactor = FMath::Pow(WindSpeed / 5.0f, 0.16f);
	float WindChill = -(13.12f + 0.6215f * AmbientTemperature - 13.96f * WindChillFactor + 0.4867f * AmbientTemperature * WindChillFactor - AmbientTemperature);

	// Wind resistance from clothing reduces wind chill
	float TotalWindResistance = 0.0f;
	for (const auto& ClothingPair : ClothingSlots)
	{
		TotalWindResistance += ClothingPair.Value.WindResistance;
	}
	float WindChillReduction = (TotalWindResistance / 100.0f); // 0-1

	return WindChill * (1.0f - WindChillReduction);
}

bool UWeatherComponent::IsFreezing() const
{
	return CurrentFreezingStage != EFreezingStage::None;
}

bool UWeatherComponent::IsOverheating() const
{
	return CurrentOverheatingStage != EOverheatingStage::None;
}

// ========== PRIVATE FUNCTIONS ==========

void UWeatherComponent::UpdateClothingWetness(float DeltaTime)
{
	if (PrecipitationIntensity > 0.0f && ShelterLevel < 50.0f)
	{
		// Get wet from rain/snow if not sheltered
		float ExposureFactor = 1.0f - (ShelterLevel / 100.0f);
		float WetnessGain = (PrecipitationIntensity / 100.0f) * 10.0f * DeltaTime * ExposureFactor;
		AddClothingWetness(WetnessGain);
	}
	else if (ShelterLevel > 80.0f || AmbientTemperature > 25.0f)
	{
		// Dry out when sheltered or in warm conditions
		float DryRate = 2.0f * DeltaTime;
		DryClothing(DryRate);
	}
}

void UWeatherComponent::UpdateTemperatureEffects(float DeltaTime)
{
	// Get effective temperature
	FTemperatureResult TempResult = CalculateEffectiveTemperature();

	// Find stat component to update body temperature
	UStatComponent* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	if (!StatComp)
	{
		return;
	}

	// Get current body temperature
	float CurrentBodyTemp = StatComp->GetStatValue(EStatType::BodyTemperature);
	float TargetBodyTemp = 37.0f; // Normal body temperature

	// Calculate how fast body temperature changes
	float TempDifference = TempResult.EffectiveTemperature - CurrentBodyTemp;
	float ChangeRate = TempDifference * 0.05f * DeltaTime; // Gradual change

	// Apply temperature change
	StatComp->ApplyStatChange(EStatType::BodyTemperature, ChangeRate, TEXT("Weather"), FGameplayTag());
}

void UWeatherComponent::UpdateTemperatureStages()
{
	// Get body temperature from stat component
	UStatComponent* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	if (!StatComp)
	{
		return;
	}

	float BodyTemp = StatComp->GetStatValue(EStatType::BodyTemperature);

	// Update freezing stage
	EFreezingStage NewFreezingStage = EFreezingStage::None;
	if (BodyTemp < 36.0f)
	{
		if (BodyTemp >= 35.0f)
			NewFreezingStage = EFreezingStage::Chilled;
		else if (BodyTemp >= 34.0f)
			NewFreezingStage = EFreezingStage::Cold;
		else if (BodyTemp >= 32.0f)
			NewFreezingStage = EFreezingStage::Freezing;
		else if (BodyTemp >= 30.0f)
			NewFreezingStage = EFreezingStage::Hypothermia;
		else
			NewFreezingStage = EFreezingStage::CriticalHypothermia;
	}

	if (NewFreezingStage != CurrentFreezingStage)
	{
		EFreezingStage OldStage = CurrentFreezingStage;
		CurrentFreezingStage = NewFreezingStage;
		OnFreezingStageChanged.Broadcast(OldStage, NewFreezingStage);

		if (NewFreezingStage != EFreezingStage::None && OldStage == EFreezingStage::None)
		{
			OnStartedFreezing.Broadcast(BodyTemp);
		}
	}

	// Update overheating stage
	EOverheatingStage NewOverheatingStage = EOverheatingStage::None;
	if (BodyTemp > 37.5f)
	{
		if (BodyTemp <= 38.5f)
			NewOverheatingStage = EOverheatingStage::Warm;
		else if (BodyTemp <= 39.5f)
			NewOverheatingStage = EOverheatingStage::Hot;
		else if (BodyTemp <= 40.5f)
			NewOverheatingStage = EOverheatingStage::Overheating;
		else if (BodyTemp <= 42.0f)
			NewOverheatingStage = EOverheatingStage::Heatstroke;
		else
			NewOverheatingStage = EOverheatingStage::CriticalHeatstroke;
	}

	if (NewOverheatingStage != CurrentOverheatingStage)
	{
		EOverheatingStage OldStage = CurrentOverheatingStage;
		CurrentOverheatingStage = NewOverheatingStage;
		OnOverheatingStageChanged.Broadcast(OldStage, NewOverheatingStage);

		if (NewOverheatingStage != EOverheatingStage::None && OldStage == EOverheatingStage::None)
		{
			OnStartedOverheating.Broadcast(BodyTemp);
		}
	}
}

void UWeatherComponent::ApplyTemperatureEffects(float DeltaTime)
{
	UStatComponent* StatComp = GetOwner()->FindComponentByClass<UStatComponent>();
	if (!StatComp)
	{
		return;
	}

	// Apply freezing damage
	switch (CurrentFreezingStage)
	{
	case EFreezingStage::Chilled:
		// Minor stamina drain
		StatComp->ApplyStatChange(EStatType::Stamina, -2.0f * DeltaTime, TEXT("Freezing"), FGameplayTag());
		break;
	case EFreezingStage::Cold:
		// Moderate stamina drain + minor health drain
		StatComp->ApplyStatChange(EStatType::Stamina, -5.0f * DeltaTime, TEXT("Freezing"), FGameplayTag());
		StatComp->ApplyStatChange(EStatType::Health_Core, -0.5f * DeltaTime, TEXT("Freezing"), FGameplayTag());
		break;
	case EFreezingStage::Freezing:
		// Heavy stamina drain + moderate health drain
		StatComp->ApplyStatChange(EStatType::Stamina, -10.0f * DeltaTime, TEXT("Freezing"), FGameplayTag());
		StatComp->ApplyStatChange(EStatType::Health_Core, -2.0f * DeltaTime, TEXT("Freezing"), FGameplayTag());
		break;
	case EFreezingStage::Hypothermia:
		// Severe health drain
		StatComp->ApplyStatChange(EStatType::Health_Core, -5.0f * DeltaTime, TEXT("Hypothermia"), FGameplayTag());
		break;
	case EFreezingStage::CriticalHypothermia:
		// Critical health drain - near death
		StatComp->ApplyStatChange(EStatType::Health_Core, -10.0f * DeltaTime, TEXT("CriticalHypothermia"), FGameplayTag());
		break;
	default:
		break;
	}

	// Apply overheating damage
	switch (CurrentOverheatingStage)
	{
	case EOverheatingStage::Warm:
		// Minor stamina drain
		StatComp->ApplyStatChange(EStatType::Stamina, -2.0f * DeltaTime, TEXT("Overheating"), FGameplayTag());
		break;
	case EOverheatingStage::Hot:
		// Moderate stamina drain + thirst increase
		StatComp->ApplyStatChange(EStatType::Stamina, -5.0f * DeltaTime, TEXT("Overheating"), FGameplayTag());
		StatComp->ApplyStatChange(EStatType::Thirst, -3.0f * DeltaTime, TEXT("Overheating"), FGameplayTag());
		break;
	case EOverheatingStage::Overheating:
		// Heavy stamina drain + thirst + minor health drain
		StatComp->ApplyStatChange(EStatType::Stamina, -10.0f * DeltaTime, TEXT("Overheating"), FGameplayTag());
		StatComp->ApplyStatChange(EStatType::Thirst, -5.0f * DeltaTime, TEXT("Overheating"), FGameplayTag());
		StatComp->ApplyStatChange(EStatType::Health_Core, -1.0f * DeltaTime, TEXT("Overheating"), FGameplayTag());
		break;
	case EOverheatingStage::Heatstroke:
		// Severe health drain
		StatComp->ApplyStatChange(EStatType::Health_Core, -5.0f * DeltaTime, TEXT("Heatstroke"), FGameplayTag());
		break;
	case EOverheatingStage::CriticalHeatstroke:
		// Critical health drain - near death
		StatComp->ApplyStatChange(EStatType::Health_Core, -10.0f * DeltaTime, TEXT("CriticalHeatstroke"), FGameplayTag());
		break;
	default:
		break;
	}
}

// ========== WEATHER PRESETS ==========

FWeatherPreset UWeatherComponent::GetBlizzardPreset()
{
	FWeatherPreset Preset;
	Preset.PresetName = "Blizzard";
	Preset.WeatherType = EWeatherType::HeavySnow;
	Preset.BaseTemperature = -15.0f;
	Preset.TemperatureVariance = 5.0f;
	Preset.WindSpeed = 25.0f;
	Preset.WindVariance = 5.0f;
	Preset.SnowIntensity = 90.0f;
	Preset.FogDensity = 60.0f;
	Preset.VisibilityMultiplier = 0.3f;
	Preset.MinDuration = 300.0f;
	Preset.MaxDuration = 900.0f;
	return Preset;
}

FWeatherPreset UWeatherComponent::GetHeatwavePreset()
{
	FWeatherPreset Preset;
	Preset.PresetName = "Heatwave";
	Preset.WeatherType = EWeatherType::Heatwave;
	Preset.BaseTemperature = 42.0f;
	Preset.TemperatureVariance = 3.0f;
	Preset.WindSpeed = 2.0f;
	Preset.WindVariance = 1.0f;
	Preset.VisibilityMultiplier = 0.8f;
	Preset.MinDuration = 600.0f;
	Preset.MaxDuration = 1800.0f;
	return Preset;
}

FWeatherPreset UWeatherComponent::GetThunderstormPreset()
{
	FWeatherPreset Preset;
	Preset.PresetName = "Thunderstorm";
	Preset.WeatherType = EWeatherType::Thunderstorm;
	Preset.BaseTemperature = 18.0f;
	Preset.TemperatureVariance = 3.0f;
	Preset.WindSpeed = 20.0f;
	Preset.WindVariance = 5.0f;
	Preset.RainIntensity = 85.0f;
	Preset.FogDensity = 30.0f;
	Preset.VisibilityMultiplier = 0.5f;
	Preset.MinDuration = 180.0f;
	Preset.MaxDuration = 600.0f;
	return Preset;
}

FWeatherPreset UWeatherComponent::GetClearWeatherPreset()
{
	FWeatherPreset Preset;
	Preset.PresetName = "Clear";
	Preset.WeatherType = EWeatherType::Clear;
	Preset.BaseTemperature = 22.0f;
	Preset.TemperatureVariance = 5.0f;
	Preset.WindSpeed = 3.0f;
	Preset.WindVariance = 2.0f;
	Preset.VisibilityMultiplier = 1.0f;
	Preset.MinDuration = 600.0f;
	Preset.MaxDuration = 1800.0f;
	return Preset;
}

// ========== REPLICATION CALLBACKS ==========

void UWeatherComponent::OnRep_CurrentWeather()
{
	if (PreviousWeather != CurrentWeather)
	{
		OnWeatherChanged.Broadcast(PreviousWeather, CurrentWeather);
		PreviousWeather = CurrentWeather;
	}
}

void UWeatherComponent::OnRep_ClothingSlots()
{
	// Clothing changed, recalculate insulation
}

void UWeatherComponent::OnRep_FreezingStage()
{
	if (PreviousFreezingStage != CurrentFreezingStage)
	{
		OnFreezingStageChanged.Broadcast(PreviousFreezingStage, CurrentFreezingStage);
		PreviousFreezingStage = CurrentFreezingStage;
	}
}

void UWeatherComponent::OnRep_OverheatingStage()
{
	if (PreviousOverheatingStage != CurrentOverheatingStage)
	{
		OnOverheatingStageChanged.Broadcast(PreviousOverheatingStage, CurrentOverheatingStage);
		PreviousOverheatingStage = CurrentOverheatingStage;
	}
}
