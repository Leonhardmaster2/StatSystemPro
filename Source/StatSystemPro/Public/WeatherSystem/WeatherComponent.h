// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeatherSystem/WeatherTypes.h"
#include "WeatherComponent.generated.h"

// Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeatherChanged, EWeatherType, OldWeather, EWeatherType, NewWeather);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFreezingStageChanged, EFreezingStage, OldStage, EFreezingStage, NewStage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOverheatingStageChanged, EOverheatingStage, OldStage, EOverheatingStage, NewStage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClothingEquipped, EClothingSlot, Slot, FClothingItem, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClothingRemoved, EClothingSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartedFreezing, float, BodyTemperature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartedOverheating, float, BodyTemperature);

/**
 * ============================================================================
 * WEATHER COMPONENT - Advanced Weather & Temperature System
 * ============================================================================
 *
 * Manages weather conditions, clothing system, and temperature effects
 *
 * FEATURES:
 * - 13 weather types (clear, rain, snow, blizzard, heatwave, etc.)
 * - 6 clothing slots with insulation, wetness, durability
 * - Freezing stages (Chilled → Cold → Freezing → Hypothermia → Critical)
 * - Overheating stages (Warm → Hot → Overheating → Heatstroke → Critical)
 * - Wind chill calculation
 * - Wetness affects insulation
 * - Weather presets (easy configuration)
 * - Multiplayer synchronized
 *
 * INTEGRATES WITH:
 * - Stat Component (BodyTemperature stat)
 * - Status Effect Component (applies freezing/overheating effects)
 * - Environment Component (temperature, wind, rain)
 */
UCLASS(ClassGroup=(StatSystemPro), meta=(BlueprintSpawnableComponent, DisplayName="Weather Component (Advanced)"))
class STATSYSTEMPRO_API UWeatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeatherComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	// ========== CONFIGURATION ==========

	/** Enable/Disable weather system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Weather System|Configuration", meta=(
		DisplayName = "Enable Weather System",
		Tooltip = "Turn ON to enable weather and temperature effects"
	))
	bool bEnabled;

	/** Current weather type */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentWeather, Category = "Weather System|State", meta=(
		DisplayName = "Current Weather",
		Tooltip = "Current active weather condition"
	))
	EWeatherType CurrentWeather;

	/** Current ambient temperature (Celsius) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weather System|State")
	float AmbientTemperature;

	/** Current wind speed (m/s) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weather System|State")
	float WindSpeed;

	/** Current precipitation intensity */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weather System|State")
	float PrecipitationIntensity;

	/** Current shelter level (0-100) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Weather System|State", meta=(
		DisplayName = "Shelter Level",
		Tooltip = "0 = Outside, 100 = Fully sheltered"
	))
	float ShelterLevel;

	/** Equipped clothing items */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ClothingSlots, Category = "Weather System|Clothing")
	TMap<EClothingSlot, FClothingItem> ClothingSlots;

	/** Current freezing stage */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FreezingStage, Category = "Weather System|Status")
	EFreezingStage CurrentFreezingStage;

	/** Current overheating stage */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_OverheatingStage, Category = "Weather System|Status")
	EOverheatingStage CurrentOverheatingStage;

	// ========== EVENTS ==========

	UPROPERTY(BlueprintAssignable, Category = "Weather System|Events")
	FOnWeatherChanged OnWeatherChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weather System|Events")
	FOnFreezingStageChanged OnFreezingStageChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weather System|Events")
	FOnOverheatingStageChanged OnOverheatingStageChanged;

	UPROPERTY(BlueprintAssignable, Category = "Weather System|Events")
	FOnClothingEquipped OnClothingEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Weather System|Events")
	FOnClothingRemoved OnClothingRemoved;

	UPROPERTY(BlueprintAssignable, Category = "Weather System|Events")
	FOnStartedFreezing OnStartedFreezing;

	UPROPERTY(BlueprintAssignable, Category = "Weather System|Events")
	FOnStartedOverheating OnStartedOverheating;

	// ========== WEATHER CONTROL ==========

	/**
	 * Set weather to specific type
	 * BLUEPRINT: Change weather condition
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Control", meta=(
		DisplayName = "Set Weather Type",
		Tooltip = "Change to a specific weather condition"
	))
	void SetWeatherType(EWeatherType NewWeather);

	/**
	 * Apply a weather preset
	 * BLUEPRINT: Use pre-configured weather settings
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Control", meta=(
		DisplayName = "Apply Weather Preset",
		Tooltip = "Apply a preset weather configuration (blizzard, heatwave, etc.)"
	))
	void ApplyWeatherPreset(const FWeatherPreset& Preset);

	/**
	 * Set temperature directly
	 * BLUEPRINT: Override temperature
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Control", meta=(
		DisplayName = "Set Temperature",
		Tooltip = "Set ambient temperature in Celsius"
	))
	void SetTemperature(float Temperature);

	/**
	 * Set wind speed
	 * BLUEPRINT: Control wind strength
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Control", meta=(
		DisplayName = "Set Wind Speed",
		Tooltip = "Set wind speed in meters per second"
	))
	void SetWindSpeed(float Speed);

	// ========== CLOTHING SYSTEM ==========

	/**
	 * Equip clothing item
	 * BLUEPRINT: Put on clothing
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Clothing", meta=(
		DisplayName = "Equip Clothing",
		Tooltip = "Equip a clothing item to a slot"
	))
	void EquipClothing(EClothingSlot Slot, const FClothingItem& Item);

	/**
	 * Remove clothing item
	 * BLUEPRINT: Take off clothing
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Clothing", meta=(
		DisplayName = "Remove Clothing",
		Tooltip = "Remove clothing from a slot"
	))
	void RemoveClothing(EClothingSlot Slot);

	/**
	 * Get clothing in slot
	 * BLUEPRINT: Check what's equipped
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Clothing", meta=(
		DisplayName = "Get Clothing in Slot",
		Tooltip = "Get the clothing item equipped in a slot"
	))
	FClothingItem GetClothingInSlot(EClothingSlot Slot, bool& bHasClothing) const;

	/**
	 * Add wetness to all clothing
	 * BLUEPRINT: Clothing gets wet from rain
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Clothing", meta=(
		DisplayName = "Add Clothing Wetness",
		Tooltip = "Add wetness to all equipped clothing (from rain, swimming, etc.)"
	))
	void AddClothingWetness(float Amount);

	/**
	 * Dry clothing over time
	 * BLUEPRINT: Clothing dries near fire, indoors, etc.
	 */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Clothing", meta=(
		DisplayName = "Dry Clothing",
		Tooltip = "Remove wetness from clothing (near fire, indoors, etc.)"
	))
	void DryClothing(float Amount);

	/**
	 * Get total clothing insulation
	 * BLUEPRINT: Calculate total cold protection
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Clothing", meta=(
		DisplayName = "Get Total Insulation",
		Tooltip = "Get combined insulation from all clothing"
	))
	float GetTotalColdInsulation() const;

	/**
	 * Get total heat protection
	 * BLUEPRINT: Calculate total heat protection
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Clothing", meta=(
		DisplayName = "Get Total Heat Protection",
		Tooltip = "Get combined heat protection from all clothing"
	))
	float GetTotalHeatProtection() const;

	/**
	 * Get total clothing weight
	 * BLUEPRINT: How heavy is your clothing?
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Clothing", meta=(
		DisplayName = "Get Total Clothing Weight",
		Tooltip = "Get combined weight of all equipped clothing"
	))
	float GetTotalClothingWeight() const;

	// ========== TEMPERATURE CALCULATIONS ==========

	/**
	 * Calculate effective temperature
	 * BLUEPRINT: Get "feels like" temperature
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Temperature", meta=(
		DisplayName = "Calculate Effective Temperature",
		Tooltip = "Calculate how cold/hot it actually feels (considers wind, wetness, clothing)"
	))
	FTemperatureResult CalculateEffectiveTemperature() const;

	/**
	 * Get wind chill
	 * BLUEPRINT: How much colder does wind make it feel?
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Temperature", meta=(
		DisplayName = "Calculate Wind Chill",
		Tooltip = "Calculate wind chill effect"
	))
	float CalculateWindChill() const;

	/**
	 * Is freezing?
	 * BLUEPRINT: Quick check for cold danger
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Temperature", meta=(
		DisplayName = "Is Freezing?",
		Tooltip = "Check if character is in freezing conditions"
	))
	bool IsFreezing() const;

	/**
	 * Is overheating?
	 * BLUEPRINT: Quick check for heat danger
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Temperature", meta=(
		DisplayName = "Is Overheating?",
		Tooltip = "Check if character is overheating"
	))
	bool IsOverheating() const;

	/**
	 * Get freezing stage
	 * BLUEPRINT: How severe is the freezing?
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Temperature", meta=(
		DisplayName = "Get Freezing Stage",
		Tooltip = "Get current freezing severity (None → Chilled → Cold → Freezing → Hypothermia → Critical)"
	))
	EFreezingStage GetFreezingStage() const { return CurrentFreezingStage; }

	/**
	 * Get overheating stage
	 * BLUEPRINT: How severe is the overheating?
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Temperature", meta=(
		DisplayName = "Get Overheating Stage",
		Tooltip = "Get current overheating severity (None → Warm → Hot → Overheating → Heatstroke → Critical)"
	))
	EOverheatingStage GetOverheatingStage() const { return CurrentOverheatingStage; }

	// ========== WEATHER PRESETS ==========

	/**
	 * Create blizzard preset
	 * BLUEPRINT: Get blizzard configuration
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Presets", meta=(
		DisplayName = "Get Blizzard Preset",
		Tooltip = "Get blizzard weather preset"
	))
	static FWeatherPreset GetBlizzardPreset();

	/**
	 * Create heatwave preset
	 * BLUEPRINT: Get heatwave configuration
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Presets", meta=(
		DisplayName = "Get Heatwave Preset",
		Tooltip = "Get heatwave weather preset"
	))
	static FWeatherPreset GetHeatwavePreset();

	/**
	 * Create thunderstorm preset
	 * BLUEPRINT: Get thunderstorm configuration
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Presets", meta=(
		DisplayName = "Get Thunderstorm Preset",
		Tooltip = "Get thunderstorm weather preset"
	))
	static FWeatherPreset GetThunderstormPreset();

	/**
	 * Create clear weather preset
	 * BLUEPRINT: Get clear weather configuration
	 */
	UFUNCTION(BlueprintPure, Category = "Weather System|Presets", meta=(
		DisplayName = "Get Clear Weather Preset",
		Tooltip = "Get clear/sunny weather preset"
	))
	static FWeatherPreset GetClearWeatherPreset();

private:
	/** Previous weather for change detection */
	EWeatherType PreviousWeather;

	/** Previous freezing stage */
	EFreezingStage PreviousFreezingStage;

	/** Previous overheating stage */
	EOverheatingStage PreviousOverheatingStage;

	/** Update temperature effects */
	void UpdateTemperatureEffects(float DeltaTime);

	/** Update clothing wetness */
	void UpdateClothingWetness(float DeltaTime);

	/** Update freezing/overheating stages */
	void UpdateTemperatureStages();

	/** Apply temperature damage/effects */
	void ApplyTemperatureEffects(float DeltaTime);

	/** Replication callbacks */
	UFUNCTION()
	void OnRep_CurrentWeather();

	UFUNCTION()
	void OnRep_ClothingSlots();

	UFUNCTION()
	void OnRep_FreezingStage();

	UFUNCTION()
	void OnRep_OverheatingStage();
};
