# StatSystemPro - Professional Modular Stat System for Unreal Engine 5.6

> **🎯 Simple by Default, Powerful When Needed**

A comprehensive, modular stat system plugin for Unreal Engine 5.6 that provides a complete framework for managing character stats, body conditions, status effects, environmental interactions, and character progression.

## ⚡ **LATEST IMPROVEMENTS!**

**🎉 Major New Features Added:**
- ✅ **Expanded Weather System** - 13 weather types, clothing mechanics, freezing/overheating stages! ❄️🔥
- ✅ **Clothing System** - 6 slots, wetness mechanics, wet clothing loses 70% effectiveness
- ✅ **Temperature Mechanics** - Wind chill, shelter, advanced calculations with all factors
- ✅ **Fixed Regeneration Curves** - Now work correctly! (Y-axis = actual regen rate, not multiplier)
- ✅ **Global Time System** - Complete day/night cycle, seasons, 8 time-based events
- ✅ **11 New Stat Functions** - Transfer stats, batch operations, threshold counting, comparison
- ✅ **Project Settings** - Configure defaults in Edit → Project Settings → Plugins

**[📖 Read Full Improvements Guide →](IMPROVEMENTS.md)**
**[🌦️ Weather System Guide →](WEATHER_SYSTEM_GUIDE.md)** ⭐ NEW!

## 🚀 Quick Start (30 Seconds!)

**For Blueprint Users:**
1. Add "Stat Component" to your character
2. That's it! All stats are automatically set to 100 and working!
3. **[📘 Read the Blueprint Documentation →](BLUEPRINT_DOCUMENTATION.md)** (Complete beginner-to-advanced guide)

**For C++ Users:**
```cpp
// Add component
UPROPERTY(VisibleAnywhere)
UStatComponent* StatComp;

// Use it
StatComp->ApplyStatChange(EStatType::Health_Core, -25.0f, TEXT("Damage"), FGameplayTag());
```

## 🎓 Documentation

- **[WEATHER_SYSTEM_GUIDE.md](WEATHER_SYSTEM_GUIDE.md)** - **🌦️ WEATHER SYSTEM (NEW!)** ⭐
  - 13 weather types (Clear, Rain, Snow, Blizzard, Heatwave, etc.)
  - Clothing system with 6 slots (wetness mechanics explained!)
  - Temperature calculations (wind chill, shelter, clothing protection)
  - 5 freezing stages and 5 overheating stages
  - Weather presets for instant configuration
  - 7 advanced use cases
  - **Complete survival weather guide!**

- **[IMPROVEMENTS.md](IMPROVEMENTS.md)** - **✨ LATEST IMPROVEMENTS**
  - Fixed regeneration curve system explained
  - Global time system complete guide
  - 11 new stat functions documented
  - Project settings configuration
  - Migration guide for upgrading
  - **Read this first to see what's new!**

- **[COMPLETE_FUNCTION_REFERENCE.md](COMPLETE_FUNCTION_REFERENCE.md)** - **📖 COMPLETE GUIDE**
  - **What regeneration curves are and how to use them**
  - Every single function with ALL inputs/outputs explained
  - 12 advanced use cases (bleeding, temperature, boss fights, etc.)
  - Perfect for when you're stuck and need examples
  - **Complete function reference!**

- **[ALL_LAYERS_DOCUMENTATION.md](ALL_LAYERS_DOCUMENTATION.md)** - **📚 ALL 5 LAYERS EXPLAINED**
  - Status Effect system (what stacks are, all effect types)
  - Body Layer system (fractures, bleeding, infections)
  - Environment Layer (temperature, weather, radiation)
  - Progression Layer (XP, levels, skills)
  - How layers work together (6 integration examples)

- **[BLUEPRINT_DOCUMENTATION.md](BLUEPRINT_DOCUMENTATION.md)** - Blueprint-specific docs
  - Every function explained with examples
  - Common use cases
  - Multiplayer setup
  - Troubleshooting
  - Performance tips

- **[ENUM_INTERFACE_GUIDE.md](ENUM_INTERFACE_GUIDE.md)** - **NO TEXT TYPING!**
  - 100% enum-based interface (dropdown selection only)
  - Stat categories (Core, Survival, Environmental, etc.)
  - Custom list support (select exactly which stats to check)
  - Visual examples and use cases
  - **Zero typos possible!**

- **[WHATS_NEW.md](WHATS_NEW.md)** - Earlier updates summary
  - Summary of previous features
  - Before/after comparisons

- **[README.md](README.md)** (This file) - Technical overview and C++ reference

## Features

StatSystemPro is built on a **modular architecture** where each layer can be independently enabled or disabled based on your project's needs.

### 🎯 Core Layers

#### 1. **Stat Layer**
Manages all numeric character values with dynamic regeneration and decay:
- **Core Stats**: Health, Stamina, Energy
- **Survival Stats**: Hunger, Thirst, Fatigue
- **Environmental Stats**: Body Temperature, Wetness
- **Health Stats**: Blood Level, Sanity, Infection Level, Toxicity
- **RPG Stats** (Optional): Strength, Dexterity, Intelligence, Endurance

**Features:**
- Real-time stat regeneration/decay with customizable curves
- Min/max value management with modifier support
- Event-driven architecture with delegates
- Data table configuration support

#### 2. **Body Layer**
Advanced body part management system:
- **Body Parts**: Head, Torso, Left/Right Arms, Left/Right Legs
- **Conditions per Part**:
  - Overall condition (0-100)
  - Fractures
  - Bleeding rate
  - Burn levels (1st, 2nd, 3rd degree)
  - Infection rate
  - Pain level

**Gameplay Effects:**
- Leg damage → Reduced movement speed, increased stamina drain
- Arm damage → Reduced accuracy, increased weapon sway
- Torso damage → Reduced max health and stamina
- Head damage → Sanity drain, unconsciousness risk

#### 3. **Status Effect Layer**
Flexible buff/debuff framework:
- **Effect Types**: Temporary, Conditional, Permanent, Stackable
- **Trigger System**: Manual, On Damage, Over Time, Environment, Body Part
- **Features**:
  - Priority-based effect resolution
  - Stack management
  - Duration tracking
  - Stat modifiers (flat and percentage)
  - Gameplay tag integration
  - Data table configuration

**Example Effects:**
- Exhausted, Panicked, Adrenaline Rush
- Hypothermia, Heatstroke, Cold
- Infection stages, Poisoned

#### 4. **Environment Layer** ⭐ EXPANDED!
Dynamic environmental interaction system with comprehensive weather mechanics:
- **13 Weather Types**: Clear, Cloudy, Rain, Heavy Rain, Thunderstorm, Snow, Blizzard, Heatwave, Fog, Sandstorm, and more
- **Advanced Clothing System**: 6 equipment slots (Head, Torso, Legs, Feet, Hands, Back)
  - Cold/Heat insulation properties
  - Water and wind resistance
  - Wetness mechanics (wet clothing loses up to 70% effectiveness!)
  - Durability system
- **Temperature System**:
  - Wind chill calculations
  - Wetness penalties (up to -10°C when soaked)
  - Clothing protection (up to +15°C with good gear)
  - Shelter bonuses (up to +5°C)
- **Progressive Temperature Stages**:
  - 5 Freezing stages: Chilled → Cold → Freezing → Hypothermia → Critical
  - 5 Overheating stages: Warm → Hot → Overheating → Heatstroke → Critical
- **Weather Presets**: Blizzard, Heatwave, Thunderstorm, Clear (instant configuration)
- **Radiation**: Optional radiation exposure mechanics

**Advanced Features:**
- Realistic temperature calculations considering all factors
- Clothing gets wet in rain/snow and dries when sheltered
- Wind chill makes cold weather feel colder
- Progressive damage based on temperature exposure
- Full Blueprint integration with 7 weather events
- Multiplayer replicated
- **[📖 Full Weather Guide →](WEATHER_SYSTEM_GUIDE.md)**

#### 5. **Progression Layer**
Complete character advancement system:
- **Leveling**: XP-based with configurable curves
- **Skills & Perks**: Unlockable abilities with prerequisites
- **Skill Categories**: Combat, Survival, Crafting, Social, Exploration
- **Reward Sources**: Combat, Crafting, Exploration, Survival, Quest
- **Passive Bonuses**: Time-based survival XP

**Features:**
- Flexible skill trees with prerequisites
- Attribute and skill point management
- Level-based rewards
- Gameplay tag integration

## 🎯 Simple Mode vs Advanced Mode

### Simple Mode (Default - Perfect for 90% of Games)

**What you get:**
- ✅ Zero configuration needed
- ✅ All 16 stats auto-initialized to 100
- ✅ Just add component and start coding
- ✅ Perfect for prototyping, game jams, indie games

**Setup:**
```
1. Add Stat Component to actor
2. Done! (Simple Mode is ON by default)
```

### Advanced Mode (For Production Customization)

**What you get:**
- ✅ Custom stat values per stat type
- ✅ Custom regeneration rates
- ✅ Regeneration curves for non-linear regen
- ✅ Data table configuration
- ✅ Fine-grained control

**Setup:**
```
1. Set "Use Simple Mode" = FALSE
2. Create Data Table (Row Type: StatConfigRow)
3. Configure each stat as needed
4. Assign table to component
```

**You can switch between modes anytime!**

## 🌐 Multiplayer Support

**Full replication out of the box:**
- ✅ Automatic stat synchronization (server → clients)
- ✅ Server authority on all changes
- ✅ Bandwidth optimized
- ✅ Client prediction friendly
- ✅ Zero extra setup required

**How it works:**
1. Server changes stats → Auto-replicates to all clients
2. Clients can read all values in real-time
3. Events fire on all machines
4. UI updates automatically

[See multiplayer examples in Blueprint Documentation](BLUEPRINT_DOCUMENTATION.md#multiplayer-setup)

## Architecture

### Modular Design
Each layer is completely independent and can be:
- ✅ Enabled/Disabled at runtime
- 🔌 Used standalone or combined
- 🎨 Customized via data tables
- 📊 Extended through C++ or Blueprints
- 🌐 Fully replicated for multiplayer

### Component-Based System
The plugin uses Unreal Engine's component system:
- `UStatSystemProComponent` - Main integration component
- Individual layer components for granular control
- Automatic component linking and initialization
- Full replication support built-in

## Installation

1. Copy the `StatSystemPro` folder to your project's `Plugins` directory
2. Regenerate project files
3. Build your project
4. Enable the plugin in the Plugins menu

## Quick Start

### C++ Usage

```cpp
// Add the component to your character
UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
UStatSystemProComponent* StatSystem;

// In constructor
StatSystem = CreateDefaultSubobject<UStatSystemProComponent>(TEXT("StatSystem"));

// Apply damage
StatSystem->ApplyGeneralDamage(25.0f);

// Check conditions
if (StatSystem->IsCritical())
{
    // Handle critical condition
}

// Heal
StatSystem->Heal(50.0f);

// Award XP
StatSystem->AwardExperience(100, EXPSource::Combat);
```

### Blueprint Usage

The plugin includes a comprehensive Blueprint Function Library:

```
Get Stat Value (Actor, Stat Type) → Float
Change Stat Value (Actor, Stat Type, Amount)
Damage Body Part (Actor, Body Part, Damage)
Apply Status Effect (Actor, Effect ID, Stacks)
Set Ambient Temperature (Actor, Temperature)
Award XP (Actor, Amount, Source)
Is Critical (Actor) → Bool
Is Dehydrated (Actor) → Bool
```

### Adding to Your Character

1. **Simple Setup**: Add `StatSystemProComponent` to your character
   - This automatically creates and links all layer components

2. **Manual Setup**: Add individual layer components for fine control
   - `StatComponent`
   - `BodyComponent`
   - `StatusEffectComponent`
   - `EnvironmentComponent`
   - `ProgressionComponent`

## Configuration

### Data Tables

The system supports data table configuration for:

#### Stat Configuration
Create a data table with `FStatConfigRow`:
- Default max values
- Regeneration rates
- Regeneration curves

#### Status Effects
Create a data table with `FStatusEffectTableRow`:
- Effect definitions
- Durations
- Stat modifiers
- Stack limits

#### Skills
Create a data table with `FSkillTableRow`:
- Skill definitions
- Prerequisites
- Costs
- Effects

## API Reference

### Main Component Functions

```cpp
// Layer Control
void InitializeLayers();
void SetLayerEnabled(FName LayerName, bool bEnabled);
bool IsLayerEnabled(FName LayerName);

// Query Functions
bool IsCritical();
bool IsHypothermic();
bool IsDehydrated();
bool IsStarving();
bool IsInShock();
float GetOverallHealthStatus();

// Helper Functions
void ApplyDamageToBodyPart(EBodyPart, float);
void ApplyGeneralDamage(float);
void Heal(float);
void Consume(float Hunger, float Thirst, float Energy);
void ApplyEffect(FName EffectID, int32 Stacks);
void AwardExperience(int32 Amount, EXPSource Source);
FString GetDebugInfo();
```

### Events/Delegates

Each layer provides events for gameplay integration:
- `OnStatChanged`, `OnStatReachedZero`
- `OnBodyPartDamaged`, `OnBodyPartFractured`
- `OnStatusEffectApplied`, `OnStatusEffectExpired`
- `OnHypothermia`, `OnHeatstroke`
- `OnXPGained`, `OnLevelUp`, `OnSkillUnlocked`

## Examples

### Survival Game Setup

```cpp
// Initialize system
StatSystem->InitializeLayers();

// Set environment
StatSystem->EnvironmentLayer->SetAmbientTemperature(-10.0f);
StatSystem->EnvironmentLayer->SetRainLevel(0.8f);
StatSystem->EnvironmentLayer->SetShelterState(EShelterState::Outside);

// The system automatically:
// - Reduces body temperature
// - Increases wetness
// - Triggers hypothermia effects
// - Affects stamina and health
```

### Combat System Integration

```cpp
// Hit detection
void OnHit(FHitResult Hit)
{
    // Determine body part from hit location
    EBodyPart BodyPart = GetBodyPartFromBone(Hit.BoneName);

    // Apply damage to specific body part
    StatSystem->ApplyDamageToBodyPart(BodyPart, Damage);

    // Check for critical injury
    if (StatSystem->BodyLayer->GetBodyPartState(BodyPart).IsCritical())
    {
        // Apply status effect
        StatSystem->ApplyEffect(TEXT("Bleeding"), 1);
    }
}
```

### RPG Progression

```cpp
// Award XP for actions
void OnEnemyKilled()
{
    StatSystem->AwardExperience(50, EXPSource::Combat);
}

void OnItemCrafted()
{
    StatSystem->AwardExperience(25, EXPSource::Crafting);
}

// Unlock skills
void OnPlayerChooseSkill(FName SkillID)
{
    if (StatSystem->ProgressionLayer->CanUnlockSkill(SkillID))
    {
        StatSystem->ProgressionLayer->UnlockSkill(SkillID);
    }
}
```

## Performance

The system is designed for efficiency:
- ✅ Component-based tick management
- ✅ Layer-level enable/disable
- ✅ Event-driven updates
- ✅ Minimal per-frame overhead
- ✅ Data table caching

## Compatibility

- **Engine Version**: Unreal Engine 5.6+
- **Language**: C++ with full Blueprint support
- **Platforms**: All platforms supported by UE5

## Support & Documentation

For issues, questions, or contributions, please refer to the project repository.

## License

Copyright Epic Games, Inc. All Rights Reserved.

---

## Version History

### v1.0.0 (2025)
- Initial release
- Complete implementation of all 5 layers
- Blueprint function library
- Comprehensive event system
- Data table support
