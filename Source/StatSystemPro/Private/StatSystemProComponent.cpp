// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatSystemProComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "StatSystemProSaveGame.h"

UStatSystemProComponent::UStatSystemProComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Layer toggles - all enabled by default
	bEnableStatLayer = true;
	bEnableBodyLayer = true;
	bEnableWeatherLayer = true;
	bEnableStatusEffectLayer = true;
	bEnableProgressionLayer = true;
	bEnableTimeLayer = true;

	// Stat Layer defaults
	CriticalThreshold = 0.15f;
	bUseSimpleMode = true;
	bEnableAutoRegeneration = true;
	StatConfigTable = nullptr;

	// Body Layer defaults
	BodyPartConfigTable = nullptr;

	// Weather Layer defaults
	CurrentWeather = EWeatherType::Clear;
	AmbientTemperature = 20.0f;
	WindSpeed = 5.0f;
	WetnessLevel = 0.0f;
	ShelterLevel = 0.0f;
	CurrentFreezingStage = EFreezingStage::None;
	CurrentOverheatingStage = EOverheatingStage::None;

	// Status Effect Layer defaults
	StatusEffectTable = nullptr;

	// Progression Layer defaults
	CurrentLevel = 1;
	CurrentXP = 0;
	StatPoints = 0;
	SkillTreeTable = nullptr;
	XPCurve = nullptr;

	// Time Layer defaults
	CurrentGameTime = 0.0f;
	TimeMultiplier = 1.0f;
	CurrentHour = 8; // Start at 8 AM
	CurrentDay = 1;
	CurrentTimeOfDay = ETimeOfDay::Morning;

	// Enable replication
	SetIsReplicatedByDefault(true);
}

void UStatSystemProComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Layer toggles
	DOREPLIFETIME(UStatSystemProComponent, bEnableStatLayer);
	DOREPLIFETIME(UStatSystemProComponent, bEnableBodyLayer);
	DOREPLIFETIME(UStatSystemProComponent, bEnableWeatherLayer);
	DOREPLIFETIME(UStatSystemProComponent, bEnableStatusEffectLayer);
	DOREPLIFETIME(UStatSystemProComponent, bEnableProgressionLayer);
	DOREPLIFETIME(UStatSystemProComponent, bEnableTimeLayer);

	// Stat Layer
	DOREPLIFETIME(UStatSystemProComponent, Stats);

	// Body Layer
	DOREPLIFETIME(UStatSystemProComponent, BodyParts);

	// Weather Layer
	DOREPLIFETIME(UStatSystemProComponent, CurrentWeather);
	DOREPLIFETIME(UStatSystemProComponent, AmbientTemperature);
	DOREPLIFETIME(UStatSystemProComponent, WindSpeed);
	DOREPLIFETIME(UStatSystemProComponent, WetnessLevel);
	DOREPLIFETIME(UStatSystemProComponent, ShelterLevel);
	DOREPLIFETIME(UStatSystemProComponent, EquippedClothing);
	DOREPLIFETIME(UStatSystemProComponent, CurrentFreezingStage);
	DOREPLIFETIME(UStatSystemProComponent, CurrentOverheatingStage);

	// Status Effect Layer
	DOREPLIFETIME(UStatSystemProComponent, ActiveEffects);

	// Progression Layer
	DOREPLIFETIME(UStatSystemProComponent, CurrentLevel);
	DOREPLIFETIME(UStatSystemProComponent, CurrentXP);
	DOREPLIFETIME(UStatSystemProComponent, StatPoints);
	DOREPLIFETIME(UStatSystemProComponent, UnlockedSkills);

	// Time Layer
	DOREPLIFETIME(UStatSystemProComponent, CurrentGameTime);
	DOREPLIFETIME(UStatSystemProComponent, TimeMultiplier);
	DOREPLIFETIME(UStatSystemProComponent, CurrentHour);
	DOREPLIFETIME(UStatSystemProComponent, CurrentDay);
	DOREPLIFETIME(UStatSystemProComponent, CurrentTimeOfDay);
}

void UStatSystemProComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only server initializes
	if (GetOwnerRole() == ROLE_Authority)
	{
		InitializeAllLayers();
	}
}

void UStatSystemProComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Only server updates
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	// Update each enabled layer
	if (bEnableStatLayer)
	{
		UpdateStatLayer(DeltaTime);
	}

	if (bEnableBodyLayer)
	{
		UpdateBodyLayer(DeltaTime);
	}

	if (bEnableWeatherLayer)
	{
		UpdateWeatherLayer(DeltaTime);
	}

	if (bEnableStatusEffectLayer)
	{
		UpdateStatusEffectLayer(DeltaTime);
	}

	if (bEnableProgressionLayer)
	{
		UpdateProgressionLayer(DeltaTime);
	}

	if (bEnableTimeLayer)
	{
		UpdateTimeLayer(DeltaTime);
	}
}

// ============================================================================
// INITIALIZATION
// ============================================================================

void UStatSystemProComponent::InitializeAllLayers()
{
	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: Initializing unified component..."));

	if (bEnableStatLayer)
	{
		InitializeStats();
	}

	if (bEnableBodyLayer)
	{
		InitializeBodyParts();
	}

	if (bEnableWeatherLayer)
	{
		// Weather layer initialized via defaults in constructor
		UE_LOG(LogTemp, Log, TEXT("  ✓ Weather Layer initialized"));
	}

	if (bEnableStatusEffectLayer)
	{
		// Status effects start empty
		ActiveEffects.Empty();
		UE_LOG(LogTemp, Log, TEXT("  ✓ Status Effect Layer initialized"));
	}

	if (bEnableProgressionLayer)
	{
		// Progression starts at level 1
		UE_LOG(LogTemp, Log, TEXT("  ✓ Progression Layer initialized (Level %d)"), CurrentLevel);
	}

	if (bEnableTimeLayer)
	{
		UE_LOG(LogTemp, Log, TEXT("  ✓ Time Layer initialized (Day %d, %02d:00)"), CurrentDay, CurrentHour);
	}

	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: All enabled layers initialized!"));
}

// ============================================================================
// STAT LAYER IMPLEMENTATION
// ============================================================================

void UStatSystemProComponent::InitializeStats()
{
	if (!bEnableStatLayer)
	{
		return;
	}

	Stats.Empty();

	if (bUseSimpleMode)
	{
		// Initialize all stats to 100
		for (int32 i = 0; i < (int32)EStatType::MAX; ++i)
		{
			EStatType StatType = (EStatType)i;
			FStatValue DefaultStat(100.0f);

			// Special defaults for certain stats
			switch (StatType)
			{
			case EStatType::BodyTemperature:
				DefaultStat.CurrentValue = 37.0f;
				DefaultStat.MaxValue = 42.0f;
				DefaultStat.BaseMaxValue = 42.0f;
				break;
			case EStatType::Wetness:
				DefaultStat.CurrentValue = 0.0f;
				DefaultStat.MaxValue = 100.0f;
				DefaultStat.BaseMaxValue = 100.0f;
				break;
			case EStatType::HeartRate:
				DefaultStat.CurrentValue = 70.0f;
				DefaultStat.MaxValue = 220.0f;
				DefaultStat.BaseMaxValue = 220.0f;
				break;
			case EStatType::BloodPressure:
				DefaultStat.CurrentValue = 120.0f;
				DefaultStat.MaxValue = 200.0f;
				DefaultStat.BaseMaxValue = 200.0f;
				break;
			default:
				break;
			}

			Stats.Add(StatType, DefaultStat);
		}

		UE_LOG(LogTemp, Log, TEXT("  ✓ Stat Layer initialized (SIMPLE MODE - all stats = 100)"));
	}
	else if (StatConfigTable)
	{
		TArray<FStatConfigRow*> Rows;
		StatConfigTable->GetAllRows<FStatConfigRow>(TEXT("InitializeStats"), Rows);

		for (FStatConfigRow* Row : Rows)
		{
			if (Row)
			{
				FStatValue NewStat;
				NewStat.BaseMaxValue = Row->DefaultMaxValue;
				NewStat.MaxValue = Row->DefaultMaxValue;
				NewStat.CurrentValue = Row->DefaultMaxValue;
				NewStat.RegenerationRate = Row->DefaultRegenerationRate;
				NewStat.RegenerationCurve = Row->RegenerationCurve;

				Stats.Add(Row->StatType, NewStat);
			}
		}

		UE_LOG(LogTemp, Log, TEXT("  ✓ Stat Layer initialized (ADVANCED MODE from data table)"));
	}
	else
	{
		// Fallback
		for (int32 i = 0; i < (int32)EStatType::MAX; ++i)
		{
			Stats.Add((EStatType)i, FStatValue(100.0f));
		}

		UE_LOG(LogTemp, Warning, TEXT("  ⚠ Stat Layer: No data table in Advanced Mode, using defaults"));
	}
}

void UStatSystemProComponent::ApplyStatChange(EStatType StatType, float Amount, FName Source, FGameplayTag ReasonTag)
{
	if (!bEnableStatLayer || !HasStat(StatType))
	{
		return;
	}

	FStatValue& Stat = Stats[StatType];
	float OldValue = Stat.CurrentValue;
	Stat.CurrentValue += Amount;
	Stat.Clamp();

	// Broadcast events
	if (!FMath::IsNearlyEqual(OldValue, Stat.CurrentValue))
	{
		OnStatChanged.Broadcast(StatType, OldValue, Stat.CurrentValue);

		if (Stat.IsAtZero() && !FMath::IsNearlyZero(OldValue))
		{
			OnStatReachedZero.Broadcast(StatType);
		}

		if (Stat.IsAtMax() && !FMath::IsNearlyEqual(OldValue, Stat.MaxValue))
		{
			OnStatReachedMax.Broadcast(StatType);
		}

		if (Stat.GetPercentage() < CriticalThreshold)
		{
			OnStatCritical.Broadcast(StatType, Stat.CurrentValue);
		}
	}
}

void UStatSystemProComponent::SetStatValue(EStatType StatType, float NewValue)
{
	if (!bEnableStatLayer || !HasStat(StatType))
	{
		return;
	}

	FStatValue& Stat = Stats[StatType];
	float OldValue = Stat.CurrentValue;
	Stat.CurrentValue = NewValue;
	Stat.Clamp();

	if (!FMath::IsNearlyEqual(OldValue, Stat.CurrentValue))
	{
		OnStatChanged.Broadcast(StatType, OldValue, Stat.CurrentValue);
	}
}

void UStatSystemProComponent::SetStatMaxValue(EStatType StatType, float NewMaxValue)
{
	if (!bEnableStatLayer || !HasStat(StatType))
	{
		return;
	}

	FStatValue& Stat = Stats[StatType];
	Stat.MaxValue = FMath::Max(0.0f, NewMaxValue);
	Stat.Clamp();

	OnStatMaxChanged.Broadcast(StatType, Stat.MaxValue);
}

void UStatSystemProComponent::SetStatRegenerationRate(EStatType StatType, float Rate)
{
	if (!bEnableStatLayer || !HasStat(StatType))
	{
		return;
	}

	Stats[StatType].RegenerationRate = Rate;
}

float UStatSystemProComponent::GetStatValue(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].CurrentValue;
	}
	return 0.0f;
}

float UStatSystemProComponent::GetStatMaxValue(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].MaxValue;
	}
	return 0.0f;
}

float UStatSystemProComponent::GetStatPercentage(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].GetPercentage();
	}
	return 0.0f;
}

FStatValue UStatSystemProComponent::GetStat(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType];
	}
	return FStatValue();
}

bool UStatSystemProComponent::HasStat(EStatType StatType) const
{
	return Stats.Contains(StatType);
}

bool UStatSystemProComponent::IsStatAtMax(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].IsAtMax();
	}
	return false;
}

bool UStatSystemProComponent::IsStatAtZero(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].IsAtZero();
	}
	return false;
}

bool UStatSystemProComponent::IsStatCritical(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].GetPercentage() < CriticalThreshold;
	}
	return false;
}

EStatType UStatSystemProComponent::GetLowestStat() const
{
	EStatType LowestStat = EStatType::Health_Core;
	float LowestPercentage = 1.0f;

	for (const auto& StatPair : Stats)
	{
		float Percentage = StatPair.Value.GetPercentage();
		if (Percentage < LowestPercentage)
		{
			LowestPercentage = Percentage;
			LowestStat = StatPair.Key;
		}
	}

	return LowestStat;
}

EStatType UStatSystemProComponent::GetHighestStat() const
{
	EStatType HighestStat = EStatType::Health_Core;
	float HighestPercentage = 0.0f;

	for (const auto& StatPair : Stats)
	{
		float Percentage = StatPair.Value.GetPercentage();
		if (Percentage > HighestPercentage)
		{
			HighestPercentage = Percentage;
			HighestStat = StatPair.Key;
		}
	}

	return HighestStat;
}

bool UStatSystemProComponent::IsAnyCritical() const
{
	for (const auto& StatPair : Stats)
	{
		if (StatPair.Value.GetPercentage() < CriticalThreshold)
		{
			return true;
		}
	}
	return false;
}

int32 UStatSystemProComponent::GetStatsBelowThresholdCount(float Threshold) const
{
	int32 Count = 0;
	for (const auto& StatPair : Stats)
	{
		if (StatPair.Value.GetPercentage() < Threshold)
		{
			Count++;
		}
	}
	return Count;
}

void UStatSystemProComponent::TransferStatValue(EStatType FromStat, EStatType ToStat, float Amount)
{
	if (!bEnableStatLayer || !HasStat(FromStat) || !HasStat(ToStat))
	{
		return;
	}

	ApplyStatChange(FromStat, -Amount, TEXT("Transfer"), FGameplayTag());
	ApplyStatChange(ToStat, Amount, TEXT("Transfer"), FGameplayTag());
}

void UStatSystemProComponent::RestoreAllStats()
{
	if (!bEnableStatLayer)
	{
		return;
	}

	for (auto& StatPair : Stats)
	{
		StatPair.Value.CurrentValue = StatPair.Value.MaxValue;
	}

	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: All stats restored to max"));
}

void UStatSystemProComponent::DepleteAllStats()
{
	if (!bEnableStatLayer)
	{
		return;
	}

	for (auto& StatPair : Stats)
	{
		StatPair.Value.CurrentValue = 0.0f;
	}

	UE_LOG(LogTemp, Warning, TEXT("StatSystemPro: All stats depleted to zero"));
}

void UStatSystemProComponent::UpdateStatLayer(float DeltaTime)
{
	if (!bEnableAutoRegeneration)
	{
		return;
	}

	for (auto& StatPair : Stats)
	{
		FStatValue& Stat = StatPair.Value;
		float OldValue = Stat.CurrentValue;
		float RegenerationAmount = 0.0f;

		// Use curve if available
		if (Stat.RegenerationCurve)
		{
			float CurrentPercentage = Stat.GetPercentage();
			float CurveValue = Stat.RegenerationCurve->GetFloatValue(CurrentPercentage);
			RegenerationAmount = CurveValue * DeltaTime;
		}
		// Otherwise use flat rate
		else if (!FMath::IsNearlyZero(Stat.RegenerationRate))
		{
			RegenerationAmount = Stat.RegenerationRate * DeltaTime;
		}

		if (!FMath::IsNearlyZero(RegenerationAmount))
		{
			Stat.CurrentValue += RegenerationAmount;
			Stat.Clamp();

			if (!FMath::IsNearlyEqual(OldValue, Stat.CurrentValue, 0.01f))
			{
				OnStatChanged.Broadcast(StatPair.Key, OldValue, Stat.CurrentValue);
			}
		}
	}
}

// ============================================================================
// BODY LAYER IMPLEMENTATION
// ============================================================================

void UStatSystemProComponent::InitializeBodyParts()
{
	if (!bEnableBodyLayer)
	{
		return;
	}

	BodyParts.Empty();

	// Initialize all body parts
	for (int32 i = 0; i < (int32)EBodyPart::MAX; ++i)
	{
		EBodyPart BodyPart = (EBodyPart)i;
		FBodyPartState NewPart;
		NewPart.Health = 100.0f;
		NewPart.MaxHealth = 100.0f;
		NewPart.bFractured = false;
		NewPart.BleedingRate = 0.0f;
		NewPart.BurnLevel = EBurnLevel::None;
		NewPart.InfectionLevel = 0.0f;
		NewPart.PainLevel = 0.0f;

		BodyParts.Add(BodyPart, NewPart);
	}

	UE_LOG(LogTemp, Log, TEXT("  ✓ Body Layer initialized (%d body parts)"), BodyParts.Num());
}

void UStatSystemProComponent::DamageBodyPart(EBodyPart BodyPart, float Damage)
{
	if (!bEnableBodyLayer || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& Part = BodyParts[BodyPart];
	Part.Health = FMath::Max(0.0f, Part.Health - Damage);
	Part.PainLevel = FMath::Min(100.0f, Part.PainLevel + Damage * 0.5f);

	OnBodyPartDamaged.Broadcast(BodyPart, Damage);

	// Also damage health stat if enabled
	if (bEnableStatLayer)
	{
		ApplyStatChange(EStatType::Health_Core, -Damage * 0.3f, TEXT("BodyDamage"), FGameplayTag());
	}
}

void UStatSystemProComponent::FractureLimb(EBodyPart BodyPart)
{
	if (!bEnableBodyLayer || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& Part = BodyParts[BodyPart];
	Part.bFractured = true;
	Part.PainLevel = FMath::Min(100.0f, Part.PainLevel + 50.0f);

	OnBodyPartFractured.Broadcast(BodyPart);
}

void UStatSystemProComponent::SetBleedingRate(EBodyPart BodyPart, float Rate)
{
	if (!bEnableBodyLayer || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& Part = BodyParts[BodyPart];
	Part.BleedingRate = FMath::Max(0.0f, Rate);

	OnBodyPartBleeding.Broadcast(BodyPart, Part.BleedingRate);
}

void UStatSystemProComponent::SetBurnLevel(EBodyPart BodyPart, EBurnLevel BurnLevel)
{
	if (!bEnableBodyLayer || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& Part = BodyParts[BodyPart];
	Part.BurnLevel = BurnLevel;

	// Add pain based on burn severity
	switch (BurnLevel)
	{
	case EBurnLevel::FirstDegree:
		Part.PainLevel = FMath::Min(100.0f, Part.PainLevel + 10.0f);
		break;
	case EBurnLevel::SecondDegree:
		Part.PainLevel = FMath::Min(100.0f, Part.PainLevel + 30.0f);
		break;
	case EBurnLevel::ThirdDegree:
		Part.PainLevel = FMath::Min(100.0f, Part.PainLevel + 60.0f);
		break;
	default:
		break;
	}

	OnBodyPartBurned.Broadcast(BodyPart, BurnLevel);
}

void UStatSystemProComponent::ApplyInfection(EBodyPart BodyPart, float InfectionAmount)
{
	if (!bEnableBodyLayer || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& Part = BodyParts[BodyPart];
	Part.InfectionLevel = FMath::Clamp(Part.InfectionLevel + InfectionAmount, 0.0f, 100.0f);

	OnBodyPartInfected.Broadcast(BodyPart, Part.InfectionLevel);
}

void UStatSystemProComponent::HealLimb(EBodyPart BodyPart, float HealAmount)
{
	if (!bEnableBodyLayer || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& Part = BodyParts[BodyPart];
	Part.Health = FMath::Min(Part.MaxHealth, Part.Health + HealAmount);
	Part.PainLevel = FMath::Max(0.0f, Part.PainLevel - HealAmount * 0.5f);
}

FBodyPartState UStatSystemProComponent::GetBodyPartState(EBodyPart BodyPart) const
{
	if (BodyParts.Contains(BodyPart))
	{
		return BodyParts[BodyPart];
	}
	return FBodyPartState();
}

float UStatSystemProComponent::GetTotalBodyCondition() const
{
	if (!bEnableBodyLayer || BodyParts.Num() == 0)
	{
		return 100.0f;
	}

	float TotalHealth = 0.0f;
	for (const auto& PartPair : BodyParts)
	{
		TotalHealth += (PartPair.Value.Health / PartPair.Value.MaxHealth) * 100.0f;
	}

	return TotalHealth / BodyParts.Num();
}

float UStatSystemProComponent::GetTotalBleedingRate() const
{
	if (!bEnableBodyLayer)
	{
		return 0.0f;
	}

	float TotalBleeding = 0.0f;
	for (const auto& PartPair : BodyParts)
	{
		TotalBleeding += PartPair.Value.BleedingRate;
	}

	return TotalBleeding;
}

float UStatSystemProComponent::GetTotalPainLevel() const
{
	if (!bEnableBodyLayer || BodyParts.Num() == 0)
	{
		return 0.0f;
	}

	float TotalPain = 0.0f;
	for (const auto& PartPair : BodyParts)
	{
		TotalPain += PartPair.Value.PainLevel;
	}

	return TotalPain / BodyParts.Num();
}

bool UStatSystemProComponent::HasCriticalInjury() const
{
	if (!bEnableBodyLayer)
	{
		return false;
	}

	for (const auto& PartPair : BodyParts)
	{
		const FBodyPartState& Part = PartPair.Value;

		// Critical if head/torso heavily damaged
		if ((PartPair.Key == EBodyPart::Head || PartPair.Key == EBodyPart::Torso) &&
			(Part.Health / Part.MaxHealth) < 0.3f)
		{
			return true;
		}

		// Critical if severe bleeding
		if (Part.BleedingRate > 5.0f)
		{
			return true;
		}

		// Critical if third degree burns
		if (Part.BurnLevel == EBurnLevel::ThirdDegree)
		{
			return true;
		}
	}

	return false;
}

FBodyPartEffectMultipliers UStatSystemProComponent::CalculateEffectMultipliers() const
{
	FBodyPartEffectMultipliers Multipliers;
	Multipliers.MovementSpeedMultiplier = 1.0f;
	Multipliers.AccuracyMultiplier = 1.0f;
	Multipliers.StaminaRegenMultiplier = 1.0f;
	Multipliers.PainMultiplier = 1.0f;

	if (!bEnableBodyLayer)
	{
		return Multipliers;
	}

	// Calculate based on body condition
	float LegCondition = 0.0f;
	if (BodyParts.Contains(EBodyPart::LeftLeg) && BodyParts.Contains(EBodyPart::RightLeg))
	{
		LegCondition = ((BodyParts[EBodyPart::LeftLeg].Health / BodyParts[EBodyPart::LeftLeg].MaxHealth) +
						(BodyParts[EBodyPart::RightLeg].Health / BodyParts[EBodyPart::RightLeg].MaxHealth)) / 2.0f;
	}

	Multipliers.MovementSpeedMultiplier = FMath::Clamp(LegCondition, 0.3f, 1.0f);

	float ArmCondition = 0.0f;
	if (BodyParts.Contains(EBodyPart::LeftArm) && BodyParts.Contains(EBodyPart::RightArm))
	{
		ArmCondition = ((BodyParts[EBodyPart::LeftArm].Health / BodyParts[EBodyPart::LeftArm].MaxHealth) +
						(BodyParts[EBodyPart::RightArm].Health / BodyParts[EBodyPart::RightArm].MaxHealth)) / 2.0f;
	}

	Multipliers.AccuracyMultiplier = FMath::Clamp(ArmCondition, 0.2f, 1.0f);

	float TotalPain = GetTotalPainLevel();
	Multipliers.PainMultiplier = 1.0f + (TotalPain / 100.0f);

	return Multipliers;
}

void UStatSystemProComponent::HealAllBodyParts()
{
	if (!bEnableBodyLayer)
	{
		return;
	}

	for (auto& PartPair : BodyParts)
	{
		PartPair.Value.Health = PartPair.Value.MaxHealth;
		PartPair.Value.PainLevel = 0.0f;
		PartPair.Value.bFractured = false;
		PartPair.Value.InfectionLevel = 0.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: All body parts healed"));
}

void UStatSystemProComponent::StopAllBleeding()
{
	if (!bEnableBodyLayer)
	{
		return;
	}

	for (auto& PartPair : BodyParts)
	{
		PartPair.Value.BleedingRate = 0.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: All bleeding stopped"));
}

void UStatSystemProComponent::UpdateBodyLayer(float DeltaTime)
{
	UpdateBleeding(DeltaTime);

	// Update infections
	for (auto& PartPair : BodyParts)
	{
		FBodyPartState& Part = PartPair.Value;

		if (Part.InfectionLevel > 0.0f)
		{
			// Infection spreads slowly
			Part.InfectionLevel = FMath::Min(100.0f, Part.InfectionLevel + DeltaTime * 0.5f);

			// Infection causes damage
			if (Part.InfectionLevel > 50.0f)
			{
				Part.Health = FMath::Max(0.0f, Part.Health - DeltaTime * 0.2f);
			}
		}
	}
}

void UStatSystemProComponent::UpdateBleeding(float DeltaTime)
{
	float TotalBleeding = GetTotalBleedingRate();

	if (TotalBleeding > 0.0f && bEnableStatLayer)
	{
		// Bleeding drains blood level
		ApplyStatChange(EStatType::BloodLevel, -TotalBleeding * DeltaTime, TEXT("Bleeding"), FGameplayTag());
	}
}

// ============================================================================
// WEATHER LAYER IMPLEMENTATION
// ============================================================================

void UStatSystemProComponent::SetWeather(EWeatherType NewWeather)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	EWeatherType OldWeather = CurrentWeather;
	CurrentWeather = NewWeather;

	if (OldWeather != NewWeather)
	{
		OnWeatherChanged.Broadcast(OldWeather, NewWeather);
	}
}

void UStatSystemProComponent::SetAmbientTemperature(float Temperature)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	float OldTemp = AmbientTemperature;
	AmbientTemperature = Temperature;

	if (!FMath::IsNearlyEqual(OldTemp, AmbientTemperature))
	{
		OnTemperatureChanged.Broadcast(OldTemp, AmbientTemperature);
	}
}

void UStatSystemProComponent::SetWindSpeed(float Speed)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	WindSpeed = FMath::Max(0.0f, Speed);
}

void UStatSystemProComponent::SetWetnessLevel(float Wetness)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	WetnessLevel = FMath::Clamp(Wetness, 0.0f, 100.0f);
}

void UStatSystemProComponent::SetShelterLevel(float Shelter)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	ShelterLevel = FMath::Clamp(Shelter, 0.0f, 100.0f);
}

void UStatSystemProComponent::EquipClothing(EClothingSlot Slot, const FClothingItem& Item)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	EquippedClothing.Add(Slot, Item);
	OnClothingEquipped.Broadcast(Slot, Item);
}

void UStatSystemProComponent::RemoveClothing(EClothingSlot Slot)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	if (EquippedClothing.Remove(Slot) > 0)
	{
		OnClothingRemoved.Broadcast(Slot);
	}
}

FClothingItem UStatSystemProComponent::GetEquippedClothing(EClothingSlot Slot) const
{
	if (EquippedClothing.Contains(Slot))
	{
		return EquippedClothing[Slot];
	}
	return FClothingItem();
}

FTemperatureResult UStatSystemProComponent::CalculateEffectiveTemperature() const
{
	FTemperatureResult Result;
	Result.AmbientTemperature = AmbientTemperature;
	Result.WindChillAdjustment = CalculateWindChill();
	Result.WetnessPenalty = -(WetnessLevel / 100.0f) * 10.0f;
	Result.ClothingProtection = GetTotalClothingInsulation(AmbientTemperature < 15.0f);
	Result.ShelterBonus = (ShelterLevel / 100.0f) * 5.0f;
	Result.EffectiveTemperature = AmbientTemperature + Result.WindChillAdjustment +
								   Result.WetnessPenalty + Result.ClothingProtection +
								   Result.ShelterBonus;

	return Result;
}

void UStatSystemProComponent::ApplyWeatherPreset(const FWeatherPreset& Preset)
{
	if (!bEnableWeatherLayer)
	{
		return;
	}

	SetWeather(Preset.WeatherType);

	float TempVariance = FMath::FRandRange(-Preset.TemperatureVariance, Preset.TemperatureVariance);
	SetAmbientTemperature(Preset.BaseTemperature + TempVariance);

	float WindVariance = FMath::FRandRange(-Preset.WindVariance, Preset.WindVariance);
	SetWindSpeed(FMath::Max(0.0f, Preset.WindSpeed + WindVariance));
}

void UStatSystemProComponent::UpdateWeatherLayer(float DeltaTime)
{
	// Calculate effective temperature
	FTemperatureResult TempResult = CalculateEffectiveTemperature();
	UpdateTemperatureStages(TempResult.EffectiveTemperature);

	// Update body temperature stat if enabled
	if (bEnableStatLayer && HasStat(EStatType::BodyTemperature))
	{
		float TargetBodyTemp = 37.0f; // Normal
		float CurrentBodyTemp = GetStatValue(EStatType::BodyTemperature);

		// Move body temp towards ambient
		float TempDiff = TempResult.EffectiveTemperature - CurrentBodyTemp;
		float TempChange = TempDiff * DeltaTime * 0.1f; // Gradual change

		ApplyStatChange(EStatType::BodyTemperature, TempChange, TEXT("Weather"), FGameplayTag());
	}

	// Apply freezing/overheating damage
	switch (CurrentFreezingStage)
	{
	case EFreezingStage::Cold:
		// Minor discomfort, no damage
		break;
	case EFreezingStage::VeryCold:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Health_Core, -1.0f * DeltaTime, TEXT("Cold"), FGameplayTag());
		}
		break;
	case EFreezingStage::Freezing:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Health_Core, -3.0f * DeltaTime, TEXT("Freezing"), FGameplayTag());
		}
		break;
	case EFreezingStage::Frostbite:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Health_Core, -5.0f * DeltaTime, TEXT("Frostbite"), FGameplayTag());
		}
		break;
	case EFreezingStage::Hypothermia:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Health_Core, -10.0f * DeltaTime, TEXT("Hypothermia"), FGameplayTag());
		}
		break;
	default:
		break;
	}

	switch (CurrentOverheatingStage)
	{
	case EOverheatingStage::Warm:
		// Minor discomfort
		break;
	case EOverheatingStage::Hot:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Stamina, -2.0f * DeltaTime, TEXT("Heat"), FGameplayTag());
		}
		break;
	case EOverheatingStage::VeryHot:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Health_Core, -2.0f * DeltaTime, TEXT("Heat"), FGameplayTag());
			ApplyStatChange(EStatType::Stamina, -5.0f * DeltaTime, TEXT("Heat"), FGameplayTag());
		}
		break;
	case EOverheatingStage::Overheating:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Health_Core, -5.0f * DeltaTime, TEXT("Overheating"), FGameplayTag());
		}
		break;
	case EOverheatingStage::Heatstroke:
		if (bEnableStatLayer)
		{
			ApplyStatChange(EStatType::Health_Core, -10.0f * DeltaTime, TEXT("Heatstroke"), FGameplayTag());
		}
		break;
	default:
		break;
	}
}

void UStatSystemProComponent::UpdateTemperatureStages(float EffectiveTemp)
{
	EFreezingStage OldFreezingStage = CurrentFreezingStage;
	EOverheatingStage OldOverheatingStage = CurrentOverheatingStage;

	// Determine freezing stage
	if (EffectiveTemp < -20.0f)
	{
		CurrentFreezingStage = EFreezingStage::Hypothermia;
	}
	else if (EffectiveTemp < -10.0f)
	{
		CurrentFreezingStage = EFreezingStage::Frostbite;
	}
	else if (EffectiveTemp < 0.0f)
	{
		CurrentFreezingStage = EFreezingStage::Freezing;
	}
	else if (EffectiveTemp < 10.0f)
	{
		CurrentFreezingStage = EFreezingStage::VeryCold;
	}
	else if (EffectiveTemp < 15.0f)
	{
		CurrentFreezingStage = EFreezingStage::Cold;
	}
	else
	{
		CurrentFreezingStage = EFreezingStage::None;
	}

	// Determine overheating stage
	if (EffectiveTemp > 45.0f)
	{
		CurrentOverheatingStage = EOverheatingStage::Heatstroke;
	}
	else if (EffectiveTemp > 40.0f)
	{
		CurrentOverheatingStage = EOverheatingStage::Overheating;
	}
	else if (EffectiveTemp > 35.0f)
	{
		CurrentOverheatingStage = EOverheatingStage::VeryHot;
	}
	else if (EffectiveTemp > 30.0f)
	{
		CurrentOverheatingStage = EOverheatingStage::Hot;
	}
	else if (EffectiveTemp > 25.0f)
	{
		CurrentOverheatingStage = EOverheatingStage::Warm;
	}
	else
	{
		CurrentOverheatingStage = EOverheatingStage::None;
	}

	// Broadcast events if changed
	if (OldFreezingStage != CurrentFreezingStage)
	{
		OnFreezingStageChanged.Broadcast(CurrentFreezingStage);
	}

	if (OldOverheatingStage != CurrentOverheatingStage)
	{
		OnOverheatingStageChanged.Broadcast(CurrentOverheatingStage);
	}
}

float UStatSystemProComponent::CalculateWindChill() const
{
	if (WindSpeed < 5.0f || AmbientTemperature > 10.0f)
	{
		return 0.0f; // No wind chill in warm temps or calm conditions
	}

	// Simplified wind chill calculation
	float WindChillEffect = -0.5f * FMath::Sqrt(WindSpeed);
	return WindChillEffect;
}

float UStatSystemProComponent::GetTotalClothingInsulation(bool bForCold) const
{
	float TotalInsulation = 0.0f;

	for (const auto& ClothingPair : EquippedClothing)
	{
		const FClothingItem& Item = ClothingPair.Value;
		float Effectiveness = 1.0f - (Item.CurrentWetness * 0.7f); // Wet clothing loses 70% effectiveness

		if (bForCold)
		{
			TotalInsulation += (Item.ColdInsulation / 100.0f) * 10.0f * Effectiveness;
		}
		else
		{
			TotalInsulation += (Item.HeatInsulation / 100.0f) * 10.0f * Effectiveness;
		}
	}

	return TotalInsulation;
}

// ============================================================================
// STATUS EFFECT LAYER IMPLEMENTATION
// ============================================================================

void UStatSystemProComponent::ApplyStatusEffect(FName EffectID, int32 Stacks)
{
	if (!bEnableStatusEffectLayer)
	{
		return;
	}

	// Check if effect already exists
	for (FActiveStatusEffect& Effect : ActiveEffects)
	{
		if (Effect.EffectID == EffectID)
		{
			Effect.Stacks += Stacks;
			OnStatusEffectApplied.Broadcast(EffectID, Effect.Stacks);
			return;
		}
	}

	// Add new effect
	FActiveStatusEffect NewEffect;
	NewEffect.EffectID = EffectID;
	NewEffect.Stacks = Stacks;
	NewEffect.RemainingDuration = 0.0f; // Set by definition

	ActiveEffects.Add(NewEffect);
	OnStatusEffectApplied.Broadcast(EffectID, Stacks);
}

void UStatSystemProComponent::RemoveEffect(FName EffectID)
{
	if (!bEnableStatusEffectLayer)
	{
		return;
	}

	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		if (ActiveEffects[i].EffectID == EffectID)
		{
			ActiveEffects.RemoveAt(i);
			OnStatusEffectRemoved.Broadcast(EffectID);
			break;
		}
	}
}

bool UStatSystemProComponent::HasStatusEffect(FName EffectID) const
{
	for (const FActiveStatusEffect& Effect : ActiveEffects)
	{
		if (Effect.EffectID == EffectID)
		{
			return true;
		}
	}
	return false;
}

int32 UStatSystemProComponent::GetStatusEffectStacks(FName EffectID) const
{
	for (const FActiveStatusEffect& Effect : ActiveEffects)
	{
		if (Effect.EffectID == EffectID)
		{
			return Effect.Stacks;
		}
	}
	return 0;
}

void UStatSystemProComponent::ClearAllStatusEffects()
{
	if (!bEnableStatusEffectLayer)
	{
		return;
	}

	ActiveEffects.Empty();
	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: All status effects cleared"));
}

void UStatSystemProComponent::UpdateStatusEffectLayer(float DeltaTime)
{
	// Update effect durations
	UpdateStatusEffectDurations(DeltaTime);
}

void UStatSystemProComponent::UpdateStatusEffectDurations(float DeltaTime)
{
	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		FActiveStatusEffect& Effect = ActiveEffects[i];

		if (Effect.RemainingDuration > 0.0f)
		{
			Effect.RemainingDuration -= DeltaTime;

			if (Effect.RemainingDuration <= 0.0f)
			{
				// Effect expired
				OnStatusEffectExpired.Broadcast(Effect.EffectID, Effect.Stacks);
				ActiveEffects.RemoveAt(i);
			}
		}
	}
}

// ============================================================================
// PROGRESSION LAYER IMPLEMENTATION
// ============================================================================

void UStatSystemProComponent::AwardXP(int32 Amount, EXPSource Source)
{
	if (!bEnableProgressionLayer)
	{
		return;
	}

	int32 OldXP = CurrentXP;
	CurrentXP += Amount;

	OnXPGained.Broadcast(Amount, CurrentXP, Source);

	// Check for level up
	int32 RequiredXP = GetXPForNextLevel();
	while (CurrentXP >= RequiredXP && CurrentLevel < 100)
	{
		CurrentXP -= RequiredXP;
		int32 OldLevel = CurrentLevel;
		CurrentLevel++;
		StatPoints += 5; // Award 5 stat points per level

		OnLevelUp.Broadcast(OldLevel, CurrentLevel);
		OnStatPointsAwarded.Broadcast(5, StatPoints);

		RequiredXP = GetXPForNextLevel();
	}
}

void UStatSystemProComponent::SetLevel(int32 NewLevel)
{
	if (!bEnableProgressionLayer)
	{
		return;
	}

	int32 OldLevel = CurrentLevel;
	CurrentLevel = FMath::Clamp(NewLevel, 1, 100);

	if (OldLevel != CurrentLevel)
	{
		OnLevelUp.Broadcast(OldLevel, CurrentLevel);
	}
}

void UStatSystemProComponent::UnlockSkill(FName SkillID)
{
	if (!bEnableProgressionLayer)
	{
		return;
	}

	if (!UnlockedSkills.Contains(SkillID))
	{
		UnlockedSkills.Add(SkillID);
		OnSkillUnlocked.Broadcast(SkillID);
	}
}

bool UStatSystemProComponent::IsSkillUnlocked(FName SkillID) const
{
	return UnlockedSkills.Contains(SkillID);
}

int32 UStatSystemProComponent::GetXPForNextLevel() const
{
	if (XPCurve)
	{
		return FMath::RoundToInt(XPCurve->GetFloatValue(CurrentLevel));
	}

	// Default formula: 100 * Level^1.5
	return FMath::RoundToInt(100.0f * FMath::Pow(CurrentLevel, 1.5f));
}

float UStatSystemProComponent::GetLevelProgress() const
{
	int32 RequiredXP = GetXPForNextLevel();
	if (RequiredXP > 0)
	{
		return (float)CurrentXP / (float)RequiredXP;
	}
	return 0.0f;
}

void UStatSystemProComponent::SpendStatPoint(EStatType StatType, float Amount)
{
	if (!bEnableProgressionLayer || !bEnableStatLayer || StatPoints <= 0)
	{
		return;
	}

	StatPoints--;
	SetStatMaxValue(StatType, GetStatMaxValue(StatType) + Amount);
}

void UStatSystemProComponent::UpdateProgressionLayer(float DeltaTime)
{
	// Passive XP or other progression logic can go here
}

// ============================================================================
// TIME LAYER IMPLEMENTATION
// ============================================================================

void UStatSystemProComponent::SetTimeMultiplier(float Multiplier)
{
	if (!bEnableTimeLayer)
	{
		return;
	}

	TimeMultiplier = FMath::Clamp(Multiplier, 0.0f, 100.0f);
}

void UStatSystemProComponent::SetCurrentTime(int32 Hour, int32 Day)
{
	if (!bEnableTimeLayer)
	{
		return;
	}

	int32 OldHour = CurrentHour;
	int32 OldDay = CurrentDay;

	CurrentHour = FMath::Clamp(Hour, 0, 23);
	CurrentDay = FMath::Max(1, Day);

	if (OldHour != CurrentHour)
	{
		OnHourChanged.Broadcast(CurrentHour);
		UpdateTimeOfDay();
	}

	if (OldDay != CurrentDay)
	{
		OnDayChanged.Broadcast(CurrentDay);
	}
}

void UStatSystemProComponent::AdvanceTime(float Hours)
{
	if (!bEnableTimeLayer)
	{
		return;
	}

	CurrentGameTime += Hours * 3600.0f; // Convert to seconds

	int32 TotalHours = FMath::FloorToInt(CurrentGameTime / 3600.0f);
	int32 NewHour = TotalHours % 24;
	int32 NewDay = (TotalHours / 24) + 1;

	SetCurrentTime(NewHour, NewDay);
}

FString UStatSystemProComponent::GetFormattedTime() const
{
	return FString::Printf(TEXT("Day %d, %02d:00"), CurrentDay, CurrentHour);
}

void UStatSystemProComponent::UpdateTimeLayer(float DeltaTime)
{
	CurrentGameTime += DeltaTime * TimeMultiplier;

	int32 TotalHours = FMath::FloorToInt(CurrentGameTime / 3600.0f);
	int32 NewHour = TotalHours % 24;
	int32 NewDay = (TotalHours / 24) + 1;

	// Check for hour change
	if (NewHour != CurrentHour)
	{
		CurrentHour = NewHour;
		OnHourChanged.Broadcast(CurrentHour);
		UpdateTimeOfDay();
	}

	// Check for day change
	if (NewDay != CurrentDay)
	{
		CurrentDay = NewDay;
		OnDayChanged.Broadcast(CurrentDay);
	}
}

void UStatSystemProComponent::UpdateTimeOfDay()
{
	ETimeOfDay OldTimeOfDay = CurrentTimeOfDay;
	ETimeOfDay NewTimeOfDay = CurrentTimeOfDay;

	if (CurrentHour >= 5 && CurrentHour < 12)
	{
		NewTimeOfDay = ETimeOfDay::Morning;
	}
	else if (CurrentHour >= 12 && CurrentHour < 17)
	{
		NewTimeOfDay = ETimeOfDay::Afternoon;
	}
	else if (CurrentHour >= 17 && CurrentHour < 20)
	{
		NewTimeOfDay = ETimeOfDay::Evening;
	}
	else
	{
		NewTimeOfDay = ETimeOfDay::Night;
	}

	if (OldTimeOfDay != NewTimeOfDay)
	{
		CurrentTimeOfDay = NewTimeOfDay;
		OnTimeOfDayChanged.Broadcast(OldTimeOfDay, NewTimeOfDay);
	}
}

// ============================================================================
// SAVE/LOAD SYSTEM
// ============================================================================

bool UStatSystemProComponent::QuickSave(const FString& SlotName)
{
	UStatSystemProSaveGame* SaveGameInstance = Cast<UStatSystemProSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UStatSystemProSaveGame::StaticClass()));

	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("StatSystemPro: Failed to create save game object"));
		return false;
	}

	// Save layer toggles
	SaveGameInstance->bEnableStatLayer = bEnableStatLayer;
	SaveGameInstance->bEnableBodyLayer = bEnableBodyLayer;
	SaveGameInstance->bEnableWeatherLayer = bEnableWeatherLayer;
	SaveGameInstance->bEnableStatusEffectLayer = bEnableStatusEffectLayer;
	SaveGameInstance->bEnableProgressionLayer = bEnableProgressionLayer;
	SaveGameInstance->bEnableTimeLayer = bEnableTimeLayer;

	// Save Stat Layer
	if (bEnableStatLayer)
	{
		SaveGameInstance->Stats = Stats;
	}

	// Save Body Layer
	if (bEnableBodyLayer)
	{
		SaveGameInstance->BodyParts = BodyParts;
	}

	// Save Weather Layer
	if (bEnableWeatherLayer)
	{
		SaveGameInstance->CurrentWeather = CurrentWeather;
		SaveGameInstance->AmbientTemperature = AmbientTemperature;
		SaveGameInstance->WindSpeed = WindSpeed;
		SaveGameInstance->WetnessLevel = WetnessLevel;
		SaveGameInstance->ShelterLevel = ShelterLevel;
		SaveGameInstance->EquippedClothing = EquippedClothing;
	}

	// Save Status Effect Layer
	if (bEnableStatusEffectLayer)
	{
		SaveGameInstance->ActiveEffects = ActiveEffects;
	}

	// Save Progression Layer
	if (bEnableProgressionLayer)
	{
		SaveGameInstance->CurrentLevel = CurrentLevel;
		SaveGameInstance->CurrentXP = CurrentXP;
		SaveGameInstance->StatPoints = StatPoints;
		SaveGameInstance->UnlockedSkills = UnlockedSkills;
	}

	// Save Time Layer
	if (bEnableTimeLayer)
	{
		SaveGameInstance->CurrentGameTime = CurrentGameTime;
		SaveGameInstance->CurrentHour = CurrentHour;
		SaveGameInstance->CurrentDay = CurrentDay;
	}

	// Save to disk
	bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("StatSystemPro: Successfully saved to slot '%s'"), *SlotName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StatSystemPro: Failed to save to slot '%s'"), *SlotName);
	}

	return bSuccess;
}

bool UStatSystemProComponent::QuickLoad(const FString& SlotName)
{
	UStatSystemProSaveGame* LoadedGame = Cast<UStatSystemProSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!LoadedGame)
	{
		UE_LOG(LogTemp, Error, TEXT("StatSystemPro: Failed to load from slot '%s'"), *SlotName);
		return false;
	}

	// Load layer toggles
	bEnableStatLayer = LoadedGame->bEnableStatLayer;
	bEnableBodyLayer = LoadedGame->bEnableBodyLayer;
	bEnableWeatherLayer = LoadedGame->bEnableWeatherLayer;
	bEnableStatusEffectLayer = LoadedGame->bEnableStatusEffectLayer;
	bEnableProgressionLayer = LoadedGame->bEnableProgressionLayer;
	bEnableTimeLayer = LoadedGame->bEnableTimeLayer;

	// Load Stat Layer
	if (bEnableStatLayer)
	{
		Stats = LoadedGame->Stats;
	}

	// Load Body Layer
	if (bEnableBodyLayer)
	{
		BodyParts = LoadedGame->BodyParts;
	}

	// Load Weather Layer
	if (bEnableWeatherLayer)
	{
		CurrentWeather = LoadedGame->CurrentWeather;
		AmbientTemperature = LoadedGame->AmbientTemperature;
		WindSpeed = LoadedGame->WindSpeed;
		WetnessLevel = LoadedGame->WetnessLevel;
		ShelterLevel = LoadedGame->ShelterLevel;
		EquippedClothing = LoadedGame->EquippedClothing;
	}

	// Load Status Effect Layer
	if (bEnableStatusEffectLayer)
	{
		ActiveEffects = LoadedGame->ActiveEffects;
	}

	// Load Progression Layer
	if (bEnableProgressionLayer)
	{
		CurrentLevel = LoadedGame->CurrentLevel;
		CurrentXP = LoadedGame->CurrentXP;
		StatPoints = LoadedGame->StatPoints;
		UnlockedSkills = LoadedGame->UnlockedSkills;
	}

	// Load Time Layer
	if (bEnableTimeLayer)
	{
		CurrentGameTime = LoadedGame->CurrentGameTime;
		CurrentHour = LoadedGame->CurrentHour;
		CurrentDay = LoadedGame->CurrentDay;
	}

	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: Successfully loaded from slot '%s'"), *SlotName);
	return true;
}

// ============================================================================
// REPLICATION CALLBACKS
// ============================================================================

void UStatSystemProComponent::OnRep_Stats()
{
	// Notify clients that stats have been updated
}

void UStatSystemProComponent::OnRep_BodyParts()
{
	// Notify clients that body parts have been updated
}

void UStatSystemProComponent::OnRep_CurrentWeather()
{
	// Notify clients that weather has changed
}

void UStatSystemProComponent::OnRep_EquippedClothing()
{
	// Notify clients that clothing has changed
}

void UStatSystemProComponent::OnRep_ActiveEffects()
{
	// Notify clients that status effects have changed
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

FString UStatSystemProComponent::GetDebugInfo() const
{
	FString Info = TEXT("=== StatSystemPro UNIFIED Component Debug ===\n\n");

	// Layer Status
	Info += TEXT("ENABLED LAYERS:\n");
	Info += FString::Printf(TEXT("  Stat Layer: %s\n"), bEnableStatLayer ? TEXT("✓") : TEXT("✗"));
	Info += FString::Printf(TEXT("  Body Layer: %s\n"), bEnableBodyLayer ? TEXT("✓") : TEXT("✗"));
	Info += FString::Printf(TEXT("  Weather Layer: %s\n"), bEnableWeatherLayer ? TEXT("✓") : TEXT("✗"));
	Info += FString::Printf(TEXT("  Status Effect Layer: %s\n"), bEnableStatusEffectLayer ? TEXT("✓") : TEXT("✗"));
	Info += FString::Printf(TEXT("  Progression Layer: %s\n"), bEnableProgressionLayer ? TEXT("✓") : TEXT("✗"));
	Info += FString::Printf(TEXT("  Time Layer: %s\n\n"), bEnableTimeLayer ? TEXT("✓") : TEXT("✗"));

	// Stat Layer
	if (bEnableStatLayer)
	{
		Info += TEXT("STAT LAYER:\n");
		Info += FString::Printf(TEXT("  Health: %.1f/%.1f (%.0f%%)\n"),
			GetStatValue(EStatType::Health_Core),
			GetStatMaxValue(EStatType::Health_Core),
			GetStatPercentage(EStatType::Health_Core) * 100.0f);
		Info += FString::Printf(TEXT("  Stamina: %.1f/%.1f\n"),
			GetStatValue(EStatType::Stamina),
			GetStatMaxValue(EStatType::Stamina));
		Info += TEXT("\n");
	}

	// Body Layer
	if (bEnableBodyLayer)
	{
		Info += TEXT("BODY LAYER:\n");
		Info += FString::Printf(TEXT("  Body Condition: %.1f%%\n"), GetTotalBodyCondition());
		Info += FString::Printf(TEXT("  Total Bleeding: %.2f/s\n"), GetTotalBleedingRate());
		Info += FString::Printf(TEXT("  Pain Level: %.1f\n"), GetTotalPainLevel());
		Info += TEXT("\n");
	}

	// Weather Layer
	if (bEnableWeatherLayer)
	{
		Info += TEXT("WEATHER LAYER:\n");
		Info += FString::Printf(TEXT("  Ambient Temp: %.1f°C\n"), AmbientTemperature);
		Info += FString::Printf(TEXT("  Wind Speed: %.1f m/s\n"), WindSpeed);
		Info += FString::Printf(TEXT("  Clothing Items: %d\n"), EquippedClothing.Num());
		Info += TEXT("\n");
	}

	// Status Effects
	if (bEnableStatusEffectLayer)
	{
		Info += FString::Printf(TEXT("STATUS EFFECTS: %d active\n\n"), ActiveEffects.Num());
	}

	// Progression
	if (bEnableProgressionLayer)
	{
		Info += TEXT("PROGRESSION:\n");
		Info += FString::Printf(TEXT("  Level: %d\n"), CurrentLevel);
		Info += FString::Printf(TEXT("  XP: %d/%d (%.0f%%)\n"),
			CurrentXP, GetXPForNextLevel(), GetLevelProgress() * 100.0f);
		Info += FString::Printf(TEXT("  Stat Points: %d\n"), StatPoints);
		Info += TEXT("\n");
	}

	// Time
	if (bEnableTimeLayer)
	{
		Info += TEXT("TIME:\n");
		Info += FString::Printf(TEXT("  %s\n"), *GetFormattedTime());
		Info += TEXT("\n");
	}

	// Overall Status
	Info += TEXT("OVERALL:\n");
	Info += FString::Printf(TEXT("  Critical: %s\n"), IsCritical() ? TEXT("YES") : TEXT("NO"));
	Info += FString::Printf(TEXT("  Health Status: %.0f%%\n"), GetOverallHealthStatus() * 100.0f);

	return Info;
}

bool UStatSystemProComponent::IsCritical() const
{
	if (bEnableStatLayer)
	{
		// Critical if health very low
		if (GetStatPercentage(EStatType::Health_Core) < 0.15f)
		{
			return true;
		}

		// Critical if blood level very low
		if (GetStatPercentage(EStatType::BloodLevel) < 0.3f)
		{
			return true;
		}
	}

	if (bEnableBodyLayer)
	{
		if (HasCriticalInjury())
		{
			return true;
		}
	}

	return false;
}

float UStatSystemProComponent::GetOverallHealthStatus() const
{
	float HealthStatus = 1.0f;

	if (bEnableStatLayer)
	{
		float HealthPercent = GetStatPercentage(EStatType::Health_Core);
		float BloodPercent = GetStatPercentage(EStatType::BloodLevel);
		HealthStatus = (HealthPercent * 0.5f) + (BloodPercent * 0.5f);
	}

	if (bEnableBodyLayer)
	{
		float BodyCondition = GetTotalBodyCondition() / 100.0f;
		HealthStatus = (HealthStatus * 0.7f) + (BodyCondition * 0.3f);
	}

	return FMath::Clamp(HealthStatus, 0.0f, 1.0f);
}
