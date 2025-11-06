// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatusEffectLayer/StatusEffectComponent.h"
#include "StatLayer/StatComponent.h"
#include "Engine/DataTable.h"

UStatusEffectComponent::UStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
	StatComponent = nullptr;
	StatusEffectTable = nullptr;
}

void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// Try to find stat component on the same actor
	if (!StatComponent)
	{
		StatComponent = GetOwner()->FindComponentByClass<UStatComponent>();
	}
}

void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnabled)
	{
		return;
	}

	UpdateEffectTimers(DeltaTime);
	ApplyEffectModifiers();
}

bool UStatusEffectComponent::ApplyStatusEffect(FName EffectID, int32 Stacks)
{
	if (!bEnabled)
	{
		return false;
	}

	FStatusEffectData* EffectData = FindEffectData(EffectID);
	if (!EffectData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Status Effect not found in data table: %s"), *EffectID.ToString());
		return false;
	}

	return ApplyStatusEffectFromData(*EffectData, Stacks);
}

bool UStatusEffectComponent::ApplyStatusEffectFromData(const FStatusEffectData& EffectData, int32 Stacks)
{
	if (!bEnabled)
	{
		return false;
	}

	// Check if effect already exists
	int32 ExistingIndex = FindActiveEffectIndex(EffectData.EffectID);

	if (ExistingIndex != INDEX_NONE)
	{
		FActiveStatusEffect& ExistingEffect = ActiveEffects[ExistingIndex];

		// Handle stacking
		if (EffectData.EffectType == EStatusEffectType::Stackable && ExistingEffect.CanStack())
		{
			ExistingEffect.CurrentStacks = FMath::Min(
				ExistingEffect.CurrentStacks + Stacks,
				EffectData.MaxStacks
			);
			ExistingEffect.TimeRemaining = EffectData.Duration; // Reset timer
			OnStatusEffectApplied.Broadcast(EffectData.EffectID, ExistingEffect.CurrentStacks);
			return true;
		}
		else if (EffectData.EffectType == EStatusEffectType::Temporary)
		{
			// Refresh duration
			ExistingEffect.TimeRemaining = EffectData.Duration;
			return true;
		}
		else
		{
			// Effect doesn't stack, ignore
			return false;
		}
	}

	// Add new effect
	FActiveStatusEffect NewEffect(EffectData);
	NewEffect.CurrentStacks = Stacks;
	NewEffect.TimeApplied = GetWorld()->GetTimeSeconds();
	ActiveEffects.Add(NewEffect);

	OnStatusEffectApplied.Broadcast(EffectData.EffectID, Stacks);

	UE_LOG(LogTemp, Log, TEXT("Status Effect Applied: %s | Stacks: %d"), *EffectData.EffectID.ToString(), Stacks);

	return true;
}

bool UStatusEffectComponent::RemoveEffect(FName EffectID)
{
	int32 Index = FindActiveEffectIndex(EffectID);
	if (Index != INDEX_NONE)
	{
		ActiveEffects.RemoveAt(Index);
		OnStatusEffectRemoved.Broadcast(EffectID);
		UE_LOG(LogTemp, Log, TEXT("Status Effect Removed: %s"), *EffectID.ToString());
		return true;
	}
	return false;
}

int32 UStatusEffectComponent::RemoveEffectsByTag(FGameplayTag Tag)
{
	int32 RemovedCount = 0;

	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		if (ActiveEffects[i].EffectData.EffectTags.HasTag(Tag))
		{
			FName EffectID = ActiveEffects[i].EffectData.EffectID;
			ActiveEffects.RemoveAt(i);
			OnStatusEffectRemoved.Broadcast(EffectID);
			RemovedCount++;
		}
	}

	return RemovedCount;
}

void UStatusEffectComponent::ClearAllEffects()
{
	for (const FActiveStatusEffect& Effect : ActiveEffects)
	{
		OnStatusEffectRemoved.Broadcast(Effect.EffectData.EffectID);
	}
	ActiveEffects.Empty();
}

bool UStatusEffectComponent::HasEffect(FName EffectID) const
{
	return FindActiveEffectIndex(EffectID) != INDEX_NONE;
}

FActiveStatusEffect UStatusEffectComponent::GetActiveEffect(FName EffectID) const
{
	int32 Index = FindActiveEffectIndex(EffectID);
	if (Index != INDEX_NONE)
	{
		return ActiveEffects[Index];
	}
	return FActiveStatusEffect();
}

TArray<FActiveStatusEffect> UStatusEffectComponent::GetEffectsByTag(FGameplayTag Tag) const
{
	TArray<FActiveStatusEffect> MatchingEffects;

	for (const FActiveStatusEffect& Effect : ActiveEffects)
	{
		if (Effect.EffectData.EffectTags.HasTag(Tag))
		{
			MatchingEffects.Add(Effect);
		}
	}

	return MatchingEffects;
}

int32 UStatusEffectComponent::GetEffectStacks(FName EffectID) const
{
	int32 Index = FindActiveEffectIndex(EffectID);
	if (Index != INDEX_NONE)
	{
		return ActiveEffects[Index].CurrentStacks;
	}
	return 0;
}

float UStatusEffectComponent::GetEffectTimeRemaining(FName EffectID) const
{
	int32 Index = FindActiveEffectIndex(EffectID);
	if (Index != INDEX_NONE)
	{
		return ActiveEffects[Index].TimeRemaining;
	}
	return 0.0f;
}

void UStatusEffectComponent::UpdateEffectTimers(float DeltaTime)
{
	// Update timers and remove expired effects
	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		FActiveStatusEffect& Effect = ActiveEffects[i];

		// Skip permanent effects
		if (Effect.EffectData.EffectType == EStatusEffectType::Permanent)
		{
			continue;
		}

		// Update timer for temporary/conditional effects
		if (Effect.EffectData.Duration > 0.0f)
		{
			Effect.TimeRemaining -= DeltaTime;

			if (Effect.HasExpired())
			{
				FName EffectID = Effect.EffectData.EffectID;
				float Duration = Effect.EffectData.Duration;
				ActiveEffects.RemoveAt(i);
				OnStatusEffectExpired.Broadcast(EffectID, Duration);
				UE_LOG(LogTemp, Log, TEXT("Status Effect Expired: %s"), *EffectID.ToString());
			}
		}
	}
}

void UStatusEffectComponent::ApplyEffectModifiers()
{
	if (!StatComponent)
	{
		return;
	}

	// Sort effects by priority (higher priority first)
	ActiveEffects.Sort([](const FActiveStatusEffect& A, const FActiveStatusEffect& B)
	{
		return A.EffectData.Priority > B.EffectData.Priority;
	});

	// Apply stat modifiers from all active effects
	for (const FActiveStatusEffect& Effect : ActiveEffects)
	{
		for (const FStatusEffectStatModifier& Modifier : Effect.EffectData.StatModifiers)
		{
			// Convert stat name to enum (simplified, would need proper mapping)
			// This is a basic implementation - in production you'd want a more robust system
			// For now, this demonstrates the concept
		}
	}
}

FStatusEffectData* UStatusEffectComponent::FindEffectData(FName EffectID)
{
	if (!StatusEffectTable)
	{
		return nullptr;
	}

	FString ContextString(TEXT("StatusEffectComponent::FindEffectData"));
	FStatusEffectTableRow* Row = StatusEffectTable->FindRow<FStatusEffectTableRow>(EffectID, ContextString);

	if (Row)
	{
		return &Row->EffectData;
	}

	return nullptr;
}

int32 UStatusEffectComponent::FindActiveEffectIndex(FName EffectID) const
{
	for (int32 i = 0; i < ActiveEffects.Num(); ++i)
	{
		if (ActiveEffects[i].EffectData.EffectID == EffectID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}
