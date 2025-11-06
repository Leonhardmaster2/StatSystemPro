// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectLayer/StatusEffectTypes.h"
#include "StatusEffectComponent.generated.h"

// Forward declarations
class UStatComponent;

// Delegates for status effect events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectApplied, FName, EffectID, int32, Stacks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectRemoved, FName, EffectID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectExpired, FName, EffectID, float, Duration);

/**
 * Component that manages status effects (buffs/debuffs)
 * Part of the Status Effect Layer
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent))
class STATSYSTEMPRO_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	/** Enable/Disable this layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect System|Layer Control")
	bool bEnabled;

	/** Data table containing effect definitions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Effect System|Configuration")
	UDataTable* StatusEffectTable;

	/** Currently active effects */
	UPROPERTY(BlueprintReadOnly, Category = "Status Effect System")
	TArray<FActiveStatusEffect> ActiveEffects;

	/** Reference to the stat component (optional, for integration) */
	UPROPERTY(BlueprintReadWrite, Category = "Status Effect System|Integration")
	UStatComponent* StatComponent;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Status Effect System|Events")
	FOnStatusEffectApplied OnStatusEffectApplied;

	UPROPERTY(BlueprintAssignable, Category = "Status Effect System|Events")
	FOnStatusEffectRemoved OnStatusEffectRemoved;

	UPROPERTY(BlueprintAssignable, Category = "Status Effect System|Events")
	FOnStatusEffectExpired OnStatusEffectExpired;

	/**
	 * Apply a status effect by ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Status Effect System")
	bool ApplyStatusEffect(FName EffectID, int32 Stacks = 1);

	/**
	 * Apply a status effect from data
	 */
	UFUNCTION(BlueprintCallable, Category = "Status Effect System")
	bool ApplyStatusEffectFromData(const FStatusEffectData& EffectData, int32 Stacks = 1);

	/**
	 * Remove a status effect by ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Status Effect System")
	bool RemoveEffect(FName EffectID);

	/**
	 * Remove all effects with a specific tag
	 */
	UFUNCTION(BlueprintCallable, Category = "Status Effect System")
	int32 RemoveEffectsByTag(FGameplayTag Tag);

	/**
	 * Remove all status effects
	 */
	UFUNCTION(BlueprintCallable, Category = "Status Effect System")
	void ClearAllEffects();

	/**
	 * Check if a specific effect is active
	 */
	UFUNCTION(BlueprintPure, Category = "Status Effect System")
	bool HasEffect(FName EffectID) const;

	/**
	 * Get active effect by ID
	 */
	UFUNCTION(BlueprintPure, Category = "Status Effect System")
	FActiveStatusEffect GetActiveEffect(FName EffectID) const;

	/**
	 * Get all active effects with a specific tag
	 */
	UFUNCTION(BlueprintPure, Category = "Status Effect System")
	TArray<FActiveStatusEffect> GetEffectsByTag(FGameplayTag Tag) const;

	/**
	 * Get stack count for an effect
	 */
	UFUNCTION(BlueprintPure, Category = "Status Effect System")
	int32 GetEffectStacks(FName EffectID) const;

	/**
	 * Get time remaining for an effect
	 */
	UFUNCTION(BlueprintPure, Category = "Status Effect System")
	float GetEffectTimeRemaining(FName EffectID) const;

private:
	/**
	 * Update effect timers and remove expired effects
	 */
	void UpdateEffectTimers(float DeltaTime);

	/**
	 * Apply effect modifiers to stats
	 */
	void ApplyEffectModifiers();

	/**
	 * Find effect data in the data table
	 */
	FStatusEffectData* FindEffectData(FName EffectID);

	/**
	 * Find active effect index by ID
	 */
	int32 FindActiveEffectIndex(FName EffectID) const;
};
