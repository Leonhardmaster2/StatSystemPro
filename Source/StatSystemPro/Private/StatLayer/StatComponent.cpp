// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatLayer/StatComponent.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
	bUseSimpleMode = true;  // Simple by default!
	bEnableAutoRegeneration = true;
	CriticalThreshold = 0.15f;  // 15%

	// Enable replication
	SetIsReplicatedByDefault(true);
}

void UStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate stats map to all clients
	DOREPLIFETIME(UStatComponent, Stats);
	DOREPLIFETIME(UStatComponent, bEnabled);
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeStats();
}

void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnabled)
	{
		return;
	}

	// Only run regen if enabled and we have authority (server)
	if (bEnableAutoRegeneration && GetOwnerRole() == ROLE_Authority)
	{
		UpdateStatRegeneration(DeltaTime);
	}

	// Check for critical stats
	CheckCriticalStats();
}

void UStatComponent::InitializeStats()
{
	// Only server initializes stats
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	Stats.Empty();

	// SIMPLE MODE: Quick setup with defaults
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
				DefaultStat.CurrentValue = 37.0f; // Normal body temp
				DefaultStat.MaxValue = 42.0f;
				DefaultStat.BaseMaxValue = 42.0f;
				break;
			case EStatType::Wetness:
				DefaultStat.CurrentValue = 0.0f;
				DefaultStat.MaxValue = 100.0f;
				DefaultStat.BaseMaxValue = 100.0f;
				break;
			default:
				break;
			}

			Stats.Add(StatType, DefaultStat);
		}

		UE_LOG(LogTemp, Log, TEXT("StatComponent: Initialized in SIMPLE MODE - all stats = 100"));
	}
	// ADVANCED MODE: Use data table configuration
	else if (StatConfigTable)
	{
		TArray<FStatConfigRow*> Rows;
		StatConfigTable->GetAllRows<FStatConfigRow>(TEXT("StatComponent::InitializeStats"), Rows);

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

		UE_LOG(LogTemp, Log, TEXT("StatComponent: Initialized in ADVANCED MODE from data table"));
	}
	else
	{
		// Fallback: Same as simple mode
		UE_LOG(LogTemp, Warning, TEXT("StatComponent: No data table assigned in Advanced Mode, falling back to defaults"));

		for (int32 i = 0; i < (int32)EStatType::MAX; ++i)
		{
			Stats.Add((EStatType)i, FStatValue(100.0f));
		}
	}
}

void UStatComponent::ApplyStatChange(EStatType StatType, float Amount, FName Source, FGameplayTag ReasonTag)
{
	if (!bEnabled || !HasStat(StatType))
	{
		return;
	}

	FStatValue& Stat = Stats[StatType];
	float OldValue = Stat.CurrentValue;
	Stat.CurrentValue += Amount;
	Stat.Clamp();

	BroadcastStatEvents(StatType, OldValue, Stat.CurrentValue);

	// Log the change for debugging
	UE_LOG(LogTemp, Verbose, TEXT("Stat Change: %d | Amount: %.2f | Source: %s"),
		(int32)StatType, Amount, *Source.ToString());
}

void UStatComponent::SetStatValue(EStatType StatType, float NewValue)
{
	if (!bEnabled || !HasStat(StatType))
	{
		return;
	}

	FStatValue& Stat = Stats[StatType];
	float OldValue = Stat.CurrentValue;
	Stat.CurrentValue = NewValue;
	Stat.Clamp();

	BroadcastStatEvents(StatType, OldValue, Stat.CurrentValue);
}

void UStatComponent::SetStatMaxValue(EStatType StatType, float NewMaxValue)
{
	if (!bEnabled || !HasStat(StatType))
	{
		return;
	}

	FStatValue& Stat = Stats[StatType];
	Stat.MaxValue = FMath::Max(0.0f, NewMaxValue);
	Stat.Clamp();

	OnStatMaxChanged.Broadcast(StatType, Stat.MaxValue);
}

float UStatComponent::GetStatValue(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].CurrentValue;
	}
	return 0.0f;
}

float UStatComponent::GetStatMaxValue(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].MaxValue;
	}
	return 0.0f;
}

float UStatComponent::GetStatPercentage(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].GetPercentage();
	}
	return 0.0f;
}

void UStatComponent::SetStatRegenerationRate(EStatType StatType, float Rate)
{
	if (!bEnabled || !HasStat(StatType))
	{
		return;
	}

	Stats[StatType].RegenerationRate = Rate;
}

FStatValue UStatComponent::GetStat(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType];
	}
	return FStatValue();
}

bool UStatComponent::HasStat(EStatType StatType) const
{
	return Stats.Contains(StatType);
}

void UStatComponent::UpdateStatRegeneration(float DeltaTime)
{
	for (auto& StatPair : Stats)
	{
		FStatValue& Stat = StatPair.Value;
		float OldValue = Stat.CurrentValue;

		// Apply regeneration/decay
		if (!FMath::IsNearlyZero(Stat.RegenerationRate))
		{
			float RegenerationAmount = Stat.RegenerationRate * DeltaTime;

			// If there's a curve, use it to modify the regeneration
			if (Stat.RegenerationCurve)
			{
				float CurveValue = Stat.RegenerationCurve->GetFloatValue(Stat.GetPercentage());
				RegenerationAmount *= CurveValue;
			}

			Stat.CurrentValue += RegenerationAmount;
			Stat.Clamp();

			// Broadcast events if value changed significantly
			if (!FMath::IsNearlyEqual(OldValue, Stat.CurrentValue, 0.01f))
			{
				BroadcastStatEvents(StatPair.Key, OldValue, Stat.CurrentValue);
			}
		}
	}
}

void UStatComponent::BroadcastStatEvents(EStatType StatType, float OldValue, float NewValue)
{
	if (!FMath::IsNearlyEqual(OldValue, NewValue))
	{
		OnStatChanged.Broadcast(StatType, OldValue, NewValue);

		const FStatValue& Stat = Stats[StatType];

		// Check for zero
		if (Stat.IsAtZero() && !FMath::IsNearlyZero(OldValue))
		{
			OnStatReachedZero.Broadcast(StatType);
		}

		// Check for max
		if (Stat.IsAtMax() && !FMath::IsNearlyEqual(OldValue, Stat.MaxValue))
		{
			OnStatReachedMax.Broadcast(StatType);
		}
	}
}

void UStatComponent::OnRep_Stats()
{
	// Called on clients when stats are replicated from server
	// Broadcast events so UI can update
	for (const auto& StatPair : Stats)
	{
		OnStatChanged.Broadcast(StatPair.Key, StatPair.Value.CurrentValue, StatPair.Value.CurrentValue);
	}
}

void UStatComponent::CheckCriticalStats()
{
	for (const auto& StatPair : Stats)
	{
		if (StatPair.Value.GetPercentage() < CriticalThreshold && StatPair.Value.CurrentValue > 0.0f)
		{
			OnStatCritical.Broadcast(StatPair.Key, StatPair.Value.CurrentValue);
		}
	}
}

// ========== NEW GETTER FUNCTIONS ==========

float UStatComponent::GetStatPercentage100(EStatType StatType) const
{
	return GetStatPercentage(StatType) * 100.0f;
}

float UStatComponent::GetStatMissingAmount(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].MaxValue - Stats[StatType].CurrentValue;
	}
	return 0.0f;
}

bool UStatComponent::IsStatAtMax(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].IsAtMax();
	}
	return false;
}

bool UStatComponent::IsStatAtZero(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].IsAtZero();
	}
	return false;
}

bool UStatComponent::IsStatCritical(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].GetPercentage() < CriticalThreshold;
	}
	return false;
}

float UStatComponent::GetStatRegenRate(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].RegenerationRate;
	}
	return 0.0f;
}

float UStatComponent::GetStatBaseMax(EStatType StatType) const
{
	if (HasStat(StatType))
	{
		return Stats[StatType].BaseMaxValue;
	}
	return 0.0f;
}

TArray<EStatType> UStatComponent::GetAllStatTypes() const
{
	TArray<EStatType> AllTypes;
	Stats.GetKeys(AllTypes);
	return AllTypes;
}

bool UStatComponent::IsAnyStatCritical() const
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

float UStatComponent::GetAverageStatHealth() const
{
	if (Stats.Num() == 0)
	{
		return 1.0f;
	}

	float TotalPercentage = 0.0f;
	for (const auto& StatPair : Stats)
	{
		TotalPercentage += StatPair.Value.GetPercentage();
	}

	return TotalPercentage / Stats.Num();
}
