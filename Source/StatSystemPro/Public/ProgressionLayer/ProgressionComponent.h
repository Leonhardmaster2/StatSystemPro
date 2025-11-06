// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProgressionLayer/ProgressionTypes.h"
#include "ProgressionComponent.generated.h"

// Forward declarations
class UStatComponent;
class UStatusEffectComponent;

// Delegates for progression events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPGained, int32, Amount, EXPSource, Source);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillUnlocked, FName, SkillID, int32, Level);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributePointsChanged, int32, NewAmount, int32, Change);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillPointsChanged, int32, NewAmount, int32, Change);

/**
 * Component that manages character progression, skills, and leveling
 * Part of the Progression Layer
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent))
class STATSYSTEMPRO_API UProgressionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProgressionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	/** Enable/Disable this layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System|Layer Control")
	bool bEnabled;

	/** Current progression data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System")
	FProgressionData ProgressionData;

	/** Unlocked skills */
	UPROPERTY(BlueprintReadOnly, Category = "Progression System")
	TArray<FActiveSkill> UnlockedSkills;

	/** Data table containing skill definitions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System|Configuration")
	UDataTable* SkillTable;

	/** XP curve for leveling (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System|Configuration")
	UCurveFloat* XPCurve;

	/** Base XP required per level (if no curve is provided) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System|Configuration")
	int32 BaseXPPerLevel;

	/** XP multiplier per level (if no curve is provided) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System|Configuration")
	float XPMultiplierPerLevel;

	/** Grant XP over time for survival */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System|Configuration")
	bool bGrantSurvivalXP;

	/** XP per minute survived */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression System|Configuration")
	float XPPerMinuteSurvived;

	/** Reference to the stat component (optional, for integration) */
	UPROPERTY(BlueprintReadWrite, Category = "Progression System|Integration")
	UStatComponent* StatComponent;

	/** Reference to the status effect component (optional, for integration) */
	UPROPERTY(BlueprintReadWrite, Category = "Progression System|Integration")
	UStatusEffectComponent* StatusEffectComponent;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Progression System|Events")
	FOnXPGained OnXPGained;

	UPROPERTY(BlueprintAssignable, Category = "Progression System|Events")
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "Progression System|Events")
	FOnSkillUnlocked OnSkillUnlocked;

	UPROPERTY(BlueprintAssignable, Category = "Progression System|Events")
	FOnAttributePointsChanged OnAttributePointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Progression System|Events")
	FOnSkillPointsChanged OnSkillPointsChanged;

	/**
	 * Award experience points
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression System")
	void AwardXP(int32 Amount, EXPSource Source);

	/**
	 * Unlock a skill by ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression System")
	bool UnlockSkill(FName SkillID);

	/**
	 * Level up a skill
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression System")
	bool LevelUpSkill(FName SkillID);

	/**
	 * Check if a skill is unlocked
	 */
	UFUNCTION(BlueprintPure, Category = "Progression System")
	bool IsSkillUnlocked(FName SkillID) const;

	/**
	 * Get skill level
	 */
	UFUNCTION(BlueprintPure, Category = "Progression System")
	int32 GetSkillLevel(FName SkillID) const;

	/**
	 * Check if can unlock skill (has requirements)
	 */
	UFUNCTION(BlueprintPure, Category = "Progression System")
	bool CanUnlockSkill(FName SkillID) const;

	/**
	 * Spend attribute points
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression System")
	bool SpendAttributePoints(int32 Amount);

	/**
	 * Spend skill points
	 */
	UFUNCTION(BlueprintCallable, Category = "Progression System")
	bool SpendSkillPoints(int32 Amount);

	/**
	 * Get current level
	 */
	UFUNCTION(BlueprintPure, Category = "Progression System")
	int32 GetCurrentLevel() const { return ProgressionData.CurrentLevel; }

	/**
	 * Get current XP
	 */
	UFUNCTION(BlueprintPure, Category = "Progression System")
	int32 GetCurrentXP() const { return ProgressionData.CurrentXP; }

	/**
	 * Get XP for next level
	 */
	UFUNCTION(BlueprintPure, Category = "Progression System")
	int32 GetXPForNextLevel() const { return ProgressionData.XPForNextLevel; }

	/**
	 * Get level progress (0-1)
	 */
	UFUNCTION(BlueprintPure, Category = "Progression System")
	float GetLevelProgress() const { return ProgressionData.GetLevelProgress(); }

private:
	/**
	 * Check for level up
	 */
	void CheckLevelUp();

	/**
	 * Calculate XP required for a specific level
	 */
	int32 CalculateXPForLevel(int32 Level) const;

	/**
	 * Find skill data by ID
	 */
	FSkillData* FindSkillData(FName SkillID);

	/**
	 * Find active skill index
	 */
	int32 FindActiveSkillIndex(FName SkillID) const;

	/**
	 * Apply skill effects to stats
	 */
	void ApplySkillEffects();

	/** Time accumulator for survival XP */
	float SurvivalXPTimer;
};
