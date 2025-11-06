// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentLayer/EnvironmentTypes.h"
#include "EnvironmentComponent.generated.h"

// Forward declarations
class UStatComponent;
class UStatusEffectComponent;

// Delegates for environment events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnvironmentChanged, const FEnvironmentState&, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHypothermia);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeatstroke);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadiationExposure, float, RadiationLevel);

/**
 * Component that manages environmental effects on the character
 * Part of the Environment Layer
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent))
class STATSYSTEMPRO_API UEnvironmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnvironmentComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	/** Enable/Disable this layer */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment System|Layer Control")
	bool bEnabled;

	/** Current environment state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment System")
	FEnvironmentState CurrentEnvironment;

	/** Configuration for environment effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment System|Configuration")
	FEnvironmentEffectConfig EffectConfig;

	/** Reference to the stat component (optional, for integration) */
	UPROPERTY(BlueprintReadWrite, Category = "Environment System|Integration")
	UStatComponent* StatComponent;

	/** Reference to the status effect component (optional, for integration) */
	UPROPERTY(BlueprintReadWrite, Category = "Environment System|Integration")
	UStatusEffectComponent* StatusEffectComponent;

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Environment System|Events")
	FOnEnvironmentChanged OnEnvironmentChanged;

	UPROPERTY(BlueprintAssignable, Category = "Environment System|Events")
	FOnHypothermia OnHypothermia;

	UPROPERTY(BlueprintAssignable, Category = "Environment System|Events")
	FOnHeatstroke OnHeatstroke;

	UPROPERTY(BlueprintAssignable, Category = "Environment System|Events")
	FOnRadiationExposure OnRadiationExposure;

	/**
	 * Set ambient temperature
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetAmbientTemperature(float Temperature);

	/**
	 * Set wind intensity
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetWindIntensity(float Intensity);

	/**
	 * Set rain level
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetRainLevel(float Level);

	/**
	 * Set snow level
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetSnowLevel(float Level);

	/**
	 * Set shelter state
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetShelterState(EShelterState NewState);

	/**
	 * Set clothing insulation
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetClothingInsulation(float Insulation);

	/**
	 * Set radiation factor
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetRadiationFactor(float Factor);

	/**
	 * Set complete environment state
	 */
	UFUNCTION(BlueprintCallable, Category = "Environment System")
	void SetEnvironmentState(const FEnvironmentState& NewState);

	/**
	 * Get current environment state
	 */
	UFUNCTION(BlueprintPure, Category = "Environment System")
	FEnvironmentState GetEnvironmentState() const { return CurrentEnvironment; }

	/**
	 * Get current exposure factor
	 */
	UFUNCTION(BlueprintPure, Category = "Environment System")
	float GetExposureFactor() const;

	/**
	 * Check if player is hypothermic
	 */
	UFUNCTION(BlueprintPure, Category = "Environment System")
	bool IsHypothermic() const;

	/**
	 * Check if player has heatstroke
	 */
	UFUNCTION(BlueprintPure, Category = "Environment System")
	bool HasHeatstroke() const;

private:
	/**
	 * Update body temperature based on environment
	 */
	void UpdateBodyTemperature(float DeltaTime);

	/**
	 * Update wetness based on weather
	 */
	void UpdateWetness(float DeltaTime);

	/**
	 * Apply radiation damage
	 */
	void UpdateRadiation(float DeltaTime);

	/**
	 * Check for and apply temperature-related status effects
	 */
	void CheckTemperatureEffects();

	/** Track if hypothermia warning was already triggered */
	bool bHypothermiaTriggered;

	/** Track if heatstroke warning was already triggered */
	bool bHeatstrokeTriggered;
};
