// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatLayer/StatComponent.h"
#include "Engine/DataTable.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
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

	UpdateStatRegeneration(DeltaTime);
}

void UStatComponent::InitializeStats()
{
	Stats.Empty();

	// If we have a config table, use it
	if (StatConfigTable)
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
	}
	else
	{
		// Initialize with default values for all stat types
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
				break;
			default:
				break;
			}

			Stats.Add(StatType, DefaultStat);
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
