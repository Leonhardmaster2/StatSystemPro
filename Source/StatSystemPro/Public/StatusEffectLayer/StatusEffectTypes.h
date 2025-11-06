// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "StatusEffectTypes.generated.h"

/**
 * Enum for effect types
 */
UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	/** Short-term buff/debuff with fixed duration */
	Temporary UMETA(DisplayName = "Temporary"),

	/** Condition-based effect (persists while condition is met) */
	Conditional UMETA(DisplayName = "Conditional"),

	/** Permanent effect (traits, perks, mutations) */
	Permanent UMETA(DisplayName = "Permanent"),

	/** Effect that can stack multiple times */
	Stackable UMETA(DisplayName = "Stackable"),

	MAX UMETA(Hidden)
};

/**
 * Enum for effect triggers
 */
UENUM(BlueprintType)
enum class EStatusEffectTrigger : uint8
{
	Manual UMETA(DisplayName = "Manual"),
	OnDamage UMETA(DisplayName = "On Damage"),
	OnTime UMETA(DisplayName = "Over Time"),
	OnEnvironment UMETA(DisplayName = "Environment"),
	OnBodyPart UMETA(DisplayName = "Body Part"),

	MAX UMETA(Hidden)
};

/**
 * Stat modifier applied by a status effect
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FStatusEffectStatModifier
{
	GENERATED_BODY()

	/** The stat to modify */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FName StatName;

	/** The modification amount (flat value) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float FlatModifier;

	/** The modification multiplier (percentage) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float MultiplierModifier;

	/** Whether this modifies the max value instead of current */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	bool bModifyMaxValue;

	FStatusEffectStatModifier()
		: StatName(NAME_None)
		, FlatModifier(0.0f)
		, MultiplierModifier(1.0f)
		, bModifyMaxValue(false)
	{
	}
};

/**
 * Data structure for a status effect
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FStatusEffectData
{
	GENERATED_BODY()

	/** Unique identifier for this effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FName EffectID;

	/** Display name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FText DisplayName;

	/** Description of the effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FText Description;

	/** Effect type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	EStatusEffectType EffectType;

	/** Trigger type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	EStatusEffectTrigger TriggerType;

	/** Duration in seconds (-1 for infinite) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float Duration;

	/** Maximum stacks allowed (0 = no stacking) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	int32 MaxStacks;

	/** Priority (higher priority effects override lower ones) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	int32 Priority;

	/** Stat modifiers applied by this effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	TArray<FStatusEffectStatModifier> StatModifiers;

	/** Gameplay tags associated with this effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FGameplayTagContainer EffectTags;

	/** Icon for UI display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	UTexture2D* Icon;

	FStatusEffectData()
		: EffectID(NAME_None)
		, EffectType(EStatusEffectType::Temporary)
		, TriggerType(EStatusEffectTrigger::Manual)
		, Duration(10.0f)
		, MaxStacks(1)
		, Priority(0)
		, Icon(nullptr)
	{
	}
};

/**
 * Active instance of a status effect
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FActiveStatusEffect
{
	GENERATED_BODY()

	/** The effect data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FStatusEffectData EffectData;

	/** Time remaining (for temporary effects) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float TimeRemaining;

	/** Current stack count */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	int32 CurrentStacks;

	/** Time when this effect was applied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	float TimeApplied;

	FActiveStatusEffect()
		: TimeRemaining(0.0f)
		, CurrentStacks(1)
		, TimeApplied(0.0f)
	{
	}

	FActiveStatusEffect(const FStatusEffectData& InData)
		: EffectData(InData)
		, TimeRemaining(InData.Duration)
		, CurrentStacks(1)
		, TimeApplied(0.0f)
	{
	}

	/** Check if effect has expired */
	bool HasExpired() const
	{
		return EffectData.Duration > 0.0f && TimeRemaining <= 0.0f;
	}

	/** Check if can stack more */
	bool CanStack() const
	{
		return EffectData.MaxStacks > 0 && CurrentStacks < EffectData.MaxStacks;
	}
};

/**
 * Data table row for status effect definitions
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FStatusEffectTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect")
	FStatusEffectData EffectData;

	FStatusEffectTableRow()
	{
	}
};
