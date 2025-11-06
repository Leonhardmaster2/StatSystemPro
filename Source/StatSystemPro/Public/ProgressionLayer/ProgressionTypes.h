// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "ProgressionTypes.generated.h"

/**
 * Enum for skill categories
 */
UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
	Combat UMETA(DisplayName = "Combat"),
	Survival UMETA(DisplayName = "Survival"),
	Crafting UMETA(DisplayName = "Crafting"),
	Social UMETA(DisplayName = "Social"),
	Exploration UMETA(DisplayName = "Exploration"),

	MAX UMETA(Hidden)
};

/**
 * Struct representing a skill/perk
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSkillData
{
	GENERATED_BODY()

	/** Unique identifier for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FName SkillID;

	/** Display name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FText DisplayName;

	/** Description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FText Description;

	/** Skill category */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillCategory Category;

	/** Maximum level for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 MaxLevel;

	/** Cost in skill points to unlock */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 SkillPointCost;

	/** Required level to unlock */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 RequiredLevel;

	/** Prerequisites (other skills that must be unlocked first) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FName> Prerequisites;

	/** Gameplay tags associated with this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FGameplayTagContainer SkillTags;

	/** Icon for UI display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	UTexture2D* Icon;

	FSkillData()
		: SkillID(NAME_None)
		, Category(ESkillCategory::Survival)
		, MaxLevel(1)
		, SkillPointCost(1)
		, RequiredLevel(1)
		, Icon(nullptr)
	{
	}
};

/**
 * Active skill instance
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FActiveSkill
{
	GENERATED_BODY()

	/** The skill data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillData SkillData;

	/** Current level of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 CurrentLevel;

	/** Time when this skill was unlocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float TimeUnlocked;

	FActiveSkill()
		: CurrentLevel(0)
		, TimeUnlocked(0.0f)
	{
	}

	FActiveSkill(const FSkillData& InData)
		: SkillData(InData)
		, CurrentLevel(1)
		, TimeUnlocked(0.0f)
	{
	}

	/** Check if skill is at max level */
	bool IsMaxLevel() const
	{
		return CurrentLevel >= SkillData.MaxLevel;
	}
};

/**
 * Player progression data
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FProgressionData
{
	GENERATED_BODY()

	/** Current level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 CurrentLevel;

	/** Current experience points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 CurrentXP;

	/** XP required for next level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 XPForNextLevel;

	/** Available attribute points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 AttributePoints;

	/** Available skill points */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	int32 SkillPoints;

	/** Total time survived (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
	float TotalTimeSurvived;

	FProgressionData()
		: CurrentLevel(1)
		, CurrentXP(0)
		, XPForNextLevel(100)
		, AttributePoints(0)
		, SkillPoints(0)
		, TotalTimeSurvived(0.0f)
	{
	}

	/** Get level progress as percentage (0-1) */
	float GetLevelProgress() const
	{
		return XPForNextLevel > 0 ? (float)CurrentXP / (float)XPForNextLevel : 0.0f;
	}
};

/**
 * XP reward source
 */
UENUM(BlueprintType)
enum class EXPSource : uint8
{
	Combat UMETA(DisplayName = "Combat"),
	Crafting UMETA(DisplayName = "Crafting"),
	Exploration UMETA(DisplayName = "Exploration"),
	Survival UMETA(DisplayName = "Survival"),
	Quest UMETA(DisplayName = "Quest"),
	Other UMETA(DisplayName = "Other"),

	MAX UMETA(Hidden)
};

/**
 * Data table row for skill definitions
 */
USTRUCT(BlueprintType)
struct STATSYSTEMPRO_API FSkillTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillData SkillData;

	FSkillTableRow()
	{
	}
};
