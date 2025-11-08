# StatSystemPro - Complete Documentation

> **The Ultimate Guide to StatSystemPro Plugin for Unreal Engine 5.6**
>
> Everything you need to know about stats, body systems, weather, progression, and editor tools in one comprehensive document.

**Version:** 1.0.0
**Last Updated:** 2025-11-08
**Plugin Type:** Runtime + Editor
**Compatibility:** Unreal Engine 5.6+

---

## Table of Contents

### 🚀 Getting Started
1. [Overview](#overview)
2. [Quick Start (30 Seconds)](#quick-start-30-seconds)
3. [Installation](#installation)
4. [Architecture & Design](#architecture--design)
5. [System Flowcharts](#system-flowcharts)

### 📊 Core Systems

#### **Stat System**
6. [Stat System Overview](#stat-system-overview)
7. [All Stat Types Explained](#all-stat-types-explained)
8. [Stat Ranges & What They Mean](#stat-ranges--what-they-mean)
9. [Regeneration System](#regeneration-system)
10. [Regeneration Curves Explained](#regeneration-curves-explained)
11. [StatComponent Complete Function Reference](#statcomponent-complete-function-reference)
12. [Stat System Examples](#stat-system-examples)

#### **Body System**
13. [Body System Overview](#body-system-overview)
14. [All Body Parts Explained](#all-body-parts-explained)
15. [Body Part Conditions & Effects](#body-part-conditions--effects)
16. [Fractures, Bleeding, Burns](#fractures-bleeding-burns)
17. [BodyComponent Complete Function Reference](#bodycomponent-complete-function-reference)
18. [Body System Examples](#body-system-examples)

#### **Weather & Environment System**
19. [Weather System Overview](#weather-system-overview)
20. [All Weather Types Explained](#all-weather-types-explained)
21. [Temperature Values & What They Mean](#temperature-values--what-they-mean)
22. [Wind Speed, Shelter, & Environmental Factors](#wind-speed-shelter--environmental-factors)
23. [Clothing System Complete Guide](#clothing-system-complete-guide)
24. [Temperature Calculation Breakdown](#temperature-calculation-breakdown)
25. [Freezing & Overheating Stages](#freezing--overheating-stages)
26. [WeatherComponent Complete Function Reference](#weathercomponent-complete-function-reference)
27. [Weather System Examples](#weather-system-examples)

#### **Status Effect System**
28. [Status Effect System Overview](#status-effect-system-overview)
29. [Effect Types & Stacking](#effect-types--stacking)
30. [StatusEffectComponent Complete Function Reference](#statuseffectcomponent-complete-function-reference)
31. [Status Effect Examples](#status-effect-examples)

#### **Progression System**
32. [Progression System Overview](#progression-system-overview)
33. [XP, Levels, & Skills](#xp-levels--skills)
34. [ProgressionComponent Complete Function Reference](#progressioncomponent-complete-function-reference)
35. [Progression Examples](#progression-examples)

#### **Time System**
36. [Time System Overview](#time-system-overview)
37. [Day/Night Cycle](#daynight-cycle)
38. [TimeComponent Complete Function Reference](#timecomponent-complete-function-reference)

### 🛠️ Editor Tools

39. [Editor Tools Overview](#editor-tools-overview)
40. [Enhanced Details Panels](#enhanced-details-panels)
41. [Quick Actions Reference](#quick-actions-reference)
42. [Weather & Clothing Presets](#weather--clothing-presets)
43. [Testing Workflows](#testing-workflows)
44. [Creating Custom Editor Tools](#creating-custom-editor-tools)

### 🎮 Advanced Usage

45. [Complete Integration Examples](#complete-integration-examples)
46. [Multiplayer Setup](#multiplayer-setup)
47. [Save & Load System](#save--load-system)
48. [DataTable Overrides](#datatable-overrides)
49. [Project Settings Configuration](#project-settings-configuration)
50. [Performance Optimization](#performance-optimization)

### 📚 Reference

51. [Blueprint Function Library](#blueprint-function-library)
52. [Enum Reference](#enum-reference)
53. [Event Delegates Reference](#event-delegates-reference)
54. [Troubleshooting](#troubleshooting)
55. [FAQ](#faq)

---


## Overview

StatSystemPro is a **comprehensive, production-ready modular stat system** for Unreal Engine 5.6 that provides everything you need to build survival games, RPGs, action games, or any game requiring character state management.

### What is StatSystemPro?

StatSystemPro is not just a health/stamina system. It's a complete framework for managing:
- **Character vitals** (health, stamina, energy, hunger, thirst, etc.)
- **Body damage simulation** (locational damage, fractures, bleeding, burns)
- **Environmental survival** (temperature, weather, clothing, freezing, overheating)
- **Status effects** (buffs, debuffs, stacking effects, timed effects)
- **Character progression** (XP, levels, skills, perks)
- **Time simulation** (day/night cycles, seasons, time-based events)

### Key Philosophy: Simple by Default, Powerful When Needed

**Simple Mode** (Default):
- Add StatComponent → Done! All 18 stats initialized to 100
- Zero configuration required
- Perfect for prototyping, game jams, indie games

**Advanced Mode** (Production):
- Custom stat values per type
- Regeneration curves for non-linear behavior
- DataTable configuration for designers
- Project-wide settings
- Full multiplayer replication

### What Makes StatSystemPro Different?

| Feature | StatSystemPro | Typical Stat Plugins |
|---------|---------------|---------------------|
| **Enum-Based Interface** | ✅ 100% dropdown selection, zero text typing | ❌ Text-based, typo-prone |
| **Modular Architecture** | ✅ Enable/disable layers independently | ❌ All-or-nothing |
| **Editor Tools** | ✅ Custom details panels, 20+ quick actions | ❌ Manual value entry only |
| **Weather System** | ✅ 13 weather types, clothing, temperature | ❌ Not included |
| **Body Part Damage** | ✅ 6 body parts with fractures, bleeding | ❌ Single health pool only |
| **Multiplayer** | ✅ Automatic replication, server authority | ❌ Manual replication setup |
| **Documentation** | ✅ 15,000+ words, examples, flowcharts | ❌ Basic readme only |

### Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    StatSystemPro Plugin                      │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌───────────┐  ┌───────────┐  ┌──────────┐  ┌──────────┐  │
│  │   Stat    │  │   Body    │  │ Weather  │  │  Status  │  │
│  │   Layer   │  │   Layer   │  │  Layer   │  │  Effect  │  │
│  │           │  │           │  │          │  │  Layer   │  │
│  └─────┬─────┘  └─────┬─────┘  └────┬─────┘  └────┬─────┘  │
│        │              │              │             │         │
│        └──────────────┴──────────────┴─────────────┘         │
│                           │                                  │
│                    ┌──────┴──────┐                          │
│                    │ Progression │                          │
│                    │    Layer    │                          │
│                    └──────┬──────┘                          │
│                           │                                  │
│  ┌────────────────────────┴────────────────────────┐        │
│  │         Time System (Global)                    │        │
│  └─────────────────────────────────────────────────┘        │
│                                                              │
└──────────────────────────────────────────────────────────────┘

Each layer is independent and optional!
Enable only what you need for your game.
```

### Layer Responsibilities

#### **Stat Layer** 📊
**Purpose:** Numeric values that represent character state
- Health, Stamina, Energy
- Hunger, Thirst, Fatigue
- Body Temperature, Wetness
- Blood Level, Sanity, Toxicity
- Optional RPG stats (Strength, Dexterity, etc.)

**When to use:** Every game needs this layer

#### **Body Layer** 🩺
**Purpose:** Locational damage and physical conditions
- 6 body parts (Head, Torso, Arms, Legs)
- Fractures (broken bones)
- Bleeding (blood loss over time)
- Burns (1st, 2nd, 3rd degree)
- Infections and pain

**When to use:** Realistic damage simulation, survival games, tactical shooters

#### **Weather Layer** 🌦️
**Purpose:** Environmental conditions affecting survival
- 13 weather types (Clear → Blizzard → Heatwave)
- Temperature simulation
- 6-slot clothing system
- Freezing and overheating stages
- Wind chill, wetness, shelter

**When to use:** Survival games, open-world games, realistic simulations

#### **Status Effect Layer** ✨
**Purpose:** Temporary or permanent modifiers
- Buffs and debuffs
- Stacking effects
- Timed effects
- Conditional effects
- Stat modifiers (flat and percentage)

**When to use:** RPGs, action games, any game with power-ups/debuffs

#### **Progression Layer** 📈
**Purpose:** Character advancement
- XP and leveling
- Skills and perks
- Skill trees with prerequisites
- Attribute points
- Multiple XP sources

**When to use:** RPGs, roguelikes, progression-based games

#### **Time System** 🕐
**Purpose:** Global time simulation
- Day/night cycle
- Seasons
- Time-based events
- Calendar system

**When to use:** Games with time-dependent mechanics

---

## Quick Start (30 Seconds)

### For Blueprint Users

**Step 1:** Add `StatComponent` to your character
1. Open your character Blueprint
2. Add Component → Search "Stat Component"
3. Done! All stats automatically initialized to 100

**Step 2:** Use the stats
```
Event BeginPlay
→ Get Component by Class (StatComponent)
→ Apply Stat Change (Health_Core, -25, "Damage")
```

**That's it!** Simple Mode is enabled by default.

### For C++ Users

**Step 1:** Add component to your character header
```cpp
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
UStatComponent* StatComponent;
```

**Step 2:** Initialize in constructor
```cpp
AMyCharacter::AMyCharacter()
{
    StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}
```

**Step 3:** Use it
```cpp
void AMyCharacter::TakeDamage(float Damage)
{
    StatComponent->ApplyStatChange(EStatType::Health_Core, -Damage, TEXT("Damage"));
}
```

### First Test - Editor Tools

**Step 1:** Select your character in the editor
**Step 2:** Look at the Details panel
**Step 3:** Find "Quick Actions" category
**Step 4:** Click "Test Critical Condition"

Your character's health is now at 10%! Test your UI, effects, death sequence.

**Step 5:** Click "Restore All Stats" when done

---


## Installation

### Method 1: Plugin Folder (Recommended)

1. Download/clone StatSystemPro
2. Copy the `StatSystemPro` folder to `YourProject/Plugins/`
3. Right-click your `.uproject` file → Generate Visual Studio project files
4. Open the solution and build
5. Launch the editor
6. Edit → Plugins → Search "StatSystemPro" → Enable
7. Restart the editor

### Method 2: Engine Plugins

1. Copy `StatSystemPro` to `UE_5.6/Engine/Plugins/`
2. Restart the editor
3. Plugin is available in all projects

### Verification

Check the Output Log for:
```
LogTemp: StatSystemProEditor: Module Started Successfully
```

If you see this, the plugin is loaded and ready!

### First-Time Setup

After installation:

1. **Test Basic Stats:**
   - Create a test actor
   - Add StatComponent
   - PIE (Play In Editor)
   - All stats should be at 100

2. **Test Editor Tools:**
   - Select the actor
   - Details panel should show "Quick Actions"
   - Click "Deplete All Stats"
   - Stats should go to 0

3. **Test Weather System:**
   - Add WeatherComponent
   - Click "Apply Blizzard"
   - Temperature should drop to -15°C

If all three work, you're ready!

---

## Architecture & Design

### Component-Based Architecture

StatSystemPro uses Unreal's component system for maximum flexibility:

```
Actor (e.g., Character)
├── StatComponent          → Manages all numeric stats
├── BodyComponent          → Manages body parts (optional)
├── WeatherComponent       → Manages weather/clothing (optional)
├── StatusEffectComponent  → Manages buffs/debuffs (optional)
├── ProgressionComponent   → Manages XP/levels (optional)
└── TimeComponent          → Manages time (usually on GameMode)
```

**Benefits:**
- Add only what you need
- Components are independent
- Easy to test in isolation
- Compatible with existing character classes
- No inheritance required
- Follows Unreal's composition pattern

### Data Flow

```
┌─────────────────────────────────────────────────────────────┐
│                      Game Events                             │
│  (Damage, Healing, Weather Change, Time Passage, etc.)       │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────────────┐
│                  Component Processing                        │
│                                                              │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐  │
│  │  Stat Comp   │◄───┤  Body Comp   │    │ Weather Comp │  │
│  │              │    │              │◄───┤              │  │
│  │  Updates:    │    │  Updates:    │    │  Updates:    │  │
│  │  - Health    │    │  - Condition │    │  - Temp      │  │
│  │  - Stamina   │    │  - Fractures │    │  - Wetness   │  │
│  │  - Regen     │    │  - Bleeding  │    │  - Clothing  │  │
│  └──────┬───────┘    └──────┬───────┘    └──────┬───────┘  │
│         │                   │                    │           │
│         └───────────────────┴────────────────────┘           │
│                             │                                │
└─────────────────────────────┼────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    Event Broadcasts                          │
│  (OnStatChanged, OnBodyPartDamaged, OnWeatherChanged, etc.)  │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────────────┐
│                   Game Responses                             │
│  (UI Updates, Animations, Effects, Death, etc.)              │
└─────────────────────────────────────────────────────────────┘
```

### Multiplayer Architecture

```
Server (Authority)
├── StatComponent [Replicated]
│   ├── Stats Map [Replicated]
│   ├── Apply changes on server
│   └── Automatically syncs to clients
│
├── BodyComponent [Replicated]
│   ├── Body Parts Map [Replicated]
│   └── Damage applied on server
│
└── WeatherComponent [Replicated]
    ├── Weather State [Replicated]
    ├── Clothing [Replicated]
    └── Temperature calculated on server

Clients (Remote)
├── StatComponent [Receive Only]
│   ├── Read stat values
│   ├── Listen to events
│   └── Update UI
│
└── All other components [Receive Only]

RPC Flow:
Client requests action → Server validates → Server applies change →
Replication updates all clients → Events fire on clients
```

**Key Points:**
- Server has authority
- All modifications on server
- Clients receive updates automatically
- No manual replication code needed (handled by DOREPLIFETIME)

### Event-Driven Design

All components use **delegates** for loose coupling:

```
┌──────────────┐         ┌──────────────┐         ┌──────────────┐
│              │ Event   │              │ Event   │              │
│ StatComponent├────────►│ UI Widget    │         │  AI System   │
│              │         │              │         │              │
└──────────────┘         └──────────────┘         └──────────────┘
       │                                                  ▲
       │ Event                                            │
       └──────────────────────────────────────────────────┘

Example:
1. Health drops to 10%
2. StatComponent broadcasts OnStatChanged
3. UI Widget receives event → Update health bar
4. AI System receives event → Switch to flee behavior
```

**No direct dependencies!** Components don't know about UI or AI.

---

## System Flowcharts

### Stat Regeneration Flow

```
┌────────────────────────────────────────────────────────────┐
│                    Every Tick (ΔTime)                       │
└────────────────┬───────────────────────────────────────────┘
                 │
                 ▼
        ┌────────────────────┐
        │ For Each Stat Type │
        └────────┬───────────┘
                 │
                 ▼
        ┌────────────────────┐       NO
        │ Has Regen Rate?    ├──────────► Skip
        └────────┬───────────┘
                 │ YES
                 ▼
        ┌────────────────────┐       NO
        │ Has Regen Curve?   ├──────────► Use Base Rate
        └────────┬───────────┘           │
                 │ YES                   │
                 ▼                       │
        ┌────────────────────┐           │
        │ Get Current %      │           │
        │ (Current/Max)      │           │
        └────────┬───────────┘           │
                 │                       │
                 ▼                       │
        ┌────────────────────┐           │
        │ Sample Curve at %  │           │
        │ Y = Regen Amount   │           │
        └────────┬───────────┘           │
                 │                       │
                 └───────┬───────────────┘
                         │
                         ▼
                ┌────────────────────┐
                │ Regen Amount × ΔT  │
                └────────┬───────────┘
                         │
                         ▼
                ┌────────────────────┐       YES
                │ Would Exceed Max?  ├──────────► Clamp to Max
                └────────┬───────────┘
                         │ NO
                         ▼
                ┌────────────────────┐
                │ Apply Regeneration │
                └────────┬───────────┘
                         │
                         ▼
                ┌────────────────────┐
                │ Broadcast Event    │
                │ OnStatChanged      │
                └────────────────────┘
```

### Temperature Calculation Flow

```
┌────────────────────────────────────────────────────────────┐
│             WeatherComponent::TickComponent                 │
└────────────────┬───────────────────────────────────────────┘
                 │
                 ▼
        ┌────────────────────┐
        │ Get Ambient Temp   │ ← Set manually or by weather preset
        └────────┬───────────┘   (e.g., -15°C for Blizzard)
                 │
                 ▼
        ┌────────────────────────────────────────┐
        │ Calculate Wind Chill                   │
        │ = -(WindSpeed/10) × (1-WindResist) × 5 │
        │                                        │
        │ Example:                               │
        │ Wind: 20 m/s, No protection            │
        │ = -(20/10) × (1-0) × 5 = -10°C        │
        └────────┬───────────────────────────────┘
                 │
                 ▼
        ┌────────────────────────────────────────┐
        │ Calculate Wetness Penalty              │
        │ = -(TotalWetness/100) × 10°C           │
        │                                        │
        │ Example:                               │
        │ 50% wet = -5°C penalty                │
        │ 100% soaked = -10°C penalty           │
        └────────┬───────────────────────────────┘
                 │
                 ▼
        ┌────────────────────────────────────────┐
        │ Calculate Clothing Protection          │
        │ Cold: (ColdInsulation/100) × 15°C      │
        │ Heat: +(HeatInsulation/100) × 10°C     │
        │ (Note: Wetness reduces insulation)     │
        │                                        │
        │ Example:                               │
        │ 80 insulation, dry = +12°C            │
        │ 80 insulation, 50% wet = +6.2°C      │
        └────────┬───────────────────────────────┘
                 │
                 ▼
        ┌────────────────────────────────────────┐
        │ Calculate Shelter Bonus                │
        │ = (ShelterLevel/100) × 5°C             │
        │                                        │
        │ Example:                               │
        │ 100% shelter (indoors) = +5°C         │
        │ 50% shelter (cave) = +2.5°C           │
        └────────┬───────────────────────────────┘
                 │
                 ▼
        ┌────────────────────────────────────────┐
        │ Sum All Factors                        │
        │ Effective = Ambient + WindChill +      │
        │            Wetness + Clothing + Shelter│
        │                                        │
        │ Real Example:                          │
        │ Ambient: -15°C (Blizzard)             │
        │ Wind Chill: -10°C                     │
        │ Wetness: -5°C                         │
        │ Clothing: +12°C (good gear)           │
        │ Shelter: 0°C (outside)                │
        │ = -18°C (FREEZING!)                   │
        └────────┬───────────────────────────────┘
                 │
                 ▼
        ┌────────────────────┐
        │ Determine Stage    │
        │ Freezing/Overheat  │
        │                    │
        │ -18°C = Stage 3    │
        │ (Freezing)         │
        └────────┬───────────┘
                 │
                 ▼
        ┌────────────────────┐       YES
        │ Stage Changed?     ├──────────► Broadcast Event
        └────────┬───────────┘           OnFreezingStageChanged
                 │ NO
                 ▼
        ┌────────────────────┐
        │ Apply Damage       │
        │ Based on Stage     │
        │                    │
        │ Stage 3: -2 HP/sec │
        └────────┬───────────┘
                 │
                 ▼
        ┌────────────────────┐
        │ Update BodyTemp    │
        │ Stat in StatComp   │
        └────────────────────┘
```

### Body Damage Propagation Flow

```
┌────────────────────────────────────────────────────────────┐
│              DamageBodyPart(BodyPart, Damage)               │
└────────────────┬───────────────────────────────────────────┘
                 │
                 ▼
        ┌────────────────────┐
        │ Apply Damage to    │
        │ Body Part          │
        │ Condition -= Damage│
        └────────┬───────────┘
                 │
                 ▼
        ┌────────────────────┐       YES
        │ Condition <= 0?    ├──────────► Body Part Destroyed
        └────────┬───────────┘           (Condition = 0)
                 │ NO                    Pain +100
                 ▼
        ┌────────────────────┐       YES
        │ Below Fracture     ├──────────► Auto-Fracture
        │ Threshold (20%)?   │           bFractured = true
        └────────┬───────────┘           Pain +50
                 │ NO
                 ▼
        ┌────────────────────────────────────────┐
        │ Propagate to Stat Component            │
        │                                        │
        │ Head damage:                           │
        │   → Sanity -15                        │
        │   → Screen blur effect                │
        │                                        │
        │ Torso damage:                          │
        │   → Health max -20%                   │
        │   → Stamina max -15%                  │
        │                                        │
        │ Limb damage:                           │
        │   → Movement speed -30%               │
        │   → Weapon accuracy -40%              │
        └────────┬───────────────────────────────┘
                 │
                 ▼
        ┌────────────────────┐
        │ Add Pain           │
        │ Pain += Damage/2   │
        └────────┬───────────┘
                 │
                 ▼
        ┌────────────────────┐       YES
        │ Is Bleeding?       ├──────────► Apply Blood Loss
        └────────┬───────────┘           │ BleedRate per sec
                 │ NO                    │ to BloodLevel stat
                 └───────┬───────────────┘
                         │
                         ▼
                ┌────────────────────┐
                │ Broadcast Event    │
                │ OnBodyPartDamaged  │
                │ (BodyPart, Damage) │
                └────────────────────┘
```

### Stat Change Decision Tree

```
ApplyStatChange(StatType, Amount, Source)
        │
        ▼
    ┌───────────────┐       NO
    │ Amount == 0?  ├──────────► Continue
    └───────┬───────┘
            │ YES
            ▼
         Return (no change)

    ┌───────────────┐
    │ Get Current   │
    │ Stat Value    │
    └───────┬───────┘
            │
            ▼
    ┌───────────────┐
    │ NewValue =    │
    │ Current+Amount│
    └───────┬───────┘
            │
            ▼
    ┌───────────────┐       YES    ┌──────────────┐
    │ NewValue > Max├──────────────►│ Clamp to Max │
    └───────┬───────┘               └──────┬───────┘
            │ NO                           │
            ▼                              │
    ┌───────────────┐       YES    ┌──────┴───────┐
    │ NewValue < Min├──────────────►│ Clamp to Min │
    └───────┬───────┘               └──────┬───────┘
            │ NO                           │
            └──────────┬───────────────────┘
                       │
                       ▼
              ┌────────────────┐
              │ Set New Value  │
              └────────┬───────┘
                       │
                       ▼
              ┌────────────────┐       YES
              │ NewValue == 0? ├──────────► Broadcast OnStatReachedZero
              └────────┬───────┘
                       │ NO
                       ▼
              ┌────────────────┐
              │ Broadcast      │
              │ OnStatChanged  │
              └────────────────┘
```

---


## Stat System Overview

The Stat System is the **foundation** of StatSystemPro. It manages all numeric values that represent character state.

### What is a Stat?

A stat is a **numeric value** with:
- **Current Value** - The actual current amount (e.g., 75 HP)
- **Max Value** - The maximum possible amount (e.g., 100 HP)  
- **Min Value** - The minimum possible amount (usually 0)
- **Regeneration Rate** - How much it regenerates per second (e.g., +2 HP/sec)
- **Regeneration Curve** (Optional) - Non-linear regeneration based on current %

### Stat Component Features

✅ **18 Built-in Stat Types** (fully customizable)
✅ **Automatic Regeneration/Decay** with curves
✅ **Min/Max Management** with clamping
✅ **Category System** for bulk operations
✅ **Custom Stat Lists** for specific queries
✅ **Event System** for UI/gameplay responses
✅ **Multiplayer Replication** automatic
✅ **Simple Mode** zero configuration
✅ **Advanced Mode** full customization
✅ **DataTable Support** for designers
✅ **Project Settings** for global defaults

---

## All Stat Types Explained

StatSystemPro includes 18 pre-defined stat types organized into categories. Here's EVERYTHING you need to know about each one:

### Core Stats (Fundamental Vitals)

#### **Health_Core**

**Display Name:** Core Health  
**Purpose:** Main health pool, character dies when 0  
**Default Value:** 100  
**Default Max:** 100  
**Min:** 0  
**Category:** Core  
**Typical Use:** Primary damage target

**VALUE MEANINGS:**

| Value | Percentage | Status | Visual Cues | Gameplay Effects |
|-------|-----------|--------|-------------|------------------|
| 100 | 100% | Perfect health | No damage indicators | Full performance |
| 90-99 | 90-99% | Barely injured | Minor blood/scratches | No penalties |
| 75-89 | 75-89% | Lightly wounded | Visible wounds | -5% movement speed |
| 60-74 | 60-74% | Wounded | Blood, limping | -10% movement speed |
| 45-59 | 45-59% | Heavily wounded | Heavy bleeding | -20% movement, -15% accuracy |
| 30-44 | 30-44% | Critically wounded | Severe wounds | -30% movement, -25% accuracy |
| 15-29 | 15-29% | Critical condition | Near death effects | -50% movement, -40% accuracy |
| 1-14 | 1-14% | Near death | Death screen effects | Barely mobile |
| 0 | 0% | Dead | Death state | Character dies |

**REAL WORLD COMPARISONS:**

Think of health like a car's structural integrity:
- 100% = Showroom condition
- 75% = Minor dents and scratches  
- 50% = Significant damage, still drivable
- 25% = Major structural damage
- 0% = Totaled

**EXAMPLE CODE:**

```cpp
// Taking damage from a bullet
void ACharacter::OnHitByBullet(float Damage)
{
    StatComp->ApplyStatChange(EStatType::Health_Core, -Damage, TEXT("Bullet"));
    
    // Check current health percentage
    float HealthPercent = StatComp->GetStatPercentage(EStatType::Health_Core);
    
    if (HealthPercent <= 0.15f) // 15%
    {
        // Critical health!
        PlayCriticalHealthSound();
        ApplyScreenBloodEffect();
        ReduceMovementSpeed(0.5f); // 50% reduction
    }
    else if (HealthPercent <= 0.50f) // 50%
    {
        // Wounded
        PlayWoundedBreathingSound();
        ApplyLimbingAnimation();
    }
}

// Healing
void ACharacter::UseHealthPack(float HealAmount)
{
    StatComp->ApplyStatChange(EStatType::Health_Core, HealAmount, TEXT("HealthPack"));
    
    // Health packs typically restore 30-50 HP
    // Small: 30 HP
    // Medium: 50 HP
    // Large: 100 HP (full heal)
}
```

**REGENERATION BEHAVIOR:**

Health typically does NOT regenerate automatically. Use regeneration for:
- Superhero/sci-fi games (shields regenerating)
- RPGs with passive healing
- Easy mode for casual games

```cpp
// Setup health regeneration (optional)
StatComp->SetStatRegenerationRate(EStatType::Health_Core, 1.0f); // +1 HP/sec

// Or use a curve for slower regen at low health
// (See Regeneration Curves section)
```

---

#### **Stamina**

**Display Name:** Stamina  
**Purpose:** Physical exertion capacity  
**Default Value:** 100  
**Default Max:** 100  
**Min:** 0  
**Category:** Core  
**Typical Use:** Running, jumping, melee attacks, dodging

**VALUE MEANINGS:**

| Value | Status | Can Sprint? | Can Jump? | Can Dodge? | Recovery Time |
|-------|--------|-------------|-----------|------------|---------------|
| 90-100 | Energized | Yes (full speed) | Yes | Yes | Instant |
| 70-89 | Good | Yes | Yes | Yes | 2-3 sec |
| 50-69 | Tiring | Yes (slower) | Yes | Limited | 5 sec |
| 30-49 | Tired | Barely | Limited | No | 8 sec |
| 10-29 | Exhausted | No | No | No | 15 sec |
| 0-9 | Depleted | No | No | No | 20+ sec |

**WHAT IT MEANS IN GAMEPLAY:**

- **100 Stamina:** You can sprint indefinitely (while stamina drains)
- **75 Stamina:** Starting to tire, heavy breathing
- **50 Stamina:** Tired, need to slow down soon
- **25 Stamina:** Exhausted, can barely run
- **0 Stamina:** Must walk, can't perform actions

**TYPICAL DRAIN RATES:**

| Action | Stamina Cost | Duration | Total Drain |
|--------|--------------|----------|-------------|
| Walking | -0.5/sec | Continuous | Low |
| Jogging | -3/sec | Continuous | Moderate |
| Sprinting | -10/sec | 10 sec | 100 (full) |
| Jumping | -15 | Instant | 15 per jump |
| Melee attack | -10 | Instant | 10 per swing |
| Heavy attack | -25 | Instant | 25 per swing |
| Dodging/Rolling | -20 | Instant | 20 per dodge |
| Blocking | -5/sec | While blocking | Moderate |
| Climbing | -8/sec | Continuous | High |

**TYPICAL REGEN RATES:**

| State | Regeneration | Notes |
|-------|--------------|-------|
| Standing still | +15/sec | Fast recovery |
| Walking | +5/sec | Slow recovery |
| Jogging | +2/sec | Very slow |
| Sprinting | 0/sec | No regen |
| In combat | +3/sec | Reduced |
| Out of combat (5 sec) | +20/sec | Bonus recovery |

**EXAMPLE CODE:**

```cpp
// Sprint system
void ACharacter::Sprint()
{
    float CurrentStamina = StatComp->GetStatValue(EStatType::Stamina);
    
    if (CurrentStamina > 10.0f) // Need at least 10 stamina to sprint
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = 600.0f; // Sprint speed
    }
}

void ACharacter::Tick(float DeltaTime)
{
    if (bIsSprinting)
    {
        // Drain stamina
        StatComp->ApplyStatChange(EStatType::Stamina, -10.0f * DeltaTime, TEXT("Sprinting"));
        
        // Auto-stop when depleted
        if (StatComp->GetStatValue(EStatType::Stamina) <= 0.0f)
        {
            StopSprinting();
        }
    }
}

void ACharacter::StopSprinting()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = 300.0f; // Walk speed
}

// Dodge/Roll
void ACharacter::Dodge()
{
    float CurrentStamina = StatComp->GetStatValue(EStatType::Stamina);
    
    if (CurrentStamina >= 20.0f) // Dodge costs 20 stamina
    {
        StatComp->ApplyStatChange(EStatType::Stamina, -20.0f, TEXT("Dodge"));
        PerformDodgeAnimation();
        ApplyDodgeInvulnerability(0.5f); // 0.5 sec invulnerable
    }
    else
    {
        // Not enough stamina!
        PlayFailSound();
    }
}
```

**REGENERATION SETUP:**

```cpp
// Setup stamina regeneration
// Standing still: +15/sec
StatComp->SetStatRegenerationRate(EStatType::Stamina, 15.0f);

// For variable regen based on state, update it dynamically:
void ACharacter::Tick(float DeltaTime)
{
    float RegenRate = 15.0f; // Default: standing
    
    if (bIsSprinting)
        RegenRate = 0.0f; // No regen while sprinting
    else if (bIsWalking)
        RegenRate = 5.0f; // Slow regen while walking
    else if (bInCombat)
        RegenRate = 3.0f; // Reduced in combat
    
    StatComp->SetStatRegenerationRate(EStatType::Stamina, RegenRate);
}
```

---

#### **Energy**

**Display Name:** Energy / Mana / Power  
**Purpose:** Mental/magical power  
**Default Value:** 100  
**Default Max:** 100  
**Min:** 0  
**Category:** Core  
**Typical Use:** Casting spells, using abilities, special attacks

**VALUE MEANINGS:**

| Value | Spell Tier Available | Gameplay State |
|-------|---------------------|----------------|
| 90-100 | All abilities | Full power |
| 70-89 | High tier spells | Plenty of energy |
| 50-69 | Medium tier spells | Moderate energy |
| 30-49 | Low tier spells | Limited energy |
| 10-29 | Basic spells only | Nearly drained |
| 0-9 | No spells | Drained |

**ABILITY COST EXAMPLES:**

| Ability Type | Energy Cost | Cooldown | Power Level |
|--------------|-------------|----------|-------------|
| Basic attack | 0 | None | Low |
| Minor spell | 10 | 2 sec | Low |
| Standard spell | 25 | 5 sec | Medium |
| Power spell | 50 | 10 sec | High |
| Ultimate ability | 100 | 60 sec | Ultimate |

**EXAMPLE CODE:**

```cpp
// Spell casting
void ACharacter::CastSpell(ESpellType SpellType)
{
    float SpellCost = GetSpellEnergyCost(SpellType);
    float CurrentEnergy = StatComp->GetStatValue(EStatType::Energy);
    
    if (CurrentEnergy >= SpellCost)
    {
        // Cast spell
        StatComp->ApplyStatChange(EStatType::Energy, -SpellCost, TEXT("Spell"));
        PerformSpellCast(SpellType);
    }
    else
    {
        // Not enough energy!
        ShowNotification("Not enough energy!");
        PlayFailSound();
    }
}

float ACharacter::GetSpellEnergyCost(ESpellType SpellType)
{
    switch (SpellType)
    {
    case ESpellType::Fireball:      return 25.0f;
    case ESpellType::Lightning:     return 35.0f;
    case ESpellType::Heal:          return 30.0f;
    case ESpellType::Teleport:      return 50.0f;
    case ESpellType::Ultimate:      return 100.0f;
    default:                        return 10.0f;
    }
}

// Energy regeneration (typically slower than stamina)
// Setup: +5 energy/sec
StatComp->SetStatRegenerationRate(EStatType::Energy, 5.0f);
```

---

### Survival Stats (Basic Needs)

#### **Hunger**

**Display Name:** Hunger  
**Purpose:** Need for food  
**Default Value:** 0  
**Default Max:** 100  
**Min:** 0  
**Category:** Survival  
**IMPORTANT:** **HIGHER = MORE HUNGRY** (inverse of other stats!)

**VALUE MEANINGS:**

| Value | Status | Gameplay Effects | Urgency |
|-------|--------|------------------|---------|
| 0-10 | Full / Stuffed | No effects, possibly slow | None |
| 10-25 | Satisfied | No effects | None |
| 25-40 | Slight hunger | No effects yet | Low |
| 40-60 | Hungry | -5% stamina regen | Medium |
| 60-75 | Very hungry | -15% stamina regen, -5% max stamina | High |
| 75-90 | Starving | -25% stamina regen, -15% max stamina, -5% movement | Critical |
| 90-100 | Severe starvation | -50% stamina, -1 HP/min, weakness | URGENT |

**REAL WORLD COMPARISON:**

Think of it like your real hunger:
- 0-25 = Just ate, satisfied
- 25-50 = Starting to think about food
- 50-75 = Hungry, should eat soon
- 75-90 = Very hungry, hard to focus
- 90-100 = Starving, physical weakness

**TIME TO STARVATION:**

In a typical survival game:
- Hunger increases: +1 every 5 minutes (real-time)
- Time to hungry (50): ~4 hours gameplay
- Time to starving (75): ~6 hours gameplay  
- Time to death (100): ~8 hours gameplay

**EXAMPLE CODE:**

```cpp
// Hunger system
void ACharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Hunger increases over time (decay)
    // +1 hunger every 5 minutes = 0.00333 per second
    StatComp->SetStatRegenerationRate(EStatType::Hunger, 0.00333f);
}

void ACharacter::Tick(float DeltaTime)
{
    float HungerLevel = StatComp->GetStatValue(EStatType::Hunger);
    
    // Apply penalties based on hunger
    if (HungerLevel >= 90.0f)
    {
        // Starving! Take damage
        StatComp->ApplyStatChange(EStatType::Health_Core, -1.0f * DeltaTime / 60.0f, TEXT("Starvation"));
        
        // Severe penalties
        StaminaRegenMultiplier = 0.5f; // 50% stamina regen
        MaxStaminaMultiplier = 0.5f; // 50% max stamina
    }
    else if (HungerLevel >= 75.0f)
    {
        // Very hungry
        StaminaRegenMultiplier = 0.75f;
        MaxStaminaMultiplier = 0.85f;
    }
    else if (HungerLevel >= 60.0f)
    {
        // Hungry
        StaminaRegenMultiplier = 0.85f;
    }
}

// Eating food
void ACharacter::Eat(float FoodValue)
{
    // Reduce hunger (remember: lower = better!)
    StatComp->ApplyStatChange(EStatType::Hunger, -FoodValue, TEXT("Eating"));
}

// Food examples
void ACharacter::EatApple()
{
    Eat(15.0f); // Reduces hunger by 15
}

void ACharacter::EatCookedMeat()
{
    Eat(40.0f); // Reduces hunger by 40
}

void ACharacter::EatFullMeal()
{
    Eat(70.0f); // Reduces hunger by 70
}
```

**FOOD VALUE REFERENCE:**

| Food Item | Hunger Reduction | Notes |
|-----------|------------------|-------|
| Berry | -5 | Small snack |
| Apple | -15 | Light snack |
| Bread | -25 | Basic food |
| Cooked meat | -40 | Good meal |
| Stew | -60 | Hearty meal |
| Feast | -80 | Full meal |

---

#### **Thirst**

**Display Name:** Thirst  
**Purpose:** Need for water  
**Default Value:** 0  
**Default Max:** 100  
**Min:** 0  
**Category:** Survival  
**IMPORTANT:** **HIGHER = MORE THIRSTY** (inverse!)

**VALUE MEANINGS:**

| Value | Status | Gameplay Effects | Time to Death |
|-------|--------|------------------|---------------|
| 0-15 | Hydrated | No effects | - |
| 15-30 | Slight thirst | No effects yet | - |
| 30-50 | Thirsty | -10% stamina max | - |
| 50-70 | Dehydrated | -20% stamina max, -5% movement | 6 hours |
| 70-85 | Severely dehydrated | -35% stamina, -15% movement, -10% health max | 3 hours |
| 85-100 | Critical dehydration | -50% stamina, -30% movement, -2 HP/min | 1 hour |

**CRITICAL INFO:** Dehydration is MORE DANGEROUS than hunger!

| Factor | Hunger | Thirst |
|--------|--------|--------|
| Increase rate | +1 per 5 min | +1 per 2 min |
| Time to critical | ~6 hours | ~3 hours |
| Time to death | ~8 hours | ~4 hours |
| Severity | Moderate | SEVERE |
| Penalties | Stamina focused | Health + Stamina |

**WHY THIRST IS DEADLIER:**
- Increases 2.5x faster than hunger
- Affects health directly (not just stamina)
- Real world: Can survive 3 weeks without food, only 3 days without water

**EXAMPLE CODE:**

```cpp
// Thirst system
void ACharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Thirst increases faster than hunger
    // +1 thirst every 2 minutes = 0.00833 per second
    StatComp->SetStatRegenerationRate(EStatType::Thirst, 0.00833f);
}

void ACharacter::Tick(float DeltaTime)
{
    float ThirstLevel = StatComp->GetStatValue(EStatType::Thirst);
    
    // Dehydration penalties (SEVERE!)
    if (ThirstLevel >= 85.0f)
    {
        // Critical dehydration! Taking damage
        StatComp->ApplyStatChange(EStatType::Health_Core, -2.0f * DeltaTime / 60.0f, TEXT("Dehydration"));
        
        // Severe penalties
        StaminaMaxMultiplier = 0.5f; // 50% max stamina
        MovementSpeedMultiplier = 0.7f; // 30% slower
        HealthMaxMultiplier = 0.9f; // 10% less max health
    }
    else if (ThirstLevel >= 70.0f)
    {
        // Severely dehydrated
        StaminaMaxMultiplier = 0.65f;
        MovementSpeedMultiplier = 0.85f;
        HealthMaxMultiplier = 0.9f;
    }
    else if (ThirstLevel >= 50.0f)
    {
        // Dehydrated
        StaminaMaxMultiplier = 0.8f;
        MovementSpeedMultiplier = 0.95f;
    }
    else if (ThirstLevel >= 30.0f)
    {
        // Thirsty
        StaminaMaxMultiplier = 0.9f;
    }
}

// Drinking water
void ACharacter::Drink(float WaterValue)
{
    // Reduce thirst (lower = better)
    StatComp->ApplyStatChange(EStatType::Thirst, -WaterValue, TEXT("Drinking"));
}

// Drink examples
void ACharacter::DrinkWaterBottle()
{
    Drink(50.0f); // Reduces thirst by 50
}

void ACharacter::DrinkFromStream()
{
    Drink(30.0f); // Reduces thirst by 30
    // Note: Might also add infection risk!
}
```

**DRINK VALUE REFERENCE:**

| Drink Item | Thirst Reduction | Notes |
|------------|------------------|-------|
| Sip of water | -10 | Minimal |
| Water bottle (250ml) | -35 | Standard |
| Water bottle (500ml) | -50 | Good |
| Canteen (1L) | -75 | Excellent |
| Full hydration | -100 | Complete |

**ENVIRONMENTAL FACTORS:**

Thirst increases faster in certain conditions:

```cpp
// Adjust thirst rate based on environment
float ThirstRate = 0.00833f; // Base rate

if (bInDesert)
    ThirstRate *= 2.0f; // 2x faster in heat

if (bIsSprinting)
    ThirstRate *= 1.5f; // 1.5x faster while running

if (BodyTemperature > 39.0f) // Overheating
    ThirstRate *= 2.5f; // Much faster when overheating

StatComp->SetStatRegenerationRate(EStatType::Thirst, ThirstRate);
```

---


#### **Fatigue**

**Display Name:** Fatigue / Tiredness  
**Purpose:** Need for sleep/rest  
**Default Value:** 0  
**Default Max:** 100  
**Min:** 0  
**Category:** Survival  
**IMPORTANT:** **HIGHER = MORE TIRED** (inverse!)

**VALUE MEANINGS:**

| Value | Status | Visual Effects | Gameplay Effects | Sleep Needed |
|-------|--------|----------------|------------------|--------------|
| 0-20 | Well-rested | Clear vision | No penalties | No |
| 20-40 | Alert | Normal | No penalties | No |
| 40-60 | Getting tired | Slight blur | -5% accuracy | Recommended |
| 60-75 | Tired | More blur | -15% accuracy, -10% stamina regen | Yes |
| 75-90 | Exhausted | Heavy blur | -30% accuracy, -25% stamina regen, slower reactions | URGENT |
| 90-100 | Cannot function | Severe blur, micro-sleeps | -50% accuracy, -50% stamina regen, risk of passing out | CRITICAL |

**FATIGUE ACCUMULATION:**

In typical 24-hour game cycle:
- +1 fatigue per 30 minutes awake (game time)
- Reaches 100 after ~17 hours awake
- Sleep for 8 hours = -100 fatigue (fully rested)

**EXAMPLE CODE:**

```cpp
// Fatigue system
void ACharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Fatigue increases while awake
    // +1 per 30 min game time
    // Assuming 1 real minute = 1 game hour: +1 per 30 sec real time
    StatComp->SetStatRegenerationRate(EStatType::Fatigue, 1.0f / 30.0f);
}

void ACharacter::Tick(float DeltaTime)
{
    float FatigueLevel = StatComp->GetStatValue(EStatType::Fatigue);
    
    // Fatigue effects
    if (FatigueLevel >= 90.0f)
    {
        // Exhausted! Risk of passing out
        AccuracyMultiplier = 0.5f;
        StaminaRegenMultiplier = 0.5f;
        
        // Chance to pass out
        if (FMath::FRandRange(0.0f, 1.0f) < 0.01f * DeltaTime) // 1% chance per second
        {
            PassOut(10.0f); // Pass out for 10 seconds
        }
    }
    else if (FatigueLevel >= 75.0f)
    {
        // Very tired
        AccuracyMultiplier = 0.7f;
        StaminaRegenMultiplier = 0.75f;
        ApplyScreenBlur(0.5f);
    }
    else if (FatigueLevel >= 60.0f)
    {
        // Tired
        AccuracyMultiplier = 0.85f;
        StaminaRegenMultiplier = 0.9f;
        ApplyScreenBlur(0.2f);
    }
}

// Sleeping
void ACharacter::Sleep(float Hours)
{
    // Reduce fatigue
    float FatigueReduction = Hours * 12.5f; // 8 hours = 100 fatigue reduction
    StatComp->ApplyStatChange(EStatType::Fatigue, -FatigueReduction, TEXT("Sleeping"));
    
    // Also restore some health/hunger/thirst while sleeping
    StatComp->ApplyStatChange(EStatType::Health_Core, Hours * 2.0f, TEXT("Rest"));
}
```

---

### Environmental Stats (World Interaction)

#### **BodyTemperature**

**Display Name:** Body Temperature (°C)  
**Purpose:** Core body temperature  
**Default Value:** 37.0  
**Default Max:** 42.0 (deadly fever)  
**Min:** 25.0 (deadly hypothermia)  
**Normal Range:** 36.5-37.5°C  
**Category:** Environmental

**VALUE MEANINGS (in Celsius):**

| Temperature | Status | Effects | Danger Level |
|-------------|--------|---------|--------------|
| 42°C+ | Critical Hyperthermia | Organ failure, death imminent | FATAL |
| 40-42°C | Severe Heatstroke | Confusion, seizures, -5 HP/sec | CRITICAL |
| 39-40°C | Heatstroke | Weakness, nausea, -2 HP/sec | SEVERE |
| 38-39°C | High Fever / Overheating | Fatigue, -1 HP/sec | DANGEROUS |
| 37.5-38°C | Elevated / Warm | Light sweating, minor discomfort | MILD |
| 36.5-37.5°C | **NORMAL** | No effects, healthy | SAFE |
| 36-36.5°C | Slightly Cool | Shivering begins | MILD |
| 35-36°C | Mild Hypothermia | Shivering, -0.5 HP/sec | DANGEROUS |
| 32-35°C | Hypothermia | Confusion, drowsiness, -2 HP/sec | SEVERE |
| 30-32°C | Severe Hypothermia | Unconscious, -5 HP/sec | CRITICAL |
| <30°C | Critical Hypothermia | Death imminent | FATAL |

**FAHRENHEIT CONVERSION:**

| Celsius | Fahrenheit | Status |
|---------|-----------|---------|
| 37°C | 98.6°F | Normal |
| 35°C | 95°F | Mild hypothermia |
| 30°C | 86°F | Severe hypothermia |
| 39°C | 102.2°F | Fever |
| 40°C | 104°F | High fever |
| 42°C | 107.6°F | Fatal |

**WHAT AFFECTS BODY TEMPERATURE:**

```
Body Temp = Affected by WeatherComponent!

Factors:
1. Ambient Temperature (-40°C to +50°C)
2. Wind Chill (up to -10°C)
3. Wetness (up to -10°C)
4. Clothing Insulation (up to +15°C)
5. Shelter (up to +5°C)
6. Physical Activity (+1°C when sprinting)
7. Illness/Infection (+1-3°C fever)
```

**WeatherComponent automatically updates this stat!**

**EXAMPLE CODE:**

```cpp
// Body temperature is usually managed by WeatherComponent
// But you can also modify it manually:

void ACharacter::Tick(float DeltaTime)
{
    float BodyTemp = StatComp->GetStatValue(EStatType::BodyTemperature);
    
    // Temperature stage effects
    if (BodyTemp >= 40.0f)
    {
        // Severe heatstroke!
        StatComp->ApplyStatChange(EStatType::Health_Core, -5.0f * DeltaTime, TEXT("Heatstroke"));
        ApplyScreenDistortion();
        ShowHeatstrokeWarning();
    }
    else if (BodyTemp >= 39.0f)
    {
        // Heatstroke
        StatComp->ApplyStatChange(EStatType::Health_Core, -2.0f * DeltaTime, TEXT("Overheating"));
        ReduceStamina(0.5f);
    }
    else if (BodyTemp <= 30.0f)
    {
        // Severe hypothermia!
        StatComp->ApplyStatChange(EStatType::Health_Core, -5.0f * DeltaTime, TEXT("Hypothermia"));
        ApplyFrostEffect();
        ShowHypothermiaWarning();
    }
    else if (BodyTemp <= 35.0f)
    {
        // Hypothermia
        StatComp->ApplyStatChange(EStatType::Health_Core, -2.0f * DeltaTime, TEXT("Cold"));
        ApplyShiveringAnimation();
    }
}

// Manual temperature modification (fever from illness)
void ACharacter::GetSick()
{
    // Raise body temp by 2°C (fever)
    float CurrentTemp = StatComp->GetStatValue(EStatType::BodyTemperature);
    StatComp->SetStatValue(EStatType::BodyTemperature, CurrentTemp + 2.0f);
}
```

---

#### **Wetness**

**Display Name:** Wetness Level  
**Purpose:** How wet the character is  
**Default Value:** 0  
**Default Max:** 100  
**Min:** 0  
**Category:** Environmental

**VALUE MEANINGS:**

| Value | Status | Visual | Effects |
|-------|--------|--------|---------|
| 0 | Completely dry | Dry clothes | No effects |
| 1-25 | Damp | Slight darkening | Minimal |
| 25-50 | Wet | Visibly wet | -2°C felt temperature |
| 50-75 | Soaked | Dripping wet | -5°C felt temp, clothing less effective |
| 75-100 | Drenched | Water dripping | -10°C felt temp, -70% clothing insulation |

**EFFECTS OF WETNESS:**

1. **Temperature:** Feels colder (up to -10°C)
2. **Clothing:** Reduces insulation (up to -70%)
3. **Weight:** Increased encumbrance (+10% at 100 wetness)
4. **Infection:** Wounds more likely to get infected (+50% risk)
5. **Stamina:** Slightly reduced regen (-10% at 100 wetness)

**SOURCES OF WETNESS:**

| Source | Wetness Gain | Time to Soaked |
|--------|--------------|----------------|
| Light Rain | +5/sec | 20 seconds |
| Rain | +15/sec | 7 seconds |
| Heavy Rain | +30/sec | 3.3 seconds |
| Swimming | +50/sec | 2 seconds |
| Wading | +20/sec | 5 seconds |
| Splashed | +15 instant | N/A |

**DRYING METHODS:**

| Method | Wetness Loss | Time to Dry |
|--------|--------------|-------------|
| Standing (outside) | -2/sec | 50 seconds |
| In shelter | -5/sec | 20 seconds |
| Near fire | -15/sec | 6.7 seconds |
| Towel dry | -30 instant | Instant (partial) |
| Change clothes | -100 instant | Instant (full) |

**EXAMPLE CODE:**

```cpp
// Wetness from rain (usually handled by WeatherComponent)
void ACharacter::Tick(float DeltaTime)
{
    // If in rain and not sheltered
    if (bInRain && !bInShelter)
    {
        float RainIntensity = WeatherComp->GetRainIntensity(); // 0-100
        float WetnessGain = (RainIntensity / 100.0f) * 30.0f * DeltaTime;
        StatComp->ApplyStatChange(EStatType::Wetness, WetnessGain, TEXT("Rain"));
    }
    
    // Drying
    else if (bNearFire)
    {
        StatComp->ApplyStatChange(EStatType::Wetness, -15.0f * DeltaTime, TEXT("Drying"));
    }
    else if (bInShelter)
    {
        StatComp->ApplyStatChange(EStatType::Wetness, -5.0f * DeltaTime, TEXT("Drying"));
    }
    
    // Apply wetness penalties
    float WetnessLevel = StatComp->GetStatValue(EStatType::Wetness);
    float TempPenalty = -(WetnessLevel / 100.0f) * 10.0f; // Up to -10°C
    
    // Clothing less effective when wet (handled by WeatherComponent)
}
```

---

### Health-Related Stats (Medical/Condition)

#### **BloodLevel**

**Display Name:** Blood Level  
**Purpose:** Amount of blood in body  
**Default Value:** 100  
**Default Max:** 100  
**Min:** 0  
**Category:** HealthConditions

**VALUE MEANINGS:**

| Value | Status | Effects | Recovery Time | Danger |
|-------|--------|---------|---------------|--------|
| 90-100 | Normal | No effects | - | Safe |
| 70-89 | Minor Loss | Slight weakness | 1 hour | Low |
| 50-69 | Moderate Loss | -15% max stamina, -10% movement | 4 hours | Medium |
| 30-49 | Severe Loss | -30% max stamina, -25% movement, blurred vision | 12 hours | High |
| 10-29 | Critical Loss | -50% max stamina, -40% movement, unconscious soon | 24 hours | Critical |
| 0-9 | Near Death | Unconscious, death in 60 sec | - | Fatal |

**HOW BLOOD IS LOST:**

```cpp
// Blood loss from wounds (body component integration)
void ACharacter::Tick(float DeltaTime)
{
    // Check bleeding body parts
    float TotalBleedRate = 0.0f;
    
    for (int32 i = 0; i < (int32)EBodyPart::MAX; ++i)
    {
        EBodyPart Part = (EBodyPart)i;
        float BleedRate = BodyComp->GetBodyPartBleedingRate(Part);
        TotalBleedRate += BleedRate;
    }
    
    // Apply blood loss
    if (TotalBleedRate > 0.0f)
    {
        StatComp->ApplyStatChange(EStatType::BloodLevel, -TotalBleedRate * DeltaTime, TEXT("Bleeding"));
    }
}
```

**BLOOD REGENERATION:**

Blood regenerates VERY SLOWLY naturally:

```cpp
// Natural blood regeneration: +1 per 10 minutes
StatComp->SetStatRegenerationRate(EStatType::BloodLevel, 1.0f / 600.0f);

// Blood restoration methods:
void ACharacter::UseBandage()
{
    // Stop bleeding but doesn't restore blood
    BodyComp->StopAllBleeding();
}

void ACharacter::UseBloodPack()
{
    // Restores blood directly
    StatComp->ApplyStatChange(EStatType::BloodLevel, 30.0f, TEXT("BloodPack"));
}

void ACharacter::GetBloodTransfusion()
{
    // Full restoration
    StatComp->SetStatValue(EStatType::BloodLevel, 100.0f);
}
```

---

#### **HeartRate**

**Display Name:** Heart Rate (BPM)  
**Purpose:** Pulse / cardiovascular indicator  
**Default Value:** 70  
**Default Max:** 220 (theoretical max)  
**Min:** 30 (dangerous)  
**Normal Range:** 60-100 BPM  
**Category:** HealthConditions

**VALUE MEANINGS:**

| BPM | Status | Cause | Effects |
|-----|--------|-------|---------|
| 30-40 | Bradycardia | Blood loss, hypothermia | Dangerous, medical attention |
| 40-60 | Very Low | Athletic, resting | Very fit individuals |
| 60-80 | Normal Resting | Healthy resting state | None |
| 80-100 | Elevated Resting | Slight stress, alert | Minor |
| 100-120 | Elevated | Walking, stress | Slight stamina drain |
| 120-140 | Exercise | Jogging, combat | Moderate stamina drain |
| 140-160 | High Exercise | Sprinting, intense combat | High stamina drain |
| 160-180 | Very High | Extreme exertion | Very high drain |
| 180+ | Dangerous | Cardiac stress, panic | Risk of heart attack |

**WHAT AFFECTS HEART RATE:**

```cpp
void ACharacter::UpdateHeartRate(float DeltaTime)
{
    float BaseHeartRate = 70.0f; // Resting
    float HeartRateModifier = 0.0f;
    
    // Activity
    if (bIsSprinting)
        HeartRateModifier += 70.0f; // +70 BPM when sprinting
    else if (bIsJogging)
        HeartRateModifier += 40.0f;
    else if (bIsWalking)
        HeartRateModifier += 20.0f;
    
    // Combat
    if (bInCombat)
        HeartRateModifier += 30.0f;
    
    // Injury
    float HealthPercent = StatComp->GetStatPercentage(EStatType::Health_Core);
    if (HealthPercent < 0.5f)
        HeartRateModifier += 30.0f; // Injured: +30 BPM
    
    // Blood loss (heart compensates)
    float BloodPercent = StatComp->GetStatPercentage(EStatType::BloodLevel);
    if (BloodPercent < 0.7f)
        HeartRateModifier += (1.0f - BloodPercent) * 50.0f;
    
    // Fear/stress
    if (bIsFrightened)
        HeartRateModifier += 40.0f;
    
    // Calculate target heart rate
    float TargetHeartRate = BaseHeartRate + HeartRateModifier;
    
    // Smoothly interpolate to target
    float CurrentHeartRate = StatComp->GetStatValue(EStatType::HeartRate);
    float NewHeartRate = FMath::FInterpTo(CurrentHeartRate, TargetHeartRate, DeltaTime, 2.0f);
    
    StatComp->SetStatValue(EStatType::HeartRate, NewHeartRate);
}
```

**AUDIO FEEDBACK:**

Heart rate is perfect for audio feedback:

```cpp
void ACharacter::UpdateHeartbeatSound()
{
    float HeartRate = StatComp->GetStatValue(EStatType::HeartRate);
    float BeatsPerSecond = HeartRate / 60.0f;
    
    // Adjust heartbeat sound interval
    HeartbeatSoundInterval = 1.0f / BeatsPerSecond;
    
    // Adjust volume based on stress
    if (HeartRate > 140.0f)
        HeartbeatVolume = 1.0f; // Loud
    else if (HeartRate > 100.0f)
        HeartbeatVolume = 0.6f;
    else
        HeartbeatVolume = 0.2f; // Quiet
}
```

---


---

## Weather System Overview

The Weather System is StatSystemPro's most advanced feature. It simulates realistic environmental conditions that affect character survival through temperature, clothing, wetness, and progressive damage stages.

### What Does The Weather System Do?

```
┌──────────────────────────────────────────────────────────┐
│                 Weather System Core Loop                  │
├──────────────────────────────────────────────────────────┤
│                                                           │
│  1. Set Weather Type (or use Preset)                     │
│     ↓                                                     │
│  2. Calculate Effective Temperature                       │
│     - Ambient temperature                                 │
│     - Wind chill                                         │
│     - Wetness penalty                                     │
│     - Clothing protection                                 │
│     - Shelter bonus                                       │
│     ↓                                                     │
│  3. Determine Temperature Stage                           │
│     - Freezing stages (5 levels)                         │
│     - Overheating stages (5 levels)                      │
│     ↓                                                     │
│  4. Apply Damage & Effects                                │
│     - Progressive damage based on stage                   │
│     - Update BodyTemperature stat                        │
│     - Broadcast events                                    │
│                                                           │
└──────────────────────────────────────────────────────────┘
```

---

## All Weather Types Explained

### 13 Weather Types Available

#### **Clear / Sunny**
**When to use:** Normal, safe conditions  
**Ambient Temperature:** 20-25°C (68-77°F)  
**Wind Speed:** 0-5 m/s  
**Precipitation:** None  
**Visibility:** 100%

**Effect on Survival:** None - ideal conditions

**Example:**
```cpp
WeatherComp->SetWeatherType(EWeatherType::Clear);
WeatherComp->SetAmbientTemperature(22.0f); // Pleasant
WeatherComp->SetWindSpeed(2.0f); // Light breeze
```

---

#### **Cloudy**
**When to use:** Overcast but safe  
**Ambient Temperature:** 15-20°C (59-68°F)  
**Wind Speed:** 5-10 m/s  
**Precipitation:** None  
**Visibility:** 95%

**Effect on Survival:** Minimal - slightly cooler

---

#### **Overcast**
**When to use:** Gloomy, pre-storm  
**Ambient Temperature:** 10-18°C (50-64°F)  
**Wind Speed:** 8-15 m/s  
**Precipitation:** None  
**Visibility:** 85%

**Effect on Survival:** Getting cold, need light jacket

---

#### **Light Rain**
**When to use:** Drizzle, light shower  
**Ambient Temperature:** 12-18°C (54-64°F)  
**Wind Speed:** 5-10 m/s  
**Rain Intensity:** 20%  
**Precipitation:** Light  
**Visibility:** 80%

**Effect on Survival:**
- Clothing gets wet slowly (+5 wetness/sec)
- Slight temperature drop
- Visibility reduced slightly

**Example:**
```cpp
WeatherComp->SetWeatherType(EWeatherType::LightRain);
WeatherComp->SetAmbientTemperature(15.0f);
WeatherComp->SetRainIntensity(20.0f);
// Character slowly gets wet, clothing loses effectiveness
```

---

#### **Rain**
**When to use:** Standard rainfall  
**Ambient Temperature:** 10-16°C (50-61°F)  
**Wind Speed:** 10-15 m/s  
**Rain Intensity:** 60%  
**Precipitation:** Moderate  
**Visibility:** 70%

**Effect on Survival:**
- Clothing gets wet quickly (+15 wetness/sec)
- Temperature feels 3-5°C colder due to wetness
- Reduced visibility
- Need waterproof clothing

**Wetness Impact:**
```
Time to soaked (no rain protection):
- Water-resistant clothing: ~15 seconds
- Normal clothing: ~7 seconds  
- No protection: ~3 seconds
```

---

#### **Heavy Rain**
**When to use:** Downpour, severe storm  
**Ambient Temperature:** 8-14°C (46-57°F)  
**Wind Speed:** 15-25 m/s  
**Rain Intensity:** 90%  
**Precipitation:** Heavy  
**Visibility:** 50%

**Effect on Survival:**
- Clothing soaked almost instantly (+30 wetness/sec)
- Temperature feels 7-10°C colder
- Heavily reduced visibility
- Need shelter or waterproof gear
- Hypothermia risk even in moderate ambient temps

**Real Example:**
```
Ambient: 12°C
Wetness at 100%: -10°C penalty
No shelter: 0°C bonus
Inadequate clothing: +5°C bonus
Effective Temperature: 7°C → Approaching hypothermia!
```

---

#### **Thunderstorm**
**When to use:** Severe weather event  
**Ambient Temperature:** 15-20°C (59-68°F)  
**Wind Speed:** 20-30 m/s  
**Rain Intensity:** 85%  
**Precipitation:** Intense  
**Lightning:** Yes  
**Visibility:** 40%

**Effect on Survival:**
- Rapid wetness accumulation
- High wind chill
- Lightning strike risk
- Severe visibility reduction
- Dangerous even with good gear

**Preset:**
```cpp
FWeatherPreset Storm = WeatherComp->GetThunderstormPreset();
// Ambient: 18°C
// Wind: 20 m/s
// Rain: 85%
WeatherComp->ApplyWeatherPreset(Storm);
```

---

#### **Light Snow**
**When to use:** Light snowfall  
**Ambient Temperature:** -2 to 2°C (28-36°F)  
**Wind Speed:** 3-8 m/s  
**Snow Intensity:** 30%  
**Precipitation:** Light  
**Visibility:** 85%

**Effect on Survival:**
- Slow wetness from melting snow
- Cold but manageable
- Light clothing wetness (+8 wetness/sec)

---

#### **Snow**
**When to use:** Standard snowfall  
**Ambient Temperature:** -8 to 0°C (18-32°F)  
**Wind Speed:** 8-15 m/s  
**Snow Intensity:** 60%  
**Precipitation:** Moderate  
**Visibility:** 70%

**Effect on Survival:**
- Moderate wetness (+15 wetness/sec)
- Cold - need winter clothing
- Reduced visibility
- Hypothermia risk without proper gear

---

#### **Heavy Snow / Blizzard**
**When to use:** Severe winter storm  
**Ambient Temperature:** -20 to -10°C (-4 to 14°F)  
**Wind Speed:** 25-40 m/s  
**Snow Intensity:** 95%  
**Precipitation:** Extreme  
**Visibility:** 20-30%

**Effect on Survival:**
- EXTREMELY DANGEROUS
- Rapid hypothermia without arctic gear
- High wetness from snow (+25 wetness/sec)
- Severe wind chill (-10 to -15°C)
- Nearly blind (30% visibility)

**Preset:**
```cpp
FWeatherPreset Blizzard = WeatherComp->GetBlizzardPreset();
// Ambient: -15°C
// Wind: 25 m/s
// Snow: 90%
WeatherComp->ApplyWeatherPreset(Blizzard);
```

**Survival Requirements:**
- High cold insulation (80+)
- High wind resistance (75+)
- Waterproof outer layer (90+)
- Shelter absolutely essential
- Will die in minutes without proper gear

---

#### **Fog**
**When to use:** Reduced visibility  
**Ambient Temperature:** 8-15°C (46-59°F)  
**Wind Speed:** 0-3 m/s  
**Fog Density:** 80%  
**Visibility:** 30%

**Effect on Survival:**
- High humidity (wetness slowly increases)
- Cool but not dangerous
- Severely reduced visibility
- Navigation difficulty

---

#### **Sandstorm**
**When to use:** Desert storm  
**Ambient Temperature:** 35-45°C (95-113°F)  
**Wind Speed:** 20-35 m/s  
**Sand Density:** 85%  
**Visibility:** 25%

**Effect on Survival:**
- EXTREMELY HOT
- High wind but no cooling (hot wind!)
- Severe visibility reduction
- Heatstroke risk
- Breathing difficulty
- Need face/eye protection

**Special mechanics:**
- Clothing durability loss (sand abrasion)
- Thirst increases 2x faster
- Cannot recover stamina effectively

---

#### **Heatwave**
**When to use:** Extreme heat  
**Ambient Temperature:** 40-50°C (104-122°F)  
**Wind Speed:** 0-5 m/s  
**Humidity:** Low  
**Visibility:** 100%

**Effect on Survival:**
- EXTREMELY DANGEROUS
- Rapid overheating
- Severe heatstroke risk
- Thirst skyrockets
- Need shade/shelter immediately

**Preset:**
```cpp
FWeatherPreset Heatwave = WeatherComp->GetHeatwavePreset();
// Ambient: 42°C
// Wind: 3 m/s (no relief)
WeatherComp->ApplyWeatherPreset(Heatwave);
```

**Survival Requirements:**
- LOW heat insulation clothing (< 20)
- Breathable fabrics
- Shade/shelter essential
- Constant hydration
- Will die in 30-60 minutes without water

**CRITICAL:** Do NOT wear winter clothing in heatwave!
```
Heatwave ambient: 42°C
Winter jacket (85 heat insulation): +8.5°C penalty
Effective temperature: 50.5°C → FATAL in minutes!
```

---

## Temperature Values & What They Mean

### Understanding Temperature Scales

#### **Ambient Temperature** (Environmental Base)

This is the **air temperature** set by weather or manually. It's the starting point for all calculations.

**Temperature Ranges:**

| Range (°C) | Range (°F) | Environment | Danger Level |
|------------|-----------|-------------|--------------|
| -40 to -30 | -40 to -22 | Extreme Arctic | FATAL without shelter |
| -30 to -20 | -22 to -4 | Arctic/Winter | Critical - frostbite in minutes |
| -20 to -10 | -4 to 14 | Severe Winter | Dangerous - need heavy gear |
| -10 to 0 | 14 to 32 | Cold Winter | Cold - winter clothing required |
| 0 to 10 | 32 to 50 | Cool | Cool - jacket recommended |
| 10 to 20 | 50 to 68 | Mild | Comfortable |
| 20 to 30 | 68 to 86 | Warm | Pleasant |
| 30 to 35 | 86 to 95 | Hot | Warm - light clothing |
| 35 to 40 | 95 to 104 | Very Hot | Hot - shade needed |
| 40 to 45 | 104 to 113 | Extreme Heat | Dangerous - heatstroke risk |
| 45 to 50 | 113 to 122 | Desert Heat | Critical - survival measured in hours |
| 50+ | 122+ | Lethal Heat | FATAL - death in minutes |

**Real World Comparison:**

```
-30°C (-22°F) = Siberian winter
-15°C (5°F) = Minnesota winter
0°C (32°F) = Freezing point of water
10°C (50°F) = Cool spring day
20°C (68°F) = Room temperature, comfortable
30°C (86°F) = Hot summer day
40°C (104°F) = Death Valley, USA
50°C (122°F) = Kuwait summer extreme
```

**Code Example:**

```cpp
// Set ambient temperature based on biome
void AGameMode::SetBiomeWeather(EBiomeType Biome)
{
    switch (Biome)
    {
    case EBiomeType::Arctic:
        WeatherComp->SetAmbientTemperature(-25.0f); // Extreme cold
        break;
    case EBiomeType::Temperate:
        WeatherComp->SetAmbientTemperature(18.0f); // Comfortable
        break;
    case EBiomeType::Desert:
        WeatherComp->SetAmbientTemperature(42.0f); // Extreme heat
        break;
    case EBiomeType::Jungle:
        WeatherComp->SetAmbientTemperature(30.0f); // Hot & humid
        break;
    }
}
```

---

## Wind Speed, Shelter, & Environmental Factors

### Wind Speed (meters per second)

Wind speed creates **wind chill**, making cold weather feel MUCH colder.

**Wind Speed Values:**

| Speed (m/s) | Speed (mph) | Speed (km/h) | Description | Wind Chill Impact |
|-------------|------------|--------------|-------------|-------------------|
| 0-1 | 0-2 | 0-4 | Calm | None |
| 1-5 | 2-11 | 4-20 | Light breeze | Minimal (-1°C) |
| 5-10 | 11-22 | 20-36 | Gentle breeze | Slight (-2 to -3°C) |
| 10-15 | 22-34 | 36-54 | Moderate wind | Moderate (-4 to -6°C) |
| 15-20 | 34-45 | 54-72 | Strong wind | Strong (-7 to -9°C) |
| 20-25 | 45-56 | 72-90 | Very strong | Very strong (-10°C) |
| 25-30 | 56-67 | 90-108 | Storm | Severe (-12°C) |
| 30-40 | 67-89 | 108-144 | Violent storm | Extreme (-15°C) |
| 40+ | 89+ | 144+ | Hurricane | Lethal (-20°C) |

**Wind Chill Calculation:**

```cpp
// Formula
WindChill = -(WindSpeed / 10.0f) * (1.0f - ClothingWindResistance) * 5.0f

// Examples:
// 20 m/s wind, no protection:
// = -(20/10) * (1-0) * 5 = -10°C
//
// 20 m/s wind, good windbreaker (75% resistance):
// = -(20/10) * (1-0.75) * 5 = -2.5°C
```

**Real Example:**

```
Scenario: Blizzard
Ambient: -15°C
Wind: 25 m/s
No wind protection:
  Wind Chill = -12.5°C
  Effective Temp = -15 + (-12.5) = -27.5°C (SEVERE!)

With windproof parka (90% wind resistance):
  Wind Chill = -1.25°C
  Effective Temp = -15 + (-1.25) = -16.25°C (Still cold but survivable)
```

---

### Shelter Level (0-100%)

Shelter protects from weather effects.

**Shelter Values:**

| Level | Location | Protection | Temp Bonus | Effects Blocked |
|-------|----------|------------|-----------|-----------------|
| 0% | Open field | None | 0°C | None |
| 25% | Tree cover | Minimal | +1.25°C | Light rain partially |
| 50% | Cave / Overhang | Moderate | +2.5°C | Rain, some wind |
| 75% | Building (damaged) | Good | +3.75°C | Most weather |
| 100% | Building (intact) | Full | +5°C | All weather |

**Code Example:**

```cpp
// Dynamic shelter detection
void ACharacter::UpdateShelterLevel()
{
    float ShelterLevel = 0.0f;
    
    // Raycast upward to detect cover
    FHitResult Hit;
    FVector Start = GetActorLocation();
    FVector End = Start + FVector(0, 0, 1000.0f); // 10 meters up
    
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        float Distance = Hit.Distance;
        
        if (Distance < 200.0f) // Within 2 meters
        {
            // Check if indoors
            if (IsInsideBuilding())
                ShelterLevel = 100.0f; // Full protection
            else
                ShelterLevel = 50.0f; // Overhang/cave
        }
        else if (Distance < 500.0f) // Within 5 meters
        {
            ShelterLevel = 25.0f; // Tree cover
        }
    }
    
    WeatherComp->SetShelterLevel(ShelterLevel);
}
```

**Impact:**

```
Example: Raining, 12°C ambient
Without shelter (0%):
  - Getting wet: +15 wetness/sec
  - Temperature: 12°C
  - Clothing getting soaked

With shelter (100%):
  - No wetness gain
  - Temperature: 12 + 5 = 17°C (shelter bonus)
  - Clothing staying dry
  - Can dry off
```

---


---

## Editor Tools Overview

The StatSystemProEditor module provides **professional-grade editor integration** with custom details panels, quick actions, and visual configuration tools. No more manual value entry - configure everything visually!

### What's Included

✅ **Custom Details Panels** - Enhanced property panels for all components  
✅ **20+ Quick Action Buttons** - One-click testing and configuration  
✅ **Weather Presets** - Instant environment switching  
✅ **Clothing Management** - Quick-equip full gear sets  
✅ **Real-Time Monitoring** - Live stat visualization  
✅ **Testing Workflows** - Structured testing scenarios  
✅ **No Code Required** - Everything visual  

---

## Enhanced Details Panels

When you select a component in the editor, you see **massively enhanced** property panels with organized categories and instant actions.

### StatComponent Enhanced Details

**Location:** Select any actor with StatComponent → Details Panel

#### Categories & Features

##### **1. Quick Actions** (Primary)

| Button | Effect | Use Case |
|--------|--------|----------|
| **Restore All Stats** | Sets all stats to 100% | Reset after testing |
| **Deplete All Stats** | Sets all stats to 0% | Test death/critical states |
| **Randomize Stats** | Random values 0-100% | Variety testing |
| **Reset Regeneration** | Clears all regen rates | Fresh start |
| **Set All to 50%** | Half health on everything | Mid-game scenario |
| **Test Critical (15%)** | Health to 15% | Critical state testing |
| **Test Near Death (5%)** | Health to 5% | Death sequence testing |

**Blueprint Equivalent:**
```
Quick Action Button = One Click
vs.
Manual Method:
  1. Find stat in list
  2. Expand stat details
  3. Type new value
  4. Repeat for each stat (18 times!)
```

##### **2. Stat Testing**

| Button | Effect | Use Case |
|--------|--------|----------|
| **Test Starvation** | Hunger to 95%, Thirst to 90% | Survival testing |
| **Test Dehydration** | Thirst to 95%, Hunger to 50% | Dehydration sequence |
| **Test Exhaustion** | Fatigue to 95%, Stamina to 10% | Sleep system testing |
| **Test Hypothermia** | BodyTemp to 32°C | Cold damage testing |
| **Test Heatstroke** | BodyTemp to 40°C | Heat damage testing |
| **Test Blood Loss** | BloodLevel to 30% | Bleeding system |
| **Test Poisoned** | Toxicity to 75% | Poison mechanics |
| **Test Infected** | InfectionLevel to 60% | Infection progression |

**Advanced Testing:**

```cpp
// Behind the scenes, these buttons do:

void OnTestStarvationClicked()
{
    StatComp->SetStatValue(EStatType::Hunger, 95.0f);
    StatComp->SetStatValue(EStatType::Thirst, 90.0f);
    
    // Also simulate time passage effects
    StatComp->SetStatValue(EStatType::Fatigue, 70.0f);
    StatComp->SetStatValue(EStatType::Stamina, 30.0f);
    
    UE_LOG(LogTemp, Warning, TEXT("STARVATION TEST: Character is starving and dehydrated!"));
}
```

##### **3. Stat Category Operations**

NEW CATEGORY! Bulk operations on stat groups:

| Button | Effect | Stats Affected |
|--------|--------|----------------|
| **Restore Core Stats** | Set to 100% | Health, Stamina, Energy |
| **Deplete Core Stats** | Set to 0% | Health, Stamina, Energy |
| **Reset Survival Stats** | Set to 0 (fed/hydrated) | Hunger, Thirst, Fatigue |
| **Max Survival Stats** | Set to 100 (starving) | Hunger, Thirst, Fatigue |
| **Normalize Environmental** | Reset to defaults | BodyTemp (37°C), Wetness (0) |
| **Critical Health Conditions** | Set to dangerous levels | BloodLevel (30%), Toxicity (75%), Infection (60%) |

##### **4. Regeneration Quick Setup**

NEW! Instant regeneration configuration:

| Preset | Regen Rates | Use Case |
|--------|-------------|----------|
| **No Regeneration** | All 0/sec | Hardcore survival |
| **Slow Regeneration** | Health +1/sec, Stamina +5/sec | Realistic |
| **Normal Regeneration** | Health +2/sec, Stamina +10/sec | Standard game |
| **Fast Regeneration** | Health +5/sec, Stamina +20/sec | Casual mode |
| **Instant Regeneration** | Health +50/sec, Stamina +50/sec | Testing/dev mode |

##### **5. Stat Visualization** (NEW!)

Real-time stat monitoring **WHILE IN PIE** (Play In Editor):

```
┌───────────────── Stat Monitor ─────────────────┐
│                                                 │
│  Health_Core:    [████████░░] 85%  (-2/sec)   │
│  Stamina:        [████░░░░░░] 45%  (+10/sec)   │
│  Hunger:         [███░░░░░░░] 35%  (+0.003/sec)│
│  Thirst:         [████░░░░░░] 42%  (+0.008/sec)│
│  BodyTemp:       37.2°C (Normal)               │
│  BloodLevel:     [█████████░] 92%  (Healthy)   │
│                                                 │
│  [Pause Regen] [Reset All] [Log Values]        │
└─────────────────────────────────────────────────┘
```

**Features:**
- Live updating bars
- Shows regen/decay rates
- Color-coded (green=safe, yellow=warning, red=critical)
- Click stat to freeze and inspect
- Export values to log

---

### BodyComponent Enhanced Details

**Location:** Select actor with BodyComponent → Details Panel

#### Categories & Features

##### **1. Quick Actions**

| Button | Effect | Visual Result |
|--------|--------|---------------|
| **Heal All Body Parts** | All parts to 100% | Clean character |
| **Damage All Body Parts (50%)** | All parts to 50% | Moderately wounded |
| **Damage All Body Parts (Critical 20%)** | All parts to 20% | Severely wounded |
| **Destroy Random Body Part** | One part to 0% | Missing limb simulation |

##### **2. Body Part Testing**

| Button | Effect | Gameplay Impact |
|--------|--------|-----------------|
| **Fracture Head** | Break head | Sanity drain, vision blur |
| **Fracture Torso** | Break torso | Health/stamina max reduced |
| **Fracture Left Arm** | Break left arm | Can't use two-handed weapons |
| **Fracture Right Arm** | Break right arm | Accuracy severely reduced |
| **Fracture Left Leg** | Break left leg | 70% movement speed loss, limping |
| **Fracture Right Leg** | Break right leg | 70% movement speed loss, limping |
| **Fracture Random Part** | Random fracture | Surprise testing |
| **Fracture All Limbs** | Break all 4 limbs | Total disability |

##### **3. Bleeding Management**

| Button | Effect | Time to Death |
|--------|--------|---------------|
| **Start Minor Bleeding (5/sec)** | Slow blood loss | ~20 minutes |
| **Start Moderate Bleeding (10/sec)** | Medium blood loss | ~10 minutes |
| **Start Severe Bleeding (20/sec)** | Fast blood loss | ~5 minutes |
| **Start Critical Bleeding (40/sec)** | Arterial bleeding | ~2.5 minutes |
| **Random Bleeding (2-3 parts)** | Multiple wounds | Varies |
| **Stop All Bleeding** | Instant stop | Survival |

##### **4. Injury Scenarios** (NEW!)

Pre-configured injury patterns:

| Scenario | Injuries | Use Case |
|----------|----------|----------|
| **Car Crash** | Torso 30%, Both Legs 40%, Head 20% | Vehicle accident |
| **Fall Damage** | Both Legs fractured, 60% condition | Falling test |
| **Gunshot Wound** | Torso 10%, Bleeding 15/sec | Combat injury |
| **Explosion** | All parts 40%, Burns, Bleeding | Explosive damage |
| **Animal Attack** | Random part 20%, Multiple bleeds | Wildlife encounter |
| **Melee Combat** | Arm 30%, Torso 25%, bleeding | Sword fight |

**Example:**
```
Click "Car Crash" scenario:
  → Torso condition: 70 → 40 (-30% damage)
  → Left Leg: 60 → 20 (-40% damage, FRACTURED)
  → Right Leg: 60 → 20 (-40% damage, FRACTURED)
  → Head: 80 → 60 (-20% damage)
  → Character can barely move, in severe pain
  → Perfect for testing emergency medical system
```

##### **5. Body Part Visualization** (NEW!)

Interactive body diagram:

```
        ┌─────┐
        │ HEAD│  [85%] 
        └──┬──┘
           │
    ┌──────┴──────┐
    │    TORSO    │  [70%] (Bleeding: 5/sec)
    └──┬───────┬──┘
       │       │
   ┌───┴──┐ ┌──┴───┐
   │L.ARM │ │R.ARM │  [50%] (FRACTURED)
   └──────┘ └──────┘
       │       │
   ┌───┴──┐ ┌──┴───┐
   │L.LEG │ │R.LEG │  [30%]
   └──────┘ └──────┘

Click any body part to:
- See detailed stats
- Apply damage
- Fracture/heal
- Start/stop bleeding
```

---

### WeatherComponent Enhanced Details

**Location:** Select actor with WeatherComponent → Details Panel

This is the MOST ENHANCED panel with tons of features!

#### Categories & Features

##### **1. Weather Presets** (Instant Switching)

| Preset | Temperature | Conditions | Survival Time |
|--------|-------------|------------|---------------|
| **Clear & Sunny** | 22°C | Perfect | Indefinite |
| **Mild Rain** | 15°C, Light rain | Comfortable | Hours |
| **Heavy Thunderstorm** | 18°C, 85% rain, 20 m/s wind | Challenging | 1-2 hours |
| **Blizzard** | -15°C, 90% snow, 25 m/s wind | EXTREME | Minutes without gear |
| **Heatwave** | 42°C | EXTREME | Minutes without water |
| **Sandstorm** | 38°C, 85% sand, 30 m/s wind | EXTREME | 30 min |
| **Arctic Extreme** | -35°C, 95% snow, 35 m/s wind | LETHAL | Seconds without shelter |
| **Desert Extreme** | 50°C | LETHAL | Seconds without water |
| **Mild Spring Day** | 18°C, Cloudy | Pleasant | Indefinite |
| **Autumn Rain** | 12°C, 60% rain | Cool | Hours |
| **Foggy Morning** | 10°C, Fog | Cool | Hours |
| **Tropical Storm** | 28°C, 90% rain, 25 m/s wind | Challenging | 1-2 hours |

**NEW PRESETS!** (Beyond the original 4)

##### **2. Clothing Quick-Equip**

**Instant Gear Sets:**

| Loadout | Contents | Use Case |
|---------|----------|----------|
| **Naked** | No clothing | Test worst-case |
| **Underwear Only** | Minimal coverage | Nearly naked test |
| **Summer Casual** | T-shirt, shorts, sandals | Hot weather |
| **Spring Casual** | Light jacket, jeans | Mild weather |
| **Autumn Outfit** | Jacket, pants, boots | Cool weather |
| **Winter Gear** | Fur hat, parka, insulated pants, boots, gloves | Cold weather |
| **Arctic Expedition** | MAXIMUM insulation (95+ all slots) | Extreme cold |
| **Desert Robe** | Breathable, low heat insulation | Hot desert |
| **Rain Gear** | Waterproof everything | Rain protection |
| **Tactical Gear** | Balanced protection | Combat/exploration |

**Clothing Stats Display:**

```
Current Loadout: Winter Gear
┌──────────────────────────────────────────────┐
│ HEAD:   Fur Hat                              │
│         Cold: 70  Heat: 60  Water: 50        │
│         Wind: 80  Wet: 0%   Durability: 100% │
├──────────────────────────────────────────────┤
│ TORSO:  Arctic Parka                         │
│         Cold: 95  Heat: 90  Water: 85        │
│         Wind: 95  Wet: 0%   Durability: 100% │
├──────────────────────────────────────────────┤
│ LEGS:   Insulated Pants                      │
│         Cold: 75  Heat: 70  Water: 60        │
│         Wind: 75  Wet: 0%   Durability: 100% │
├──────────────────────────────────────────────┤
│ FEET:   Winter Boots                         │
│         Cold: 80  Heat: 75  Water: 90        │
│         Wind: 85  Wet: 0%   Durability: 100% │
├──────────────────────────────────────────────┤
│ HANDS:  Insulated Gloves                     │
│         Cold: 65  Heat: 60  Water: 70        │
│         Wind: 80  Wet: 0%   Durability: 100% │
└──────────────────────────────────────────────┘
Total Cold Insulation: 385/500 (77%)
Total Wind Resistance: 415/500 (83%)
Estimated Survival in Blizzard: 45 minutes
```

##### **3. Environmental Control** (NEW!)

Precise control over all environmental factors:

**Temperature Slider:**
- Range: -50°C to +60°C
- Presets: Arctic (-40°C), Cold (-10°C), Cool (5°C), Mild (15°C), Warm (25°C), Hot (35°C), Extreme (45°C)
- Live preview: "At -15°C with current clothing: Effective Temp = -8°C (COLD)"

**Wind Speed Slider:**
- Range: 0-50 m/s
- Presets: Calm (0), Breeze (5), Wind (15), Strong (25), Storm (35), Hurricane (45)
- Live preview: "25 m/s wind creates -10°C wind chill"

**Rain/Snow Intensity Slider:**
- Range: 0-100%
- Presets: None (0%), Light (20%), Moderate (50%), Heavy (80%), Extreme (95%)
- Live preview: "At 80% rain, clothing soaked in 8 seconds"

**Shelter Level Slider:**
- Range: 0-100%
- Presets: Outside (0%), Tree (25%), Cave (50%), Building (75%), Indoors (100%)
- Live preview: "50% shelter provides +2.5°C protection"

**Wetness Control:**
- Manual wetness adjustment
- "Soak All" button (instant 100%)
- "Dry All" button (instant 0%)
- "Gradual Soak" (increases over 30 sec)
- "Gradual Dry" (decreases over 30 sec)

##### **4. Testing Scenarios** (NEW!)

Pre-configured survival challenges:

| Scenario | Setup | Challenge | Expected Outcome |
|----------|-------|-----------|------------------|
| **Survival Test: Easy** | 10°C, Light rain, Good clothes | Stay alive 10 min | Should survive easily |
| **Survival Test: Medium** | -5°C, Rain, Moderate clothes | Stay alive 10 min | Challenging but possible |
| **Survival Test: Hard** | -15°C, Blizzard, Poor clothes | Stay alive 5 min | Very difficult |
| **Survival Test: Extreme** | -30°C, Blizzard, Naked | Stay alive 2 min | Nearly impossible |
| **Heat Challenge: Easy** | 35°C, Shade, Light clothes | Stay alive 10 min | Should survive |
| **Heat Challenge: Hard** | 45°C, No shade, Heavy clothes | Stay alive 5 min | Difficult |
| **Wet & Cold** | 5°C, Rain, No waterproof | Stay alive 10 min | Get hypothermia |
| **Perfect Storm** | -20°C, Blizzard, High wind, Naked | Stay alive 1 min | Guaranteed death |

**Scenario Workflow:**
```
1. Click "Survival Test: Hard"
2. Character spawns with:
   - Temperature: -15°C
   - Weather: Blizzard
   - Clothing: T-shirt and jeans only
3. Timer starts: 5:00 minutes
4. Watch character:
   - Gets wet immediately
   - Body temp drops fast
   - Enters Freezing stage at 1:30
   - Enters Hypothermia at 3:00
   - Health critical at 4:30
   - Likely death before timer ends
5. Results screen:
   - Survived: 4:42
   - Cause of death: Hypothermia
   - Max freezing stage: Hypothermia
   - Lowest body temp: 31.2°C
```

##### **5. Weather Effects Monitor** (NEW!)

Real-time calculation display:

```
┌────────── Temperature Breakdown ──────────┐
│                                           │
│ Ambient Temperature:        -15.0°C      │
│ + Wind Chill:               -10.0°C      │
│ + Wetness Penalty:           -7.5°C      │
│ + Clothing Protection:      +12.0°C      │
│ + Shelter Bonus:             +0.0°C      │
│ ─────────────────────────────────────    │
│ = Effective Temperature:     -20.5°C     │
│                                           │
│ Current Stage: FREEZING (Stage 3)        │
│ Damage Rate: -2.0 HP/sec                 │
│ Time to Death: ~3 minutes                │
│                                           │
│ ⚠️ WARNING: Hypothermia imminent!        │
│                                           │
└───────────────────────────────────────────┘

Clothing Status:
  Total Wetness: 75% (SOAKED)
  Effective Cold Insulation: 28.5 (was 95)
  Insulation Loss: 70% due to wetness!

Recommendations:
  🔥 Find shelter immediately
  🧥 Change to dry clothing
  ☕ Drink hot beverage
```

---


## StatComponent Complete Function Reference

### Core Stat Operations

#### **GetStatValue**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
float GetStatValue(EStatType StatType) const;
```

**Purpose:** Get the current value of a stat

**Parameters:**
- `StatType` - Which stat to query (dropdown in Blueprint)

**Returns:** Current value (float)

**Example:**
```cpp
float Health = StatComp->GetStatValue(EStatType::Health_Core);
if (Health < 20.0f)
{
    PlayLowHealthWarning();
}
```

**Blueprint:**
```
Get Stat Value (Health_Core) → Float
```

---

#### **SetStatValue**

```cpp
UFUNCTION(BlueprintCallable, Category = "Stats")
void SetStatValue(EStatType StatType, float NewValue);
```

**Purpose:** Directly set a stat to a specific value

**Parameters:**
- `StatType` - Which stat to set
- `NewValue` - New value to set (will be clamped to min/max)

**When to use:**
- Setting initial values
- Cheat codes
- Loading from save game
- Editor testing

**Example:**
```cpp
// Set health to full
StatComp->SetStatValue(EStatType::Health_Core, 100.0f);

// Set to critical health for testing
StatComp->SetStatValue(EStatType::Health_Core, 10.0f);
```

**NOTE:** Use `ApplyStatChange` for gameplay changes (respects rules, broadcasts events properly)

---

#### **ApplyStatChange**

```cpp
UFUNCTION(BlueprintCallable, Category = "Stats")
void ApplyStatChange(EStatType StatType, float Amount, const FString& Source, FGameplayTag SourceTag = FGameplayTag());
```

**Purpose:** Modify a stat by an amount (damage, healing, etc.)

**Parameters:**
- `StatType` - Which stat to modify
- `Amount` - How much to change (negative = decrease, positive = increase)
- `Source` - Description for debugging (e.g., "Bullet", "HealthPack")
- `SourceTag` - Optional gameplay tag for categorization

**Returns:** Nothing (broadcasts events)

**Example:**
```cpp
// Take 25 damage
StatComp->ApplyStatChange(EStatType::Health_Core, -25.0f, TEXT("Bullet"));

// Heal 30 HP
StatComp->ApplyStatChange(EStatType::Health_Core, 30.0f, TEXT("HealthPack"));

// Reduce hunger by eating
StatComp->ApplyStatChange(EStatType::Hunger, -40.0f, TEXT("CookedMeat"));
```

**Events Broadcast:**
- `OnStatChanged` - Always (unless amount is 0)
- `OnStatReachedZero` - If stat hits 0
- `OnStatReachedMax` - If stat hits max

---

#### **GetStatPercentage**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
float GetStatPercentage(EStatType StatType) const;
```

**Purpose:** Get stat as percentage (0.0 to 1.0)

**Returns:** Value from 0.0 (0%) to 1.0 (100%)

**Example:**
```cpp
float HealthPercent = StatComp->GetStatPercentage(EStatType::Health_Core);

if (HealthPercent < 0.15f) // Less than 15%
{
    // Critical health!
    ShowCriticalHealthUI();
}
else if (HealthPercent < 0.5f) // Less than 50%
{
    // Low health warning
    ShowLowHealthUI();
}
```

**Use for UI:**
```cpp
// Update health bar
HealthBar->SetPercent(StatComp->GetStatPercentage(EStatType::Health_Core));
```

---

#### **GetStatMaxValue**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
float GetStatMaxValue(EStatType StatType) const;
```

**Purpose:** Get the maximum value for a stat

**Example:**
```cpp
float MaxHealth = StatComp->GetStatMaxValue(EStatType::Health_Core); // Usually 100
float CurrentHealth = StatComp->GetStatValue(EStatType::Health_Core);

// Calculate how much healing is needed for full
float HealingNeeded = MaxHealth - CurrentHealth;
```

---

#### **SetStatMaxValue**

```cpp
UFUNCTION(BlueprintCallable, Category = "Stats")
void SetStatMaxValue(EStatType StatType, float NewMax);
```

**Purpose:** Change the maximum value for a stat

**Use cases:**
- Leveling up (increasing max health)
- Equipment bonuses
- Temporary buffs
- Penalties from injuries

**Example:**
```cpp
// Level up: Increase max health by 10
float CurrentMax = StatComp->GetStatMaxValue(EStatType::Health_Core);
StatComp->SetStatMaxValue(EStatType::Health_Core, CurrentMax + 10.0f);

// Torso injury: Reduce max stamina by 20%
float MaxStamina = StatComp->GetStatMaxValue(EStatType::Stamina);
StatComp->SetStatMaxValue(EStatType::Stamina, MaxStamina * 0.8f);
```

**NOTE:** Changing max value does NOT change current value!
```cpp
// Before: Health = 80/100
StatComp->SetStatMaxValue(EStatType::Health_Core, 150.0f);
// After: Health = 80/150 (current stays same!)
```

---

#### **RestoreAllStatsToMax**

```cpp
UFUNCTION(BlueprintCallable, Category = "Stats")
void RestoreAllStatsToMax();
```

**Purpose:** Set all stats to their maximum values

**Use cases:**
- Full heal ability
- Respawn
- Cheat code
- Save point restoration

**Example:**
```cpp
// Full restoration shrine
void AShrine::OnActivate(ACharacter* Character)
{
    if (UStatComponent* Stats = Character->FindComponentByClass<UStatComponent>())
    {
        Stats->RestoreAllStatsToMax();
        PlayHealingEffect();
        ShowNotification("Fully Restored!");
    }
}
```

---

### Regeneration Functions

#### **SetStatRegenerationRate**

```cpp
UFUNCTION(BlueprintCallable, Category = "Stats")
void SetStatRegenerationRate(EStatType StatType, float RegenRate);
```

**Purpose:** Set how fast a stat regenerates per second

**Parameters:**
- `StatType` - Which stat
- `RegenRate` - Amount per second (positive = regen, negative = decay)

**Example:**
```cpp
// Health regeneration: +2 HP/sec
StatComp->SetStatRegenerationRate(EStatType::Health_Core, 2.0f);

// Stamina regeneration: +15/sec
StatComp->SetStatRegenerationRate(EStatType::Stamina, 15.0f);

// Hunger decay: +1 per 5 minutes = 0.00333/sec
StatComp->SetStatRegenerationRate(EStatType::Hunger, 0.00333f);

// Stop regeneration
StatComp->SetStatRegenerationRate(EStatType::Health_Core, 0.0f);
```

**Dynamic Adjustment:**
```cpp
void ACharacter::Tick(float DeltaTime)
{
    // Stamina regen depends on state
    float StaminaRegen = 15.0f; // Default
    
    if (bIsSprinting)
        StaminaRegen = 0.0f; // No regen while sprinting
    else if (bInCombat)
        StaminaRegen = 5.0f; // Slow regen in combat
    else if (bIsCrouching)
        StaminaRegen = 20.0f; // Faster when resting
    
    StatComp->SetStatRegenerationRate(EStatType::Stamina, StaminaRegen);
}
```

---

#### **GetStatRegenerationRate**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
float GetStatRegenerationRate(EStatType StatType) const;
```

**Purpose:** Get current regeneration rate

**Example:**
```cpp
float HealthRegen = StatComp->GetStatRegenerationRate(EStatType::Health_Core);
if (HealthRegen > 0.0f)
{
    ShowRegenIcon(); // Show regenerating icon in UI
}
```

---

#### **SetStatRegenerationCurve**

```cpp
UFUNCTION(BlueprintCallable, Category = "Stats")
void SetStatRegenerationCurve(EStatType StatType, UCurveFloat* Curve);
```

**Purpose:** Use a curve for non-linear regeneration

**Parameters:**
- `StatType` - Which stat
- `Curve` - Float curve asset (X = percentage 0-1, Y = regen rate)

**Curve Explanation:**
```
X-Axis: Current stat percentage (0.0 = empty, 1.0 = full)
Y-Axis: Regeneration amount per second

Example health regen curve:
At 0% health (0.0):    Y = 10.0  (fast regen when low)
At 50% health (0.5):   Y = 5.0   (medium regen)
At 100% health (1.0):  Y = 0.0   (no regen when full)
```

**Use cases:**
- Faster healing at low health
- Slower stamina regen when depleted
- Exponential recovery curves

**Example:**
```cpp
// Create curve in editor:
// Content Browser → Right-click → Miscellaneous → Curve
// Name: HealthRegenCurve
// Set points:
//   (0.0, 15.0)  - 15 HP/sec at 0%
//   (0.5, 8.0)   - 8 HP/sec at 50%
//   (1.0, 0.0)   - 0 HP/sec at 100%

// Load and apply
UCurveFloat* RegenCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Curves/HealthRegenCurve"));
StatComp->SetStatRegenerationCurve(EStatType::Health_Core, RegenCurve);
```

**Result:** Character heals faster when hurt, slows as health increases

---

### Utility Functions

#### **GetLowestStat**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
EStatType GetLowestStat(float& OutPercentage) const;
```

**Purpose:** Find which stat is lowest (by percentage)

**Parameters:**
- `OutPercentage` - (Output) Percentage of the lowest stat

**Returns:** The stat type that is lowest

**Example:**
```cpp
float LowestPercent;
EStatType LowestStat = StatComp->GetLowestStat(LowestPercent);

if (LowestPercent < 0.3f) // Less than 30%
{
    FString StatName = UEnum::GetValueAsString(LowestStat);
    ShowWarning(FString::Printf(TEXT("WARNING: %s is low!"), *StatName));
}
```

**Use case:** AI decision making
```cpp
// AI decides what to do based on lowest stat
EStatType NeedMost = StatComp->GetLowestStat(Percent);

if (NeedMost == EStatType::Health_Core)
    AI_FindHealthPack();
else if (NeedMost == EStatType::Hunger)
    AI_FindFood();
else if (NeedMost == EStatType::Thirst)
    AI_FindWater();
```

---

#### **GetHighestStat**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
EStatType GetHighestStat(float& OutPercentage) const;
```

**Purpose:** Find which stat is highest

**Example:**
```cpp
float HighestPercent;
EStatType HighestStat = StatComp->GetHighestStat(HighestPercent);

// Character is best at whatever stat is highest
if (HighestStat == EStatType::Stamina)
    AIPersonality = "Athletic";
else if (HighestStat == EStatType::Intelligence)
    AIPersonality = "Smart";
```

---

#### **IsAnyCritical**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
bool IsAnyCritical() const;
```

**Purpose:** Check if ANY stat is below critical threshold (default 15%)

**Returns:** True if at least one stat is critical

**Example:**
```cpp
if (StatComp->IsAnyCritical())
{
    // At least one stat is critical!
    ShowCriticalWarning();
    PlayAlarmSound();
    
    // Maybe force player to address it
    if (bHardcoreMode)
    {
        ReduceMaxSpeed(0.5f); // Can only move at half speed
    }
}
```

**Multiplayer example:**
```cpp
// Server checks all players
for (APlayerController* PC : GetWorld()->GetPlayerControllerIterator())
{
    if (APawn* Pawn = PC->GetPawn())
    {
        if (UStatComponent* Stats = Pawn->FindComponentByClass<UStatComponent>())
        {
            if (Stats->IsAnyCritical())
            {
                // This player needs medical attention!
                BroadcastMedicalAlert(Pawn);
            }
        }
    }
}
```

---

#### **GetStatsBelowThresholdCount**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
int32 GetStatsBelowThresholdCount(float Threshold) const;
```

**Purpose:** Count how many stats are below a threshold

**Parameters:**
- `Threshold` - Percentage threshold (0.0 to 1.0)

**Returns:** Number of stats below threshold

**Example:**
```cpp
// How many stats are below 50%?
int32 LowStats = StatComp->GetStatsBelowThresholdCount(0.5f);

if (LowStats >= 3)
{
    ShowNotification("Multiple systems failing!");
}

// Difficulty scaling
int32 CriticalStats = StatComp->GetStatsBelowThresholdCount(0.15f);
if (CriticalStats >= 2)
{
    // Multiple critical stats = emergency mode
    ActivateEmergencyMode();
}
```

---

#### **TransferStatValue**

```cpp
UFUNCTION(BlueprintCallable, Category = "Stats")
void TransferStatValue(EStatType FromStat, EStatType ToStat, float Amount);
```

**Purpose:** Transfer value from one stat to another

**Parameters:**
- `FromStat` - Stat to take from
- `ToStat` - Stat to give to
- `Amount` - How much to transfer

**Use cases:**
- Energy to health conversion (magic healing)
- Stamina sacrifice for power
- Life drain abilities

**Example:**
```cpp
// Sacrifice 30 stamina to heal 20 health
StatComp->TransferStatValue(EStatType::Stamina, EStatType::Health_Core, 30.0f);
// Stamina: -30
// Health: +30 (but we want +20...)

// Better approach: Calculate conversion rate
void ACharacter::SacrificeStaminaForHealth()
{
    float StaminaCost = 30.0f;
    float HealthGain = StaminaCost * 0.66f; // 66% conversion rate = 20 HP
    
    if (StatComp->GetStatValue(EStatType::Stamina) >= StaminaCost)
    {
        StatComp->ApplyStatChange(EStatType::Stamina, -StaminaCost, TEXT("Sacrifice"));
        StatComp->ApplyStatChange(EStatType::Health_Core, HealthGain, TEXT("SacrificeHeal"));
    }
}
```

---

#### **GetHighestStatInCategory**

```cpp
UFUNCTION(BlueprintPure, Category = "Stats")
EStatType GetHighestStatInCategory(EStatCategory Category, float& OutPercentage) const;
```

**Purpose:** Find highest stat within a category

**Parameters:**
- `Category` - Which category to search (Core, Survival, Environmental, etc.)
- `OutPercentage` - (Output) Percentage of highest stat

**Returns:** Stat type of highest in category

**Categories:**
- `Core` - Health, Stamina, Energy
- `Survival` - Hunger, Thirst, Fatigue
- `Environmental` - BodyTemperature, Wetness
- `HealthConditions` - BloodLevel, Sanity, InfectionLevel, Toxicity, HeartRate, BloodPressure
- `RPG` - Strength, Dexterity, Intelligence, Endurance

**Example:**
```cpp
// Which survival stat is best?
float HighestPercent;
EStatType BestSurvival = StatComp->GetHighestStatInCategory(EStatCategory::Survival, HighestPercent);

if (BestSurvival == EStatType::Hunger)
{
    // Hunger is best (lowest), but thirst/fatigue need attention
    ShowNotification("Drink water and rest!");
}
```

---


---

## Complete Integration Examples

### Example 1: Survival Game - Full Integration

This example shows ALL systems working together in a survival game:

```cpp
// Character with all components
AMyCharacter::AMyCharacter()
{
    StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("Stats"));
    BodyComp = CreateDefaultSubobject<UBodyComponent>(TEXT("Body"));
    WeatherComp = CreateDefaultSubobject<UWeatherComponent>(TEXT("Weather"));
    
    // Setup initial stats
    bUseSimpleMode = true; // All stats start at 100
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Setup survival stat decay
    StatComp->SetStatRegenerationRate(EStatType::Hunger, 0.00333f); // +1 per 5 min
    StatComp->SetStatRegenerationRate(EStatType::Thirst, 0.00833f); // +1 per 2 min
    StatComp->SetStatRegenerationRate(EStatType::Fatigue, 0.0333f); // +1 per 30 sec
    
    // Setup stamina regen
    StatComp->SetStatRegenerationRate(EStatType::Stamina, 15.0f); // +15/sec
    
    // Bind events
    StatComp->OnStatChanged.AddDynamic(this, &AMyCharacter::OnStatChanged);
    StatComp->OnStatReachedZero.AddDynamic(this, &AMyCharacter::OnStatReachedZero);
    BodyComp->OnBodyPartDamaged.AddDynamic(this, &AMyCharacter::OnBodyPartDamaged);
    WeatherComp->OnFreezingStageChanged.AddDynamic(this, &AMyCharacter::OnFreezingStageChanged);
    
    // Setup weather for current biome
    SetupBiomeWeather();
}

void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Update gameplay based on stats
    UpdateMovementFromStats();
    UpdateStaminaFromActivity(DeltaTime);
    CheckSurvivalConditions();
    ApplyWeatherDamage(DeltaTime);
}

void AMyCharacter::UpdateMovementFromStats()
{
    // Base speed
    float Speed = 600.0f;
    
    // Health affects speed
    float HealthPercent = StatComp->GetStatPercentage(EStatType::Health_Core);
    if (HealthPercent < 0.5f)
        Speed *= (0.5f + HealthPercent); // 50-100% speed based on health
    
    // Leg injuries affect speed
    float LeftLegCondition = BodyComp->GetBodyPartCondition(EBodyPart::Left_Leg);
    float RightLegCondition = BodyComp->GetBodyPartCondition(EBodyPart::Right_Leg);
    float LegHealth = (LeftLegCondition + RightLegCondition) / 200.0f; // Average as percent
    Speed *= (0.5f + LegHealth * 0.5f); // 50-100% speed
    
    // Fractured legs: severe penalty
    if (BodyComp->IsBodyPartFractured(EBodyPart::Left_Leg) ||
        BodyComp->IsBodyPartFractured(EBodyPart::Right_Leg))
    {
        Speed *= 0.3f; // 70% slower with broken leg!
        bIsLimping = true;
    }
    
    // Stamina affects sprint
    float Stamina = StatComp->GetStatValue(EStatType::Stamina);
    if (bIsSprinting && Stamina <= 0.0f)
    {
        bIsSprinting = false; // Can't sprint without stamina
    }
    
    if (bIsSprinting)
        Speed *= 2.0f; // Sprint = 2x speed
    
    // Apply final speed
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AMyCharacter::UpdateStaminaFromActivity(float DeltaTime)
{
    float StaminaDrain = 0.0f;
    float StaminaRegen = 15.0f; // Base regen
    
    if (bIsSprinting)
    {
        StaminaDrain = 10.0f; // -10/sec while sprinting
        StaminaRegen = 0.0f; // No regen while sprinting
    }
    else if (GetVelocity().Size() > 10.0f) // Moving
    {
        StaminaRegen = 5.0f; // Slower regen while moving
    }
    else // Standing still
    {
        StaminaRegen = 20.0f; // Faster regen when resting
    }
    
    // Fatigue reduces stamina regen
    float FatigueLevel = StatComp->GetStatValue(EStatType::Fatigue);
    if (FatigueLevel > 75.0f)
        StaminaRegen *= 0.5f; // Half regen when exhausted
    
    // Apply
    if (StaminaDrain > 0.0f)
        StatComp->ApplyStatChange(EStatType::Stamina, -StaminaDrain * DeltaTime, TEXT("Activity"));
    
    StatComp->SetStatRegenerationRate(EStatType::Stamina, StaminaRegen);
}

void AMyCharacter::CheckSurvivalConditions()
{
    // Check starvation
    float Hunger = StatComp->GetStatValue(EStatType::Hunger);
    if (Hunger >= 90.0f)
    {
        // Starving! Take damage
        float Damage = 1.0f * GetWorld()->GetDeltaSeconds() / 60.0f; // -1 HP/min
        StatComp->ApplyStatChange(EStatType::Health_Core, -Damage, TEXT("Starvation"));
    }
    
    // Check dehydration
    float Thirst = StatComp->GetStatValue(EStatType::Thirst);
    if (Thirst >= 85.0f)
    {
        // Severely dehydrated! Take damage
        float Damage = 2.0f * GetWorld()->GetDeltaSeconds() / 60.0f; // -2 HP/min
        StatComp->ApplyStatChange(EStatType::Health_Core, -Damage, TEXT("Dehydration"));
    }
    
    // Check bleeding
    float TotalBleedRate = BodyComp->GetTotalBleedingRate();
    if (TotalBleedRate > 0.0f)
    {
        // Losing blood!
        StatComp->ApplyStatChange(EStatType::BloodLevel, -TotalBleedRate * GetWorld()->GetDeltaSeconds(), TEXT("Bleeding"));
    }
    
    // Check blood level
    float BloodLevel = StatComp->GetStatValue(EStatType::BloodLevel);
    if (BloodLevel < 30.0f)
    {
        // Severe blood loss! Pass out soon
        if (FMath::FRandRange(0.0f, 1.0f) < 0.01f * GetWorld()->GetDeltaSeconds())
        {
            PassOut();
        }
    }
}

void AMyCharacter::ApplyWeatherDamage(float DeltaTime)
{
    // Weather component handles temperature calculation and damage automatically!
    // But we can respond to freezing/overheating states
    
    EFreezingStage FreezingStage = WeatherComp->GetCurrentFreezingStage();
    if (FreezingStage != EFreezingStage::None)
    {
        // Character is freezing - show appropriate UI/effects
        UpdateFreezingEffects(FreezingStage);
    }
    
    EOverheatingStage OverheatingStage = WeatherComp->GetCurrentOverheatingStage();
    if (OverheatingStage != EOverheatingStage::None)
    {
        // Character is overheating
        UpdateOverheatingEffects(OverheatingStage);
    }
}

// Event handlers
void AMyCharacter::OnStatChanged(EStatType StatType, float NewValue, float Delta)
{
    // Update UI
    UpdateStatUI(StatType, NewValue);
    
    // Special handling
    if (StatType == EStatType::Health_Core)
    {
        float HealthPercent = StatComp->GetStatPercentage(EStatType::Health_Core);
        if (HealthPercent < 0.15f && !bCriticalHealthWarningShown)
        {
            ShowCriticalHealthWarning();
            bCriticalHealthWarningShown = true;
        }
        else if (HealthPercent > 0.2f)
        {
            bCriticalHealthWarningShown = false;
        }
    }
}

void AMyCharacter::OnStatReachedZero(EStatType StatType)
{
    if (StatType == EStatType::Health_Core)
    {
        // Dead!
        Die();
    }
    else if (StatType == EStatType::BloodLevel)
    {
        // Bled out!
        Die(TEXT("Blood Loss"));
    }
    else if (StatType == EStatType::Stamina)
    {
        // Exhausted, can't sprint
        bIsSprinting = false;
    }
}

void AMyCharacter::OnBodyPartDamaged(EBodyPart Part, float Damage)
{
    // Show damage indicator
    ShowDamageIndicator(Part);
    
    // Play appropriate sound
    PlayBodyPartDamageSound(Part, Damage);
    
    // Special effects for critical parts
    if (Part == EBodyPart::Head)
    {
        // Head damage = screen shake + blur
        ApplyScreenShake();
        ApplyVisionBlur(Damage / 10.0f);
    }
}

void AMyCharacter::OnFreezingStageChanged(EFreezingStage NewStage)
{
    switch (NewStage)
    {
    case EFreezingStage::None:
        RemoveFreezingEffects();
        break;
    case EFreezingStage::Chilled:
        ShowNotification("You're getting cold");
        PlayShiveringAnimation();
        break;
    case EFreezingStage::Cold:
        ShowNotification("You're cold! Find warmth!");
        break;
    case EFreezingStage::Freezing:
        ShowWarning("FREEZING! Seek shelter immediately!");
        ApplyFreezingScreenEffect();
        break;
    case EFreezingStage::Hypothermia:
        ShowCriticalWarning("HYPOTHERMIA! YOU ARE DYING!");
        break;
    case EFreezingStage::CriticalHypothermia:
        // Near death - force player to find help
        ReduceMovementSpeed(0.3f);
        ApplySevereVisionBlur();
        break;
    }
}

// Player actions
void AMyCharacter::Eat(AActor* FoodItem)
{
    if (UFoodComponent* Food = FoodItem->FindComponentByClass<UFoodComponent>())
    {
        // Reduce hunger
        StatComp->ApplyStatChange(EStatType::Hunger, -Food->HungerReduction, TEXT("Eating"));
        
        // Maybe also restore some health
        if (Food->bRestoresHealth)
        {
            StatComp->ApplyStatChange(EStatType::Health_Core, Food->HealthRestoration, TEXT("Food"));
        }
        
        // Destroy food
        FoodItem->Destroy();
    }
}

void AMyCharacter::Drink(AActor* WaterItem)
{
    if (UWaterComponent* Water = WaterItem->FindComponentByClass<UWaterComponent>())
    {
        // Reduce thirst
        StatComp->ApplyStatChange(EStatType::Thirst, -Water->ThirstReduction, TEXT("Drinking"));
        
        // Check if water is clean
        if (!Water->bIsClean)
        {
            // Dirty water = chance of infection
            if (FMath::FRandRange(0.0f, 1.0f) < 0.3f) // 30% chance
            {
                StatComp->ApplyStatChange(EStatType::InfectionLevel, 20.0f, TEXT("DirtyWater"));
                ShowNotification("You feel sick from the water...");
            }
        }
        
        WaterItem->Destroy();
    }
}

void AMyCharacter::UseMedkit()
{
    // Heal health
    StatComp->ApplyStatChange(EStatType::Health_Core, 50.0f, TEXT("Medkit"));
    
    // Stop all bleeding
    BodyComp->StopAllBleeding();
    
    // Heal some body part damage
    for (int32 i = 0; i < (int32)EBodyPart::MAX; ++i)
    {
        EBodyPart Part = (EBodyPart)i;
        BodyComp->HealBodyPart(Part, 20.0f);
    }
    
    ShowNotification("Used Medkit");
}

void AMyCharacter::Sleep(float Hours)
{
    // Reduce fatigue
    float FatigueReduction = Hours * 12.5f; // 8 hours = 100
    StatComp->ApplyStatChange(EStatType::Fatigue, -FatigueReduction, TEXT("Sleeping"));
    
    // Restore some health
    StatComp->ApplyStatChange(EStatType::Health_Core, Hours * 2.0f, TEXT("Rest"));
    
    // Heal body parts slightly
    for (int32 i = 0; i < (int32)EBodyPart::MAX; ++i)
    {
        EBodyPart Part = (EBodyPart)i;
        BodyComp->HealBodyPart(Part, Hours * 1.0f);
    }
    
    // BUT hunger and thirst still increase!
    StatComp->ApplyStatChange(EStatType::Hunger, Hours * 5.0f, TEXT("Sleeping"));
    StatComp->ApplyStatChange(EStatType::Thirst, Hours * 8.0f, TEXT("Sleeping"));
}

void AMyCharacter::TakeCover()
{
    // Find shelter
    float ShelterLevel = DetectShelterLevel(); // Raycast, check for building, etc.
    WeatherComp->SetShelterLevel(ShelterLevel);
    
    if (ShelterLevel > 75.0f)
    {
        ShowNotification("Found good shelter");
    }
}
```

This comprehensive example shows:
✅ All components working together
✅ Stats affecting gameplay (movement, sprint)
✅ Body damage affecting character (fractures, bleeding)
✅ Weather affecting survival (freezing, overheating)
✅ Survival mechanics (hunger, thirst, fatigue)
✅ Event-driven design
✅ Player actions (eat, drink, sleep, heal)

---

## Summary

StatSystemPro provides everything you need for character state management in Unreal Engine 5.6:

### Core Features Recap

✅ **18 Built-in Stats** - Fully explained with real-world meanings
✅ **Body Part System** - 6 body parts with fractures, bleeding, burns
✅ **Weather System** - 13 weather types, clothing, temperature mechanics
✅ **Status Effects** - Buffs, debuffs, stacking, timed effects
✅ **Progression** - XP, levels, skills, perks
✅ **Time System** - Day/night cycles, seasons
✅ **Editor Tools** - 20+ quick actions, testing scenarios, visual config
✅ **Multiplayer** - Automatic replication, zero setup
✅ **100% Enum Interface** - No text typing, zero typos

### Documentation Summary

This comprehensive guide covered:

📊 **Complete Stat Explanations** (18 stats, all values explained)
🌡️ **Complete Weather Guide** (All temperatures, wind speeds, shelter explained)
🩺 **Body System** (All body parts, conditions, damage)
🛠️ **Massively Expanded Editor Tools** (Testing workflows, scenarios)
📝 **Complete Function References** (Every function with examples)
🎮 **Integration Examples** (Real-world usage patterns)

### Where to Go Next

1. **Start Simple:**
   - Add StatComponent to your character
   - Use editor quick actions to test
   - Watch the stats in PIE

2. **Add Complexity:**
   - Add BodyComponent for locational damage
   - Add WeatherComponent for survival
   - Bind to events for UI updates

3. **Customize:**
   - Create regeneration curves
   - Setup DataTable overrides
   - Configure project settings

4. **Polish:**
   - Add UI based on events
   - Create testing scenarios
   - Optimize for your game

### Getting Help

- Check this documentation first (search for your question)
- Use editor tools to test and learn
- Check function tooltips in Blueprint
- Experiment with quick actions

---

**Thank you for using StatSystemPro!**

**Version:** 1.0.0  
**Documentation Version:** 1.0.0  
**Last Updated:** 2025-11-08

---

