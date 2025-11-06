// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatLayer/StatComponent.h"
#include "BodyLayer/BodyComponent.h"
#include "StatusEffectLayer/StatusEffectComponent.h"
#include "EnvironmentLayer/EnvironmentComponent.h"
#include "ProgressionLayer/ProgressionComponent.h"
#include "StatSystemProComponent.generated.h"

/**
 * Main component that manages and integrates all stat system layers
 * This is the primary interface for the StatSystemPro plugin
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent))
class STATSYSTEMPRO_API UStatSystemProComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatSystemProComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ========== Layer Components ==========

	/** Stat Layer - Manages all numeric values */
	UPROPERTY(BlueprintReadOnly, Category = "Stat System Pro|Layers")
	UStatComponent* StatLayer;

	/** Body Layer - Manages body parts and physical conditions */
	UPROPERTY(BlueprintReadOnly, Category = "Stat System Pro|Layers")
	UBodyComponent* BodyLayer;

	/** Status Effect Layer - Manages buffs and debuffs */
	UPROPERTY(BlueprintReadOnly, Category = "Stat System Pro|Layers")
	UStatusEffectComponent* StatusEffectLayer;

	/** Environment Layer - Manages environmental effects */
	UPROPERTY(BlueprintReadOnly, Category = "Stat System Pro|Layers")
	UEnvironmentComponent* EnvironmentLayer;

	/** Progression Layer - Manages XP, levels, and skills */
	UPROPERTY(BlueprintReadOnly, Category = "Stat System Pro|Layers")
	UProgressionComponent* ProgressionLayer;

	// ========== Layer Control ==========

	/**
	 * Initialize all layers
	 * This will automatically find or create all layer components
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro")
	void InitializeLayers();

	/**
	 * Enable or disable a specific layer
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro")
	void SetLayerEnabled(FName LayerName, bool bEnabled);

	/**
	 * Check if a layer is enabled
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro")
	bool IsLayerEnabled(FName LayerName) const;

	// ========== Query Functions ==========

	/**
	 * Check if character is in critical condition
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query")
	bool IsCritical() const;

	/**
	 * Check if character is hypothermic
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query")
	bool IsHypothermic() const;

	/**
	 * Check if character is dehydrated
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query")
	bool IsDehydrated() const;

	/**
	 * Check if character is starving
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query")
	bool IsStarving() const;

	/**
	 * Check if character is in shock (from blood loss or pain)
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query")
	bool IsInShock() const;

	/**
	 * Get overall health status as percentage (0-1)
	 * Factors in health, body condition, and critical injuries
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query")
	float GetOverallHealthStatus() const;

	// ========== Helper Functions ==========

	/**
	 * Apply damage to a specific body part
	 * This is a convenience function that automatically routes to the body layer
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Helpers")
	void ApplyDamageToBodyPart(EBodyPart BodyPart, float Damage);

	/**
	 * Apply general damage (distributed across body)
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Helpers")
	void ApplyGeneralDamage(float Damage);

	/**
	 * Heal character
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Helpers")
	void Heal(float Amount);

	/**
	 * Consume food or water
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Helpers")
	void Consume(float HungerRestore, float ThirstRestore, float EnergyRestore);

	/**
	 * Apply a status effect by ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Helpers")
	void ApplyEffect(FName EffectID, int32 Stacks = 1);

	/**
	 * Remove a status effect by ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Helpers")
	void RemoveEffect(FName EffectID);

	/**
	 * Award XP to the character
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Helpers")
	void AwardExperience(int32 Amount, EXPSource Source);

	/**
	 * Get debug information about all layers
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Debug")
	FString GetDebugInfo() const;

private:
	/**
	 * Create or find layer components
	 */
	void EnsureLayerComponents();

	/**
	 * Link layer components together
	 */
	void LinkLayerComponents();
};
