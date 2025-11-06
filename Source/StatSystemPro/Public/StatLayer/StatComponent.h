// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatLayer/StatTypes.h"
#include "StatComponent.generated.h"

// Delegates for stat events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatChanged, EStatType, StatType, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatMaxChanged, EStatType, StatType, float, NewMaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatReachedZero, EStatType, StatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatReachedMax, EStatType, StatType);

/**
 * Component that manages all stat values and their updates
 * Part of the Stat Layer
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent))
class STATSYSTEMPRO_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	/** Enable/Disable this layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat System|Layer Control")
	bool bEnabled;

	/** Data table for configuring default stat values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat System|Configuration")
	UDataTable* StatConfigTable;

	/** All stat values */
	UPROPERTY(BlueprintReadOnly, Category = "Stat System|Stats")
	TMap<EStatType, FStatValue> Stats;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events")
	FOnStatChanged OnStatChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events")
	FOnStatMaxChanged OnStatMaxChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events")
	FOnStatReachedZero OnStatReachedZero;

	UPROPERTY(BlueprintAssignable, Category = "Stat System|Events")
	FOnStatReachedMax OnStatReachedMax;

	/**
	 * Initialize stats from config or defaults
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System")
	void InitializeStats();

	/**
	 * Apply a stat change with reason tracking
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System")
	void ApplyStatChange(EStatType StatType, float Amount, FName Source, FGameplayTag ReasonTag);

	/**
	 * Set a stat to a specific value
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System")
	void SetStatValue(EStatType StatType, float NewValue);

	/**
	 * Set the maximum value for a stat
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System")
	void SetStatMaxValue(EStatType StatType, float NewMaxValue);

	/**
	 * Get current stat value
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System")
	float GetStatValue(EStatType StatType) const;

	/**
	 * Get stat max value
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System")
	float GetStatMaxValue(EStatType StatType) const;

	/**
	 * Get stat percentage (0-1)
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System")
	float GetStatPercentage(EStatType StatType) const;

	/**
	 * Set regeneration rate for a stat
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System")
	void SetStatRegenerationRate(EStatType StatType, float Rate);

	/**
	 * Get stat struct
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System")
	FStatValue GetStat(EStatType StatType) const;

	/**
	 * Check if stat exists
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System")
	bool HasStat(EStatType StatType) const;

private:
	/**
	 * Update stat regeneration/decay
	 */
	void UpdateStatRegeneration(float DeltaTime);

	/**
	 * Broadcast appropriate events for stat changes
	 */
	void BroadcastStatEvents(EStatType StatType, float OldValue, float NewValue);
};
