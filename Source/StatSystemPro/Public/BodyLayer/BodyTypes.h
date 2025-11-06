// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BodyTypes.generated.h"

/**
 * Enum for body part types
 */
UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	Head UMETA(DisplayName = "Head"),
	Torso UMETA(DisplayName = "Torso"),
	LeftArm UMETA(DisplayName = "Left Arm"),
	RightArm UMETA(DisplayName = "Right Arm"),
	LeftLeg UMETA(DisplayName = "Left Leg"),
	RightLeg UMETA(DisplayName = "Right Leg"),

	MAX UMETA(Hidden)
};

/**
 * Enum for burn severity levels
 */
UENUM(BlueprintType)
enum class EBurnLevel : uint8
{
	None UMETA(DisplayName = "None"),
	FirstDegree UMETA(DisplayName = "First Degree"),
	SecondDegree UMETA(DisplayName = "Second Degree"),
	ThirdDegree UMETA(DisplayName = "Third Degree"),

	MAX UMETA(Hidden)
};

/**
 * Struct representing the state of a body part
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FBodyPartState
{
	GENERATED_BODY()

	/** Overall condition of this body part (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part")
	float Condition;

	/** Maximum condition (can be modified by permanent injuries) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part")
	float MaxCondition;

	/** Is this body part fractured? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part")
	bool bFractured;

	/** Bleeding rate (units per second) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part")
	float BleedingRate;

	/** Burn severity level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part")
	EBurnLevel BurnLevel;

	/** Infection rate (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part")
	float InfectionRate;

	/** Pain level (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Part")
	float PainLevel;

	FBodyPartState()
		: Condition(100.0f)
		, MaxCondition(100.0f)
		, bFractured(false)
		, BleedingRate(0.0f)
		, BurnLevel(EBurnLevel::None)
		, InfectionRate(0.0f)
		, PainLevel(0.0f)
	{
	}

	/** Get condition as percentage (0-1) */
	float GetConditionPercentage() const
	{
		return MaxCondition > 0.0f ? Condition / MaxCondition : 0.0f;
	}

	/** Check if body part is critically damaged */
	bool IsCritical() const
	{
		return Condition < 20.0f || bFractured || BurnLevel == EBurnLevel::ThirdDegree;
	}

	/** Check if body part is bleeding */
	bool IsBleeding() const
	{
		return BleedingRate > 0.0f;
	}

	/** Check if body part is infected */
	bool IsInfected() const
	{
		return InfectionRate > 0.0f;
	}
};

/**
 * Multipliers for how body part damage affects overall stats
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FBodyPartEffectMultipliers
{
	GENERATED_BODY()

	/** Movement speed multiplier when legs are damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Effects")
	float MovementSpeedMultiplier;

	/** Stamina drain multiplier when legs are damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Effects")
	float StaminaDrainMultiplier;

	/** Weapon accuracy multiplier when arms are damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Effects")
	float AccuracyMultiplier;

	/** Weapon sway multiplier when arms are damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Effects")
	float WeaponSwayMultiplier;

	/** Max health multiplier when torso is damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Effects")
	float MaxHealthMultiplier;

	/** Sanity drain rate when head is damaged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Effects")
	float SanityDrainRate;

	/** Chance of unconsciousness when head is critically damaged (0-1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Effects")
	float UnconsciousChance;

	FBodyPartEffectMultipliers()
		: MovementSpeedMultiplier(1.0f)
		, StaminaDrainMultiplier(1.0f)
		, AccuracyMultiplier(1.0f)
		, WeaponSwayMultiplier(1.0f)
		, MaxHealthMultiplier(1.0f)
		, SanityDrainRate(0.0f)
		, UnconsciousChance(0.0f)
	{
	}
};
