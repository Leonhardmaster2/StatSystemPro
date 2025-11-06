// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BodyLayer/BodyTypes.h"
#include "BodyComponent.generated.h"

// Forward declarations
class UStatComponent;

// Delegates for body events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartDamaged, EBodyPart, BodyPart, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBodyPartFractured, EBodyPart, BodyPart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartBleeding, EBodyPart, BodyPart, float, BleedRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartBurned, EBodyPart, BodyPart, EBurnLevel, BurnLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBodyPartInfected, EBodyPart, BodyPart, float, InfectionRate);

/**
 * Component that manages body parts and their states
 * Part of the Body Layer
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent))
class STATSYSTEMPRO_API UBodyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBodyComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	/** Enable/Disable this layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body System|Layer Control")
	bool bEnabled;

	/** All body parts and their states */
	UPROPERTY(BlueprintReadOnly, Category = "Body System")
	TMap<EBodyPart, FBodyPartState> BodyParts;

	/** Reference to the stat component (optional, for integration) */
	UPROPERTY(BlueprintReadWrite, Category = "Body System|Integration")
	UStatComponent* StatComponent;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Body System|Events")
	FOnBodyPartDamaged OnBodyPartDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Body System|Events")
	FOnBodyPartFractured OnBodyPartFractured;

	UPROPERTY(BlueprintAssignable, Category = "Body System|Events")
	FOnBodyPartBleeding OnBodyPartBleeding;

	UPROPERTY(BlueprintAssignable, Category = "Body System|Events")
	FOnBodyPartBurned OnBodyPartBurned;

	UPROPERTY(BlueprintAssignable, Category = "Body System|Events")
	FOnBodyPartInfected OnBodyPartInfected;

	/**
	 * Initialize body parts
	 */
	UFUNCTION(BlueprintCallable, Category = "Body System")
	void InitializeBodyParts();

	/**
	 * Apply damage to a specific body part
	 */
	UFUNCTION(BlueprintCallable, Category = "Body System")
	void DamageBodyPart(EBodyPart BodyPart, float Damage);

	/**
	 * Fracture a body part
	 */
	UFUNCTION(BlueprintCallable, Category = "Body System")
	void FractureLimb(EBodyPart BodyPart);

	/**
	 * Set bleeding rate for a body part
	 */
	UFUNCTION(BlueprintCallable, Category = "Body System")
	void SetBleedingRate(EBodyPart BodyPart, float Rate);

	/**
	 * Set burn level for a body part
	 */
	UFUNCTION(BlueprintCallable, Category = "Body System")
	void SetBurnLevel(EBodyPart BodyPart, EBurnLevel BurnLevel);

	/**
	 * Apply infection to a body part
	 */
	UFUNCTION(BlueprintCallable, Category = "Body System")
	void ApplyInfection(EBodyPart BodyPart, float InfectionAmount);

	/**
	 * Heal a body part
	 */
	UFUNCTION(BlueprintCallable, Category = "Body System")
	void HealLimb(EBodyPart BodyPart, float HealAmount);

	/**
	 * Get body part state
	 */
	UFUNCTION(BlueprintPure, Category = "Body System")
	FBodyPartState GetBodyPartState(EBodyPart BodyPart) const;

	/**
	 * Get total body condition (average of all parts)
	 */
	UFUNCTION(BlueprintPure, Category = "Body System")
	float GetTotalBodyCondition() const;

	/**
	 * Get total bleeding rate from all body parts
	 */
	UFUNCTION(BlueprintPure, Category = "Body System")
	float GetTotalBleedingRate() const;

	/**
	 * Get total pain level from all body parts
	 */
	UFUNCTION(BlueprintPure, Category = "Body System")
	float GetTotalPainLevel() const;

	/**
	 * Calculate effect multipliers based on body state
	 */
	UFUNCTION(BlueprintPure, Category = "Body System")
	FBodyPartEffectMultipliers CalculateEffectMultipliers() const;

	/**
	 * Check if any critical body part is damaged
	 */
	UFUNCTION(BlueprintPure, Category = "Body System")
	bool HasCriticalInjury() const;

private:
	/**
	 * Update bleeding effects
	 */
	void UpdateBleeding(float DeltaTime);

	/**
	 * Update infection progression
	 */
	void UpdateInfection(float DeltaTime);

	/**
	 * Apply body state effects to stats
	 */
	void ApplyBodyEffectsToStats();
};
