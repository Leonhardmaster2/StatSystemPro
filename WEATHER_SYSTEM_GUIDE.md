# Weather System Guide - StatSystemPro

## Table of Contents
1. [Overview](#overview)
2. [Weather Types](#weather-types)
3. [Clothing System](#clothing-system)
4. [Temperature Mechanics](#temperature-mechanics)
5. [Freezing Stages](#freezing-stages)
6. [Overheating Stages](#overheating-stages)
7. [Weather Presets](#weather-presets)
8. [Blueprint Usage](#blueprint-usage)
9. [Integration with Other Systems](#integration-with-other-systems)
10. [Advanced Use Cases](#advanced-use-cases)

---

## Overview

The Weather System is a comprehensive environmental simulation that affects character survival through:

- **13 weather types** ranging from clear skies to deadly blizzards
- **6-slot clothing system** with realistic insulation and wetness mechanics
- **Temperature calculations** that consider wind chill, wetness, clothing, and shelter
- **Progressive damage stages** for both freezing (hypothermia) and overheating (heatstroke)
- **Full Blueprint integration** with events and easy-to-use functions
- **Multiplayer replication** for consistent weather across clients

**Key Features:**
- Clothing gets wet in rain and snow (reduces insulation effectiveness by up to 70%)
- Wind chill makes cold weather feel even colder
- Shelter provides protection from harsh weather
- Weather presets for instant configuration (Blizzard, Heatwave, etc.)
- Smooth transitions between weather states

---

## Weather Types

### Available Weather Types (EWeatherType)

#### Clear Weather
- **Clear**: Perfect weather, no precipitation
- **Cloudy**: Some cloud cover, no precipitation
- **Overcast**: Heavy cloud cover, reduced visibility

#### Rain Weather
- **LightRain**: Light drizzle, minimal wetness
- **Rain**: Moderate rainfall
- **HeavyRain**: Heavy downpour, rapid wetness accumulation
- **Thunderstorm**: Intense rain with strong winds

#### Snow Weather
- **LightSnow**: Light snowfall
- **Snow**: Moderate snowfall
- **HeavySnow**: Heavy blizzard conditions, severe visibility reduction

#### Special Weather
- **Fog**: Reduced visibility, neutral temperature
- **Sandstorm**: High winds, extreme heat, reduced visibility
- **Heatwave**: Extreme heat, clear skies, low wind

### Setting Weather

**C++:**
```cpp
UWeatherComponent* Weather = Character->FindComponentByClass<UWeatherComponent>();
Weather->SetWeatherType(EWeatherType::Blizzard);
```

**Blueprint:**
Call "Set Weather Type" node with dropdown selection.

---

## Clothing System

### Clothing Slots (6 Total)

1. **Head** - Helmets, hats, hoods
2. **Torso** - Jackets, shirts, vests
3. **Legs** - Pants, shorts
4. **Feet** - Boots, shoes
5. **Hands** - Gloves, mittens
6. **Back** - Backpacks, cloaks (future use)

### Clothing Properties

Each clothing item has the following properties:

#### **Cold Insulation** (0-100)
How well the item protects against cold temperatures.
- 0 = No protection
- 50 = Moderate protection
- 100 = Maximum protection

**Examples:**
- T-Shirt: 5 cold insulation
- Winter Jacket: 80 cold insulation
- Arctic Parka: 95 cold insulation

#### **Heat Insulation** (0-100)
How well the item protects against heat.
- 0 = No protection (breathable)
- 50 = Moderate heat retention
- 100 = Traps heat (dangerous in hot weather)

**Note:** High heat insulation is BAD in hot weather!

**Examples:**
- Breathable Shirt: 10 heat insulation (good for hot weather)
- Winter Jacket: 85 heat insulation (dangerous in heat)
- Desert Robe: 20 heat insulation (good for hot climates)

#### **Water Resistance** (0-100)
How well the item resists getting wet.
- 0 = Absorbs water instantly
- 50 = Moderate resistance
- 100 = Completely waterproof

**Examples:**
- Cotton Shirt: 10 water resistance
- Leather Jacket: 60 water resistance
- Rain Coat: 95 water resistance

#### **Wind Resistance** (0-100)
How well the item blocks wind.
- 0 = No wind protection
- 50 = Moderate wind blocking
- 100 = Complete wind block

**Examples:**
- T-Shirt: 5 wind resistance
- Windbreaker: 75 wind resistance
- Winter Parka: 90 wind resistance

#### **Current Wetness** (0-100)
How wet the clothing currently is. **CRITICAL MECHANIC:**
- Wet clothing loses insulation effectiveness (up to 70% penalty)
- Wetness increases in rain/snow
- Wetness decreases when sheltered or near heat sources

**Formula:**
```
Effective Insulation = Base Insulation × (1 - Wetness × 0.7) × (Durability / 100)
```

**Example:**
- Winter Jacket: 80 cold insulation
- After rain: 50% wetness
- Effective insulation: 80 × (1 - 0.5 × 0.7) = 80 × 0.65 = **52 insulation** (35% loss!)

#### **Durability** (0-100)
Condition of the item. Lower durability = lower effectiveness.

#### **Weight** (0+)
Weight in kg. Affects encumbrance (future feature).

### Equipping Clothing

**C++:**
```cpp
FClothingItem WinterJacket;
WinterJacket.DisplayName = FText::FromString("Winter Jacket");
WinterJacket.Slot = EClothingSlot::Torso;
WinterJacket.ColdInsulation = 80.0f;
WinterJacket.HeatInsulation = 85.0f;
WinterJacket.WaterResistance = 40.0f;
WinterJacket.WindResistance = 70.0f;
WinterJacket.CurrentWetness = 0.0f;
WinterJacket.Durability = 100.0f;
WinterJacket.Weight = 2.5f;

Weather->EquipClothing(EClothingSlot::Torso, WinterJacket);
```

**Blueprint:**
1. Create "Clothing Item" struct
2. Fill in properties
3. Call "Equip Clothing" with slot and item

### Clothing Wetness Mechanics

**Getting Wet:**
Clothing automatically gets wet in rain/snow based on water resistance.

```cpp
// In heavy rain (90% intensity)
float WetnessGain = (RainIntensity / 100.0f) × (1.0f - WaterResistance / 100.0f) × 10.0f × DeltaTime;
```

**Example:**
- Rain Intensity: 90%
- T-Shirt (10% water resistance): Gains ~8.1 wetness per second
- Rain Coat (95% water resistance): Gains ~0.45 wetness per second

**Drying:**
Call `DryClothing(Amount)` when near fire or in shelter:

```cpp
// Near campfire
Weather->DryClothing(20.0f * DeltaTime); // Dry 20% per second
```

**Checking Wetness:**
```cpp
float TotalWetness = Weather->GetTotalClothingWetness();
// 0 = Completely dry
// 100 = Completely soaked
```

---

## Temperature Mechanics

### Effective Temperature Calculation

The weather system calculates an **Effective Temperature** that combines multiple factors:

```
Effective Temperature =
    Ambient Temperature
    + Wind Chill Adjustment
    + Wetness Penalty
    + Clothing Protection
    + Shelter Bonus
```

#### **1. Ambient Temperature**
Base environmental temperature set by weather or manually.

**Typical ranges:**
- Arctic: -40°C to -10°C
- Temperate: 0°C to 30°C
- Desert: 30°C to 50°C

#### **2. Wind Chill Adjustment**

Wind makes cold weather feel colder:

```cpp
WindChill = -((WindSpeed / 10.0f) * (1.0f - AvgWindResistance)) * 5.0f;
```

**Example:**
- Ambient: 0°C
- Wind Speed: 20 m/s
- No clothing: Wind chill = -10°C → **Feels like -10°C**
- Good windbreaker (75% resistance): Wind chill = -2.5°C → **Feels like -2.5°C**

#### **3. Wetness Penalty**

Wet clothing dramatically reduces temperature:

```
Wetness Penalty = -(Total Wetness / 100) × 10°C
```

**Example:**
- 50% wet: -5°C penalty
- 100% soaked: -10°C penalty

**CRITICAL:** Getting wet in cold weather can be deadly!

#### **4. Clothing Protection**

Good insulation provides warmth:

**Cold Weather:**
```
Clothing Protection = (Total Cold Insulation / 100) × 15°C
```

**Example:**
- 100 total insulation: +15°C bonus
- 50 total insulation: +7.5°C bonus

**Hot Weather:**
Clothing with high heat insulation TRAPS heat:
```
Clothing Penalty = (Total Heat Insulation / 100) × 10°C
```

**Example wearing winter jacket in desert:**
- Heat insulation: 85
- Penalty: +8.5°C (makes you HOTTER!)

#### **5. Shelter Bonus**

Shelter protects from weather:

```
Shelter Bonus = (Shelter Level / 100) × 5°C
```

**Example:**
- 100% shelter (indoors): +5°C
- 50% shelter (cave): +2.5°C

### Complete Example

**Scenario: Caught in rain with mediocre clothing**

- Ambient Temperature: 5°C
- Wind Speed: 15 m/s
- Rain Intensity: 80%
- Clothing: 50 cold insulation, 40 wind resistance, 30 water resistance
- Clothing Wetness: 60% (got soaked)
- Shelter Level: 0%

**Calculation:**
1. Ambient: 5°C
2. Wind Chill: -((15/10) × (1-0.4)) × 5 = -4.5°C
3. Wetness Penalty: -(60/100) × 10 = -6°C
4. Clothing Protection: (50/100) × 15 = +7.5°C (but reduced by wetness!)
5. Shelter: 0°C

**Effective Temperature: 5 - 4.5 - 6 + 7.5 + 0 = 2°C**

**But wait!** Wetness also reduces clothing effectiveness:
- Effective insulation: 50 × (1 - 0.6 × 0.7) = 50 × 0.58 = 29
- Actual clothing protection: (29/100) × 15 = +4.35°C

**True Effective Temperature: 5 - 4.5 - 6 + 4.35 + 0 = -1.15°C**

**Result:** Character is freezing despite 5°C ambient temperature!

### Getting Temperature in Blueprint

**Get Effective Temperature (Full Breakdown):**
```cpp
FTemperatureResult Result = Weather->CalculateEffectiveTemperature();
// Result.AmbientTemperature
// Result.WindChillAdjustment
// Result.WetnessPenalty
// Result.ClothingProtection
// Result.ShelterBonus
// Result.EffectiveTemperature (final)
```

**Get Simple Temperature:**
```cpp
float Temp = Weather->GetEffectiveTemperature();
```

---

## Freezing Stages

### 5 Progressive Stages

Characters experience 5 stages of hypothermia as temperature drops:

#### **Stage 1: Chilled** (Effective Temp: 10°C to 15°C)
**Effects:**
- -2 Stamina/sec
- Slight shivering
- Minor discomfort

**Visual:** Shivering animation

#### **Stage 2: Cold** (Effective Temp: 5°C to 10°C)
**Effects:**
- -5 Stamina/sec
- -0.5 Core Health/sec
- Moderate shivering
- Movement speed slightly reduced

**Visual:** Heavy shivering, breath visible

#### **Stage 3: Freezing** (Effective Temp: 0°C to 5°C)
**Effects:**
- -10 Stamina/sec
- -2 Core Health/sec
- Severe movement penalties
- Inability to perform fine motor skills

**Visual:** Extreme shivering, blue-tinted skin

#### **Stage 4: Hypothermia** (Effective Temp: -5°C to 0°C)
**Effects:**
- -5 Core Health/sec
- Critical movement impairment
- Unconsciousness risk

**Visual:** Collapse animations, severe discoloration

#### **Stage 5: Critical Hypothermia** (Effective Temp: < -5°C)
**Effects:**
- -10 Core Health/sec
- Near death
- Imminent cardiac arrest

**Visual:** Character collapse, ice formation on clothing

### Preventing Freezing

**Methods:**
1. **Better Clothing** - High cold insulation
2. **Stay Dry** - Avoid rain, dry clothing near fire
3. **Find Shelter** - Increases effective temperature by up to +5°C
4. **Wind Protection** - High wind resistance clothing
5. **Heat Sources** - Campfires, buildings

### Recovering from Hypothermia

**Priority actions:**
1. Get indoors (shelter = 100%)
2. Remove wet clothing and equip dry clothing
3. Stand near fire (`DryClothing(20.0f)` per second)
4. Wait for temperature to rise above 15°C

**Time to recover:**
From Critical Hypothermia to Normal with proper shelter + fire: ~2-5 minutes

---

## Overheating Stages

### 5 Progressive Stages

Similar to freezing, but triggered by excessive heat:

#### **Stage 1: Warm** (Effective Temp: 30°C to 35°C)
**Effects:**
- -1 Stamina/sec
- Slight sweating

#### **Stage 2: Hot** (Effective Temp: 35°C to 40°C)
**Effects:**
- -3 Stamina/sec
- -0.3 Core Health/sec
- Moderate sweating

#### **Stage 3: Overheating** (Effective Temp: 40°C to 45°C)
**Effects:**
- -6 Stamina/sec
- -1.5 Core Health/sec
- Heavy sweating
- Movement speed reduced

#### **Stage 4: Heatstroke** (Effective Temp: 45°C to 50°C)
**Effects:**
- -4 Core Health/sec
- Severe impairment
- Risk of collapse

#### **Stage 5: Critical Heatstroke** (Effective Temp: > 50°C)
**Effects:**
- -8 Core Health/sec
- Organ failure imminent
- Near death

### Preventing Overheating

**Methods:**
1. **Low Heat Insulation Clothing** - Breathable fabrics
2. **Find Shade** - Shelter in hot weather
3. **Hydration** - Drink water (affects Hydration stat)
4. **Reduce Activity** - Lower exertion reduces heat generation

**IMPORTANT:** Do NOT wear winter clothing in hot weather!

**Example:**
- Desert ambient: 45°C
- Wearing winter jacket (85 heat insulation): +8.5°C penalty
- Effective temperature: **53.5°C** → Critical Heatstroke!

---

## Weather Presets

### Quick Configuration

Instead of manually setting all weather parameters, use presets:

### **Blizzard Preset**
```cpp
FWeatherPreset Blizzard = Weather->GetBlizzardPreset();
Weather->ApplyWeatherPreset(Blizzard);
```

**Properties:**
- Weather Type: Heavy Snow
- Temperature: -15°C ± 5°C
- Wind Speed: 25 m/s
- Snow Intensity: 90%
- Visibility: 30%

**Survival requirements:**
- High cold insulation clothing (80+)
- High wind resistance (75+)
- Waterproof outer layer (90+ water resistance)
- Shelter essential for long-term survival

### **Heatwave Preset**
```cpp
FWeatherPreset Heatwave = Weather->GetHeatwavePreset();
Weather->ApplyWeatherPreset(Heatwave);
```

**Properties:**
- Weather Type: Heatwave
- Temperature: 42°C ± 3°C
- Wind Speed: 3 m/s
- Visibility: 100%

**Survival requirements:**
- Low heat insulation clothing (< 20)
- Shade/shelter
- High hydration

### **Thunderstorm Preset**
```cpp
FWeatherPreset Storm = Weather->GetThunderstormPreset();
Weather->ApplyWeatherPreset(Storm);
```

**Properties:**
- Weather Type: Thunderstorm
- Temperature: 18°C ± 4°C
- Wind Speed: 20 m/s
- Rain Intensity: 85%
- Visibility: 50%

**Survival requirements:**
- High water resistance (70+)
- Wind protection
- Shelter to avoid wetness

### **Clear Weather Preset**
```cpp
FWeatherPreset Clear = Weather->GetClearWeatherPreset();
Weather->ApplyWeatherPreset(Clear);
```

**Properties:**
- Weather Type: Clear
- Temperature: 22°C ± 5°C
- Wind Speed: 5 m/s
- Visibility: 100%

**Survival requirements:** Minimal

### Creating Custom Presets

```cpp
FWeatherPreset CustomPreset;
CustomPreset.PresetName = "Arctic Storm";
CustomPreset.WeatherType = EWeatherType::HeavySnow;
CustomPreset.BaseTemperature = -30.0f;
CustomPreset.TemperatureVariance = 10.0f;
CustomPreset.WindSpeed = 35.0f;
CustomPreset.SnowIntensity = 95.0f;
CustomPreset.VisibilityMultiplier = 0.2f;
CustomPreset.bAffectsPlayerStats = true;

Weather->ApplyWeatherPreset(CustomPreset);
```

---

## Blueprint Usage

### Setting Up Weather Component

1. **Add Component to Actor:**
   - Select your Character/Actor
   - Add Component → Search "Weather Component"
   - Component appears in component list

2. **Configure Default Settings:**
   - Ambient Temperature: 20.0
   - Wind Speed: 5.0
   - Shelter Level: 0.0

### Common Blueprint Nodes

#### **Set Weather Type**
**Inputs:** Weather Type (dropdown)
**Example:** Set Weather Type → Blizzard

#### **Apply Weather Preset**
**Inputs:** Weather Preset (struct)
**Example:** Get Blizzard Preset → Apply Weather Preset

#### **Equip Clothing**
**Inputs:** Clothing Slot (dropdown), Clothing Item (struct)

#### **Get Effective Temperature**
**Outputs:** Float (temperature in °C)

#### **Get Current Freezing Stage**
**Outputs:** Freezing Stage (enum)

#### **Is Freezing?**
**Outputs:** Boolean (true if stage > None)

### Event Examples

#### **OnFreezingStageChanged**
```
Event OnFreezingStageChanged
    → Branch: Stage == Hypothermia?
        → True: Play Warning Sound
        → False: Continue
```

#### **OnWeatherChanged**
```
Event OnWeatherChanged
    → Print String: "Weather changed to {NewWeather}"
    → Play Weather Sound Effect
```

#### **OnClothingEquipped**
```
Event OnClothingEquipped
    → Print String: "Equipped {Item.DisplayName} on {Slot}"
    → Update Inventory UI
```

### Example Blueprint Flow

**Survival System:**
```
Event BeginPlay
    → Get Weather Component
    → Bind Event OnFreezingStageChanged

Event Tick
    → Get Effective Temperature
    → Branch: < 10°C?
        → True:
            → Set Widget Text: "You are cold! Find shelter!"
            → Play Shivering Animation
        → False: Continue

Event OnFreezingStageChanged (New Stage)
    → Switch on New Stage:
        → Chilled: Show UI Warning "Getting Cold"
        → Cold: Show UI Alert "Very Cold! Seek Warmth!"
        → Freezing: Screen Effect (Blue Tint)
        → Hypothermia: Camera Shake, Critical UI
        → Critical: Death Sequence
```

---

## Integration with Other Systems

### Stat Component Integration

The weather system automatically updates the **BodyTemperature** stat:

```cpp
// In WeatherComponent::TickComponent
if (UStatComponent* StatComp = GetOwner()->FindComponentByClass<UStatComponent>())
{
    float EffectiveTemp = CalculateEffectiveTemperature().EffectiveTemperature;
    StatComp->SetStatValue(EStatType::BodyTemperature, EffectiveTemp);
}
```

**Monitoring temperature in Blueprint:**
```
Get Stat Component
    → Get Stat Value (BodyTemperature)
    → Branch: < 15°C?
        → True: Warning System
```

### Body Component Integration (Future)

When Body Component is implemented, weather will affect:
- **Frostbite** - Damage to extremities (Hands, Feet) in freezing conditions
- **Sunburn** - Damage to exposed skin in extreme heat
- **Respiratory** - Cold air affects breathing, reduces stamina regen

### Environment Component Integration

The weather system IS part of the environment layer. It provides:
- Ambient temperature
- Wind speed
- Precipitation intensity
- Shelter level

These values are used by other environmental systems.

### Status Effect Integration (Future)

Weather can trigger status effects:
- **Hypothermia Effect** - Stacking cold damage
- **Heatstroke Effect** - Stacking heat damage
- **Wet Effect** - Reduced insulation, increased cold damage
- **Frostbite Effect** - Permanent damage to extremities

### Time System Integration (Future)

Weather can change based on time:
```cpp
// In TimeComponent::OnNewHour
if (CurrentHour >= 14 && CurrentHour <= 16) // Afternoon
{
    // Higher chance of thunderstorms
    Weather->SetWeatherType(EWeatherType::Thunderstorm);
}
```

**Day/Night temperature:**
```cpp
// Night: -5°C to ambient
// Day: +5°C to ambient
float TimeModifier = TimeComp->IsDaytime() ? 5.0f : -5.0f;
Weather->SetAmbientTemperature(BaseTemp + TimeModifier);
```

---

## Advanced Use Cases

### Use Case 1: Dynamic Weather System

**Goal:** Weather changes every 10 minutes

```cpp
UPROPERTY()
FTimerHandle WeatherTimer;

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(
        WeatherTimer,
        this,
        &AMyGameMode::ChangeRandomWeather,
        600.0f, // 10 minutes
        true // Loop
    );
}

void AMyGameMode::ChangeRandomWeather()
{
    TArray<EWeatherType> PossibleWeather = {
        EWeatherType::Clear,
        EWeatherType::Cloudy,
        EWeatherType::Rain,
        EWeatherType::Thunderstorm
    };

    int32 RandomIndex = FMath::RandRange(0, PossibleWeather.Num() - 1);

    for (TActorIterator<APawn> It(GetWorld()); It; ++It)
    {
        if (UWeatherComponent* Weather = It->FindComponentByClass<UWeatherComponent>())
        {
            Weather->SetWeatherType(PossibleWeather[RandomIndex]);
        }
    }
}
```

### Use Case 2: Campfire Warmth Zone

**Goal:** Standing near campfire warms you up and dries clothing

```cpp
// In Campfire actor
void ACampfire::BeginPlay()
{
    Super::BeginPlay();

    // Create sphere collision
    WarmthZone = CreateDefaultSubobject<USphereComponent>(TEXT("WarmthZone"));
    WarmthZone->InitSphereRadius(300.0f); // 3 meter radius
    WarmthZone->OnComponentBeginOverlap.AddDynamic(this, &ACampfire::OnEnterWarmth);
    WarmthZone->OnComponentEndOverlap.AddDynamic(this, &ACampfire::OnExitWarmth);
}

void ACampfire::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Warm all actors in range
    for (AActor* Actor : ActorsInRange)
    {
        if (UWeatherComponent* Weather = Actor->FindComponentByClass<UWeatherComponent>())
        {
            // Increase temperature bonus
            Weather->SetShelterLevel(100.0f); // Full shelter

            // Dry clothing
            Weather->DryClothing(15.0f * DeltaTime); // 15% per second
        }
    }
}
```

### Use Case 3: Clothing Durability Loss

**Goal:** Clothing degrades over time and with use

```cpp
void UWeatherComponent::TickComponent(float DeltaTime, ...)
{
    Super::TickComponent(DeltaTime, ...);

    // Degrade clothing in harsh conditions
    if (CurrentWeatherType == EWeatherType::Sandstorm ||
        CurrentWeatherType == EWeatherType::Thunderstorm)
    {
        float DurabilityLoss = 0.5f * DeltaTime; // 0.5% per second in harsh weather

        for (auto& Pair : EquippedClothing)
        {
            FClothingItem& Item = Pair.Value;
            Item.Durability = FMath::Max(0.0f, Item.Durability - DurabilityLoss);

            if (Item.Durability <= 0.0f)
            {
                // Item destroyed!
                OnClothingDestroyed.Broadcast(Pair.Key, Item);
            }
        }
    }
}
```

### Use Case 4: Seasonal Weather

**Goal:** Weather appropriate to current season

```cpp
void AGameMode::SetSeasonalWeather(ESeasonType Season)
{
    FWeatherPreset SeasonPreset;

    switch (Season)
    {
    case ESeasonType::Winter:
        SeasonPreset.WeatherType = EWeatherType::Snow;
        SeasonPreset.BaseTemperature = -5.0f;
        SeasonPreset.TemperatureVariance = 10.0f;
        break;

    case ESeasonType::Summer:
        SeasonPreset.WeatherType = EWeatherType::Clear;
        SeasonPreset.BaseTemperature = 28.0f;
        SeasonPreset.TemperatureVariance = 7.0f;
        break;

    case ESeasonType::Spring:
        SeasonPreset.WeatherType = EWeatherType::Rain;
        SeasonPreset.BaseTemperature = 15.0f;
        SeasonPreset.TemperatureVariance = 8.0f;
        break;

    case ESeasonType::Autumn:
        SeasonPreset.WeatherType = EWeatherType::Overcast;
        SeasonPreset.BaseTemperature = 12.0f;
        SeasonPreset.TemperatureVariance = 6.0f;
        break;
    }

    // Apply to all players
    for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
    {
        if (UWeatherComponent* Weather = It->FindComponentByClass<UWeatherComponent>())
        {
            Weather->ApplyWeatherPreset(SeasonPreset);
        }
    }
}
```

### Use Case 5: Temperature-Based Stamina Drain

**Goal:** Extreme temperatures drain stamina faster

```cpp
// In Character class
void ACharacter::TickStaminaDrain(float DeltaTime)
{
    if (UWeatherComponent* Weather = FindComponentByClass<UWeatherComponent>())
    {
        float EffectiveTemp = Weather->GetEffectiveTemperature();
        float StaminaDrainMultiplier = 1.0f;

        // Cold increases stamina drain
        if (EffectiveTemp < 10.0f)
        {
            StaminaDrainMultiplier = 1.0f + ((10.0f - EffectiveTemp) * 0.1f);
            // At 0°C: 2x stamina drain
            // At -10°C: 3x stamina drain
        }

        // Heat increases stamina drain
        else if (EffectiveTemp > 30.0f)
        {
            StaminaDrainMultiplier = 1.0f + ((EffectiveTemp - 30.0f) * 0.05f);
            // At 40°C: 1.5x stamina drain
            // At 50°C: 2x stamina drain
        }

        // Apply multiplied drain
        if (UStatComponent* Stats = FindComponentByClass<UStatComponent>())
        {
            float BaseDrain = 5.0f; // 5 stamina/sec when running
            Stats->ApplyStatChange(
                EStatType::Stamina,
                -BaseDrain * StaminaDrainMultiplier * DeltaTime,
                TEXT("Exertion")
            );
        }
    }
}
```

### Use Case 6: Clothing Loadout System

**Goal:** Quick-swap clothing sets for different environments

```cpp
USTRUCT(BlueprintType)
struct FClothingLoadout
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LoadoutName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EClothingSlot, FClothingItem> ClothingSet;
};

// Define loadouts
FClothingLoadout WinterLoadout;
WinterLoadout.LoadoutName = "Arctic Gear";
WinterLoadout.ClothingSet.Add(EClothingSlot::Head, CreateFurHat());
WinterLoadout.ClothingSet.Add(EClothingSlot::Torso, CreateParka());
WinterLoadout.ClothingSet.Add(EClothingSlot::Legs, CreateInsulatedPants());
WinterLoadout.ClothingSet.Add(EClothingSlot::Feet, CreateWinterBoots());
WinterLoadout.ClothingSet.Add(EClothingSlot::Hands, CreateInsulatedGloves());

// Apply loadout
void ApplyClothingLoadout(UWeatherComponent* Weather, const FClothingLoadout& Loadout)
{
    for (const auto& Pair : Loadout.ClothingSet)
    {
        Weather->EquipClothing(Pair.Key, Pair.Value);
    }
}
```

### Use Case 7: Weather-Dependent AI Behavior

**Goal:** NPCs seek shelter in bad weather

```cpp
void AAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (UWeatherComponent* Weather = FindComponentByClass<UWeatherComponent>())
    {
        // Check if weather is dangerous
        bool bDangerousWeather =
            Weather->GetCurrentWeatherType() == EWeatherType::Blizzard ||
            Weather->GetCurrentWeatherType() == EWeatherType::Thunderstorm ||
            Weather->GetCurrentWeatherType() == EWeatherType::Sandstorm;

        if (bDangerousWeather && Weather->GetShelterLevel() < 50.0f)
        {
            // Seek shelter!
            AActor* NearestShelter = FindNearestShelter();
            if (NearestShelter)
            {
                GetCharacterMovement()->MaxWalkSpeed = 600.0f; // Run!
                MoveToActor(NearestShelter);
            }
        }
    }
}
```

---

## Summary

The Weather System provides a comprehensive environmental survival simulation with:

**Key Features:**
- 13 weather types with realistic effects
- 6-slot clothing system with wetness and durability
- Advanced temperature calculations
- 5 freezing stages and 5 overheating stages
- Weather presets for easy configuration
- Full Blueprint integration
- Multiplayer replication

**Critical Mechanics to Remember:**
1. **Wet clothing loses up to 70% effectiveness** - Stay dry!
2. **Wind chill can make temperatures feel 10°C+ colder** - Wind protection matters!
3. **Shelter provides up to +5°C** - Always seek shelter in harsh weather
4. **High heat insulation in hot weather is DANGEROUS** - Wear breathable clothing in deserts
5. **Temperature stages progress quickly** - Monitor your temperature constantly

**Integration:**
- Automatically updates BodyTemperature stat
- Works with other StatSystemPro components
- Future integration with Body Component, Status Effects, Time System

**Performance:**
- Efficient tick-based updates
- Multiplayer optimized with DOREPLIFETIME
- Minimal overhead with smart calculations

Use this system to create immersive survival experiences where weather is a constant challenge!
