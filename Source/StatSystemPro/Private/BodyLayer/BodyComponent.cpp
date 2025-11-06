// Copyright Epic Games, Inc. All Rights Reserved.

#include "BodyLayer/BodyComponent.h"
#include "StatLayer/StatComponent.h"

UBodyComponent::UBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
	StatComponent = nullptr;
}

void UBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeBodyParts();

	// Try to find stat component on the same actor
	if (!StatComponent)
	{
		StatComponent = GetOwner()->FindComponentByClass<UStatComponent>();
	}
}

void UBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnabled)
	{
		return;
	}

	UpdateBleeding(DeltaTime);
	UpdateInfection(DeltaTime);
	ApplyBodyEffectsToStats();
}

void UBodyComponent::InitializeBodyParts()
{
	BodyParts.Empty();

	// Initialize all body parts with default values
	for (int32 i = 0; i < (int32)EBodyPart::MAX; ++i)
	{
		EBodyPart BodyPart = (EBodyPart)i;
		BodyParts.Add(BodyPart, FBodyPartState());
	}
}

void UBodyComponent::DamageBodyPart(EBodyPart BodyPart, float Damage)
{
	if (!bEnabled || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& State = BodyParts[BodyPart];
	State.Condition = FMath::Max(0.0f, State.Condition - Damage);

	// Increase pain based on damage
	State.PainLevel = FMath::Min(100.0f, State.PainLevel + Damage * 0.5f);

	OnBodyPartDamaged.Broadcast(BodyPart, Damage);

	UE_LOG(LogTemp, Warning, TEXT("Body Part Damaged: %d | Damage: %.2f | Condition: %.2f"),
		(int32)BodyPart, Damage, State.Condition);
}

void UBodyComponent::FractureLimb(EBodyPart BodyPart)
{
	if (!bEnabled || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& State = BodyParts[BodyPart];
	State.bFractured = true;
	State.PainLevel = FMath::Min(100.0f, State.PainLevel + 50.0f);

	OnBodyPartFractured.Broadcast(BodyPart);

	UE_LOG(LogTemp, Warning, TEXT("Body Part Fractured: %d"), (int32)BodyPart);
}

void UBodyComponent::SetBleedingRate(EBodyPart BodyPart, float Rate)
{
	if (!bEnabled || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& State = BodyParts[BodyPart];
	State.BleedingRate = FMath::Max(0.0f, Rate);

	if (State.BleedingRate > 0.0f)
	{
		OnBodyPartBleeding.Broadcast(BodyPart, State.BleedingRate);
	}
}

void UBodyComponent::SetBurnLevel(EBodyPart BodyPart, EBurnLevel BurnLevel)
{
	if (!bEnabled || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& State = BodyParts[BodyPart];
	State.BurnLevel = BurnLevel;

	// Apply pain based on burn severity
	float PainIncrease = 0.0f;
	switch (BurnLevel)
	{
	case EBurnLevel::FirstDegree:
		PainIncrease = 10.0f;
		break;
	case EBurnLevel::SecondDegree:
		PainIncrease = 30.0f;
		break;
	case EBurnLevel::ThirdDegree:
		PainIncrease = 50.0f;
		break;
	default:
		break;
	}

	State.PainLevel = FMath::Min(100.0f, State.PainLevel + PainIncrease);

	OnBodyPartBurned.Broadcast(BodyPart, BurnLevel);
}

void UBodyComponent::ApplyInfection(EBodyPart BodyPart, float InfectionAmount)
{
	if (!bEnabled || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& State = BodyParts[BodyPart];
	State.InfectionRate = FMath::Clamp(State.InfectionRate + InfectionAmount, 0.0f, 100.0f);

	if (State.InfectionRate > 0.0f)
	{
		OnBodyPartInfected.Broadcast(BodyPart, State.InfectionRate);
	}
}

void UBodyComponent::HealLimb(EBodyPart BodyPart, float HealAmount)
{
	if (!bEnabled || !BodyParts.Contains(BodyPart))
	{
		return;
	}

	FBodyPartState& State = BodyParts[BodyPart];
	State.Condition = FMath::Min(State.MaxCondition, State.Condition + HealAmount);

	// Reduce pain when healing
	State.PainLevel = FMath::Max(0.0f, State.PainLevel - HealAmount * 0.3f);
}

FBodyPartState UBodyComponent::GetBodyPartState(EBodyPart BodyPart) const
{
	if (BodyParts.Contains(BodyPart))
	{
		return BodyParts[BodyPart];
	}
	return FBodyPartState();
}

float UBodyComponent::GetTotalBodyCondition() const
{
	if (BodyParts.Num() == 0)
	{
		return 100.0f;
	}

	float TotalCondition = 0.0f;
	for (const auto& Pair : BodyParts)
	{
		TotalCondition += Pair.Value.GetConditionPercentage();
	}

	return (TotalCondition / BodyParts.Num()) * 100.0f;
}

float UBodyComponent::GetTotalBleedingRate() const
{
	float TotalBleeding = 0.0f;
	for (const auto& Pair : BodyParts)
	{
		TotalBleeding += Pair.Value.BleedingRate;
	}
	return TotalBleeding;
}

float UBodyComponent::GetTotalPainLevel() const
{
	float TotalPain = 0.0f;
	int32 Count = 0;

	for (const auto& Pair : BodyParts)
	{
		if (Pair.Value.PainLevel > 0.0f)
		{
			TotalPain += Pair.Value.PainLevel;
			Count++;
		}
	}

	return Count > 0 ? TotalPain / Count : 0.0f;
}

FBodyPartEffectMultipliers UBodyComponent::CalculateEffectMultipliers() const
{
	FBodyPartEffectMultipliers Multipliers;

	// Get leg conditions
	float LeftLegCondition = BodyParts.Contains(EBodyPart::LeftLeg) ?
		BodyParts[EBodyPart::LeftLeg].GetConditionPercentage() : 1.0f;
	float RightLegCondition = BodyParts.Contains(EBodyPart::RightLeg) ?
		BodyParts[EBodyPart::RightLeg].GetConditionPercentage() : 1.0f;
	float AvgLegCondition = (LeftLegCondition + RightLegCondition) / 2.0f;

	// Movement speed affected by leg condition
	Multipliers.MovementSpeedMultiplier = FMath::Lerp(0.3f, 1.0f, AvgLegCondition);

	// Stamina drain increases when legs are damaged
	Multipliers.StaminaDrainMultiplier = FMath::Lerp(2.0f, 1.0f, AvgLegCondition);

	// Get arm conditions
	float LeftArmCondition = BodyParts.Contains(EBodyPart::LeftArm) ?
		BodyParts[EBodyPart::LeftArm].GetConditionPercentage() : 1.0f;
	float RightArmCondition = BodyParts.Contains(EBodyPart::RightArm) ?
		BodyParts[EBodyPart::RightArm].GetConditionPercentage() : 1.0f;
	float AvgArmCondition = (LeftArmCondition + RightArmCondition) / 2.0f;

	// Accuracy and weapon sway affected by arm condition
	Multipliers.AccuracyMultiplier = FMath::Lerp(0.4f, 1.0f, AvgArmCondition);
	Multipliers.WeaponSwayMultiplier = FMath::Lerp(2.5f, 1.0f, AvgArmCondition);

	// Torso condition affects max health
	float TorsoCondition = BodyParts.Contains(EBodyPart::Torso) ?
		BodyParts[EBodyPart::Torso].GetConditionPercentage() : 1.0f;
	Multipliers.MaxHealthMultiplier = FMath::Lerp(0.5f, 1.0f, TorsoCondition);

	// Head condition affects sanity and consciousness
	float HeadCondition = BodyParts.Contains(EBodyPart::Head) ?
		BodyParts[EBodyPart::Head].GetConditionPercentage() : 1.0f;
	Multipliers.SanityDrainRate = FMath::Lerp(5.0f, 0.0f, HeadCondition);
	Multipliers.UnconsciousChance = FMath::Lerp(0.5f, 0.0f, HeadCondition);

	return Multipliers;
}

bool UBodyComponent::HasCriticalInjury() const
{
	for (const auto& Pair : BodyParts)
	{
		if (Pair.Value.IsCritical())
		{
			return true;
		}
	}
	return false;
}

void UBodyComponent::UpdateBleeding(float DeltaTime)
{
	float TotalBleedingRate = GetTotalBleedingRate();

	if (TotalBleedingRate > 0.0f && StatComponent)
	{
		// Apply blood loss to the stat component
		StatComponent->ApplyStatChange(
			EStatType::BloodLevel,
			-TotalBleedingRate * DeltaTime,
			TEXT("Bleeding"),
			FGameplayTag()
		);
	}
}

void UBodyComponent::UpdateInfection(float DeltaTime)
{
	// Progress infection over time
	for (auto& Pair : BodyParts)
	{
		FBodyPartState& State = Pair.Value;
		if (State.InfectionRate > 0.0f)
		{
			// Infection slowly grows
			State.InfectionRate = FMath::Min(100.0f, State.InfectionRate + DeltaTime * 0.5f);

			// Infection causes pain
			State.PainLevel = FMath::Min(100.0f, State.PainLevel + DeltaTime * 0.1f);

			// Apply infection to global infection stat
			if (StatComponent)
			{
				StatComponent->ApplyStatChange(
					EStatType::Infection_Level,
					DeltaTime * 0.2f,
					TEXT("BodyInfection"),
					FGameplayTag()
				);
			}
		}
	}
}

void UBodyComponent::ApplyBodyEffectsToStats()
{
	if (!StatComponent)
	{
		return;
	}

	FBodyPartEffectMultipliers Multipliers = CalculateEffectMultipliers();

	// Apply torso damage to max health
	float CurrentHealthMax = StatComponent->GetStatMaxValue(EStatType::Health_Core);
	float BaseHealthMax = StatComponent->GetStat(EStatType::Health_Core).BaseMaxValue;
	float NewMaxHealth = BaseHealthMax * Multipliers.MaxHealthMultiplier;

	if (!FMath::IsNearlyEqual(CurrentHealthMax, NewMaxHealth))
	{
		StatComponent->SetStatMaxValue(EStatType::Health_Core, NewMaxHealth);
	}

	// Apply head damage to sanity
	if (Multipliers.SanityDrainRate > 0.0f)
	{
		StatComponent->ApplyStatChange(
			EStatType::Sanity,
			-Multipliers.SanityDrainRate * GetWorld()->GetDeltaSeconds(),
			TEXT("HeadInjury"),
			FGameplayTag()
		);
	}
}
