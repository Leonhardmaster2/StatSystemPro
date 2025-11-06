// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StatSystemProComponent.h"
#include "StatSystemProBlueprintLibrary.generated.h"

/**
 * Blueprint function library for StatSystemPro
 * Provides convenient static functions for accessing the stat system
 */
UCLASS()
class STATSYSTEMPRO_API UStatSystemProBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========== Component Access ==========

	/**
	 * Get the StatSystemPro component from an actor
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro", meta = (DefaultToSelf = "Actor"))
	static UStatSystemProComponent* GetStatSystemComponent(AActor* Actor);

	/**
	 * Get the Stat Layer component from an actor
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro", meta = (DefaultToSelf = "Actor"))
	static UStatComponent* GetStatLayer(AActor* Actor);

	/**
	 * Get the Body Layer component from an actor
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro", meta = (DefaultToSelf = "Actor"))
	static UBodyComponent* GetBodyLayer(AActor* Actor);

	/**
	 * Get the Status Effect Layer component from an actor
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro", meta = (DefaultToSelf = "Actor"))
	static UStatusEffectComponent* GetStatusEffectLayer(AActor* Actor);

	/**
	 * Get the Environment Layer component from an actor
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro", meta = (DefaultToSelf = "Actor"))
	static UEnvironmentComponent* GetEnvironmentLayer(AActor* Actor);

	/**
	 * Get the Progression Layer component from an actor
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro", meta = (DefaultToSelf = "Actor"))
	static UProgressionComponent* GetProgressionLayer(AActor* Actor);

	// ========== Quick Stat Functions ==========

	/**
	 * Get a stat value from an actor
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Stats", meta = (DefaultToSelf = "Actor"))
	static float GetStatValue(AActor* Actor, EStatType StatType);

	/**
	 * Set a stat value on an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Stats", meta = (DefaultToSelf = "Actor"))
	static void SetStatValue(AActor* Actor, EStatType StatType, float Value);

	/**
	 * Change a stat value on an actor (relative change)
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Stats", meta = (DefaultToSelf = "Actor"))
	static void ChangeStatValue(AActor* Actor, EStatType StatType, float Amount);

	/**
	 * Get stat percentage (0-1)
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Stats", meta = (DefaultToSelf = "Actor"))
	static float GetStatPercentage(AActor* Actor, EStatType StatType);

	// ========== Quick Body Functions ==========

	/**
	 * Damage a body part on an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Body", meta = (DefaultToSelf = "Actor"))
	static void DamageBodyPart(AActor* Actor, EBodyPart BodyPart, float Damage);

	/**
	 * Fracture a limb on an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Body", meta = (DefaultToSelf = "Actor"))
	static void FractureLimb(AActor* Actor, EBodyPart BodyPart);

	/**
	 * Heal a body part on an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Body", meta = (DefaultToSelf = "Actor"))
	static void HealBodyPart(AActor* Actor, EBodyPart BodyPart, float Amount);

	/**
	 * Get body part state
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Body", meta = (DefaultToSelf = "Actor"))
	static FBodyPartState GetBodyPartState(AActor* Actor, EBodyPart BodyPart);

	// ========== Quick Status Effect Functions ==========

	/**
	 * Apply a status effect to an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Status Effects", meta = (DefaultToSelf = "Actor"))
	static bool ApplyStatusEffect(AActor* Actor, FName EffectID, int32 Stacks = 1);

	/**
	 * Remove a status effect from an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Status Effects", meta = (DefaultToSelf = "Actor"))
	static bool RemoveStatusEffect(AActor* Actor, FName EffectID);

	/**
	 * Check if actor has a status effect
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Status Effects", meta = (DefaultToSelf = "Actor"))
	static bool HasStatusEffect(AActor* Actor, FName EffectID);

	// ========== Quick Environment Functions ==========

	/**
	 * Set ambient temperature for an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Environment", meta = (DefaultToSelf = "Actor"))
	static void SetAmbientTemperature(AActor* Actor, float Temperature);

	/**
	 * Set shelter state for an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Environment", meta = (DefaultToSelf = "Actor"))
	static void SetShelterState(AActor* Actor, EShelterState ShelterState);

	/**
	 * Set clothing insulation for an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Environment", meta = (DefaultToSelf = "Actor"))
	static void SetClothingInsulation(AActor* Actor, float Insulation);

	// ========== Quick Progression Functions ==========

	/**
	 * Award XP to an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Progression", meta = (DefaultToSelf = "Actor"))
	static void AwardXP(AActor* Actor, int32 Amount, EXPSource Source);

	/**
	 * Get actor's current level
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Progression", meta = (DefaultToSelf = "Actor"))
	static int32 GetLevel(AActor* Actor);

	/**
	 * Unlock a skill for an actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat System Pro|Progression", meta = (DefaultToSelf = "Actor"))
	static bool UnlockSkill(AActor* Actor, FName SkillID);

	// ========== Query Functions ==========

	/**
	 * Check if actor is in critical condition
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query", meta = (DefaultToSelf = "Actor"))
	static bool IsCritical(AActor* Actor);

	/**
	 * Check if actor is dehydrated
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query", meta = (DefaultToSelf = "Actor"))
	static bool IsDehydrated(AActor* Actor);

	/**
	 * Check if actor is starving
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query", meta = (DefaultToSelf = "Actor"))
	static bool IsStarving(AActor* Actor);

	/**
	 * Check if actor is in shock
	 */
	UFUNCTION(BlueprintPure, Category = "Stat System Pro|Query", meta = (DefaultToSelf = "Actor"))
	static bool IsInShock(AActor* Actor);
};
