# Complete Function Reference - Every Function Explained

## Table of Contents
1. [Regeneration Curves Explained](#regeneration-curves-explained)
2. [All Functions by Category](#all-functions-by-category)
3. [Function Details](#function-details)
4. [Advanced Use Cases](#advanced-use-cases)

---

## Regeneration Curves Explained

### What Are Regeneration Curves?

**Regeneration curves let you control HOW FAST a stat regenerates based on its current percentage.**

### Why Use Curves?

**Without Curve (Linear):**
- Stamina regenerates at +5 per second
- ALWAYS +5 per second
- Whether you're at 10% or 90% stamina
- Simple but unrealistic

**With Curve (Dynamic):**
- Stamina regenerates FASTER when nearly full
- Regenerates SLOWER when nearly empty
- More realistic and interesting gameplay!

### How Curves Work

**A curve is a graph that says: "At X% full, multiply regen by Y"**

**Example Curve (Stamina Regen):**
```
Percentage → Multiplier
0%    (0.0) → 0.5x  (regen 50% speed when exhausted)
25%   (0.25)→ 0.8x  (regen 80% speed)
50%   (0.5) → 1.0x  (regen 100% speed - normal)
75%   (0.75)→ 1.2x  (regen 120% speed)
100%  (1.0) → 1.5x  (regen 150% speed when nearly full)
```

**Result:**
- Base regen: +10 per second
- At 10% stamina: +10 * 0.5 = **+5 per second** (slow)
- At 50% stamina: +10 * 1.0 = **+10 per second** (normal)
- At 90% stamina: +10 * 1.5 = **+15 per second** (fast)

### Creating a Curve in Unreal

1. **Right-click in Content Browser**
2. **Miscellaneous → Curve**
3. **Choose "Curve Float"**
4. **Name it** (e.g., "Curve_StaminaRegen")
5. **Open it and add points:**
   - Click graph to add points
   - X axis = Percentage (0 to 1)
   - Y axis = Multiplier
6. **Assign to Stat Config Table** or **Set in C++**

### Common Curve Patterns

#### 1. "Slow When Low" (Realistic)
```
0%   → 0.3x  (slow recovery when exhausted)
50%  → 1.0x  (normal)
100% → 1.5x  (fast when nearly full)

USE FOR: Stamina, Energy (harder to recover when exhausted)
```

#### 2. "Fast When Low" (Arcade)
```
0%   → 2.0x  (fast recovery when low!)
50%  → 1.0x  (normal)
100% → 0.5x  (slow when nearly full)

USE FOR: Health (encourage aggressive play)
```

#### 3. "Panic Threshold"
```
0-30%  → 0.0x  (NO REGEN when critical!)
30-50% → 0.5x  (slow regen)
50%+   → 1.0x  (normal regen)

USE FOR: Advanced mechanics, survival difficulty
```

#### 4. "Linear" (No Curve)
```
All percentages → 1.0x (flat line)

USE FOR: Simple games, prototypes
```

### Example: Stamina System with Curve

**Setup:**
```cpp
// In data table:
Row: Stamina
- Default Max: 100
- Regen Rate: 10 (per second)
- Regen Curve: Curve_StaminaRegen (see above)
```

**In-Game Behavior:**
```
Player at 20% stamina:
→ Regen = 10 * 0.5 (curve value at 20%) = 5/sec
→ Takes 16 seconds to fill

Player at 80% stamina:
→ Regen = 10 * 1.5 (curve value at 80%) = 15/sec
→ Only takes 1.3 seconds to fill remaining 20%

Result: Last 20% fills super fast (satisfying!)
```

### When to Use Curves vs Flat Regen

| Use Flat Regen (No Curve) | Use Curves |
|----------------------------|-----------|
| Simple prototypes | Polished games |
| Arcade-style games | Realistic simulations |
| Constant gameplay pace | Dynamic gameplay |
| Beginner projects | Advanced mechanics |

---

## All Functions by Category

### 📊 Basic Getters (Most Used)
- `Get Stat Value` - Get current value
- `Get Stat Maximum` - Get max value
- `Get Stat Percentage (0-1)` - For progress bars
- `Get Stat Percentage (0-100)` - For text displays
- `Get Complete Stat Data` - All info at once
- `Has Stat?` - Check if stat exists

### 🔍 Checker Functions
- `Is Stat at Max?` - Check if full
- `Is Stat at Zero?` - Check if empty
- `Is Stat Critical?` - Check if below threshold
- `Is Any Stat Critical?` - Check all stats
- `Is Any Critical in Category?` - Check category
- `Is Any Critical in List?` - Check custom list

### 📈 Calculation Functions
- `Get Stat Missing Amount` - How much to heal
- `Get Average Stat Health` - Overall condition
- `Get Average Health in Category` - Category condition
- `Get Stat Regen Rate` - Current regen speed
- `Get Stat Base Max` - Original max before buffs
- `Get Lowest Stat in Category` - Find worst
- `Get Lowest Stat in List` - Find worst in list

### ✏️ Modification Functions
- `Change Stat (Add/Subtract)` - Main function!
- `Set Stat to Exact Value` - Override current
- `Set Stat Maximum` - Change max value
- `Set Regeneration Rate` - Change regen speed
- `Restore All Stats in Category` - Batch heal
- `Restore Stats in List` - Batch heal custom

### 🗂️ Category Functions
- `Get Stats in Category` - Get category members
- `Get All Stat Types` - Get all stats

---

## Function Details

### 📊 GET STAT VALUE

**What it does:** Returns the current value of a stat

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat? (dropdown)

**Outputs:**
- `Return Value` (float) - Current value

**Example:**
```
Get Stat Value (Health_Core)
→ Returns: 75.0 (current health)
```

**Use When:**
- Checking how much health player has
- Displaying stat value in UI
- Calculating healing needed
- Any time you need current value

**Multiplayer:** ✅ Works on server and clients

---

### 📊 GET STAT MAXIMUM

**What it does:** Returns the maximum value a stat can reach

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?

**Outputs:**
- `Return Value` (float) - Max value

**Example:**
```
Get Stat Maximum (Health_Core)
→ Returns: 100.0 (max health)

After buff:
→ Returns: 150.0 (buffed max)
```

**Use When:**
- Creating UI progress bars (need max for calculation)
- Calculating percentages manually
- Checking if buffs are active

**Multiplayer:** ✅ Works on server and clients

---

### 📊 GET STAT PERCENTAGE (0-1)

**What it does:** Returns stat as percentage between 0.0 and 1.0

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?

**Outputs:**
- `Return Value` (float) - Percentage (0.0 to 1.0)

**Formula:** Current ÷ Max

**Example:**
```
Health: 75 / 100
Get Stat Percentage (Health_Core)
→ Returns: 0.75

Plug directly into Progress Bar "Percent" pin!
```

**Use When:**
- UI progress bars
- Calculating other percentages
- Comparing stat levels

**Perfect For:** Progress bar widgets!

**Multiplayer:** ✅ Works on server and clients

---

### 📊 GET STAT PERCENTAGE (0-100)

**What it does:** Returns stat as percentage between 0 and 100

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?

**Outputs:**
- `Return Value` (float) - Percentage (0 to 100)

**Example:**
```
Health: 75 / 100
Get Stat Percentage 100 (Health_Core)
→ Returns: 75.0

Display as: "Health: 75%"
```

**Use When:**
- Text displays (Health: 75%)
- UI text blocks
- You prefer whole numbers

**Multiplayer:** ✅ Works on server and clients

---

### 📊 GET STAT MISSING AMOUNT

**What it does:** Returns how much stat is missing (Max - Current)

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?

**Outputs:**
- `Return Value` (float) - Missing amount

**Formula:** Max - Current

**Example:**
```
Health: 75 / 100
Get Stat Missing Amount (Health_Core)
→ Returns: 25.0 (can heal up to 25)

Use Case:
Health Pack heals 50
Player missing 25
→ Don't waste 25 healing!
→ Heal exactly 25, save item charges
```

**Use When:**
- Smart healing items
- Calculating waste
- Efficiency systems

**Multiplayer:** ✅ Works on server and clients

---

### ✏️ CHANGE STAT (ADD/SUBTRACT)

**What it does:** **MOST IMPORTANT FUNCTION** - Adds or subtracts from a stat

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat to change?
- `Amount` (float, required) - How much? (positive = add, negative = subtract)
- `Source` (Name, required) - For debugging (e.g., "Combat", "Food")
- `Reason Tag` (Gameplay Tag, optional) - Advanced filtering

**Outputs:** None

**Examples:**
```
// Damage player
Change Stat (Health_Core, -25, "Combat", None)
→ Health -25

// Heal player
Change Stat (Health_Core, +50, "Potion", None)
→ Health +50

// Eat food
Change Stat (Hunger, +30, "Food", None)
→ Hunger +30

// Drink water
Change Stat (Thirst, +40, "Water", None)
→ Thirst +40
```

**Use When:**
- Damage
- Healing
- Eating
- Drinking
- Any stat change!

**Important:**
- Auto-clamps to [0, Max]
- Can't go below 0
- Can't go above max

**Multiplayer:** ✅ Server only! Auto-replicates to clients

---

### ✏️ SET STAT TO EXACT VALUE

**What it does:** Sets stat to a specific value (overwrites current)

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?
- `New Value` (float, required) - Exact value to set

**Outputs:** None

**WARNING:** Ignores current value! Use "Change Stat" for relative changes.

**Examples:**
```
// Respawn with full health
Set Stat Value (Health_Core, 100)

// Cheat code
Set Stat Value (Health_Core, 9999)

// Load saved game
Set Stat Value (Health_Core, SavedHealth)
```

**Use When:**
- Respawning
- Loading saves
- Cheat codes
- Precise control needed

**DON'T Use For:**
- Damage (use Change Stat instead)
- Healing (use Change Stat instead)

**Multiplayer:** ✅ Server only! Auto-replicates

---

### ✏️ SET STAT MAXIMUM

**What it does:** Changes the maximum value a stat can reach

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?
- `New Max Value` (float, required) - New maximum

**Outputs:** None

**Auto-Behavior:** Current value is clamped to new max

**Examples:**
```
// +50 Max Health buff
Current Max: 100
Set Stat Maximum (Health_Core, 150)
→ New Max: 150

// -20 Max Stamina debuff (injured)
Current Max: 100
Set Stat Maximum (Stamina, 80)
→ New Max: 80
→ If current was 90, now clamped to 80
```

**Use When:**
- Buff items
- Debuff effects
- Leveling up
- Equipment bonuses

**Multiplayer:** ✅ Server only! Auto-replicates

---

### ✏️ SET REGENERATION RATE

**What it does:** Changes how fast a stat regenerates or decays per second

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?
- `Rate` (float, required) - Units per second (positive = regen, negative = decay)

**Outputs:** None

**Examples:**
```
// Health regeneration
Set Regeneration Rate (Health_Core, +5)
→ Gains 5 health per second

// Hunger decay
Set Regeneration Rate (Hunger, -1)
→ Loses 1 hunger per second

// Stop stamina regen (exhausted)
Set Regeneration Rate (Stamina, 0)
→ No change per second

// Fast stamina regen (rested)
Set Regeneration Rate (Stamina, +15)
→ Gains 15 stamina per second
```

**Use When:**
- Health regeneration buffs
- Survival stat decay
- Status effect durations
- Temporary regen boosts

**Note:** If stat has a Regeneration Curve, the curve multiplies this rate!

**Multiplayer:** ✅ Server only! Auto-replicates

---

### 🔍 IS STAT AT MAX?

**What it does:** Checks if a stat is at maximum value

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?

**Outputs:**
- `Return Value` (bool) - True if at max, False otherwise

**Examples:**
```
Health: 100 / 100
Is Stat at Max? (Health_Core)
→ Returns: True

Health: 75 / 100
Is Stat at Max? (Health_Core)
→ Returns: False
```

**Use When:**
- Preventing healing when full
- Achievement triggers
- "Fully Healed" messages
- Disabling regen when full

**Multiplayer:** ✅ Works on server and clients

---

### 🔍 IS STAT AT ZERO?

**What it does:** Checks if a stat is at 0

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?

**Outputs:**
- `Return Value` (bool) - True if zero, False otherwise

**Examples:**
```
Health: 0 / 100
Is Stat at Zero? (Health_Core)
→ Returns: True (player dead!)

Stamina: 0 / 100
Is Stat at Zero? (Stamina)
→ Returns: True (can't sprint!)
```

**Use When:**
- Death checks
- Stamina depletion
- Starvation triggers
- "Out of X" states

**Multiplayer:** ✅ Works on server and clients

---

### 🔍 IS STAT CRITICAL?

**What it does:** Checks if stat is below critical threshold (default 15%)

**Inputs:**
- `Stat Type` (EStatType, required) - Which stat?

**Outputs:**
- `Return Value` (bool) - True if below threshold

**Threshold:** Configurable in component (default 15%)

**Examples:**
```
Critical Threshold: 15%

Health: 10 / 100 (10%)
Is Stat Critical? (Health_Core)
→ Returns: True (below 15%!)

Health: 20 / 100 (20%)
Is Stat Critical? (Health_Core)
→ Returns: False (above 15%)
```

**Use When:**
- Warning UI
- Screen effects (red edges)
- Warning sounds
- Critical state logic

**Multiplayer:** ✅ Works on server and clients

---

### 🗂️ IS ANY CRITICAL IN CATEGORY?

**What it does:** Checks if ANY stat in selected category is below threshold

**Inputs:**
- `Category` (EStatCategory, required) - Which category? (dropdown)

**Outputs:**
- `Return Value` (bool) - True if any stat in category is critical

**Categories:**
- Core (Health, Stamina, Energy)
- Survival (Hunger, Thirst, Fatigue)
- Environmental (Temperature, Wetness)
- Health Conditions (Blood, Sanity, Infection, Toxicity)
- RPG Attributes (Str, Dex, Int, End)
- All (Every stat)

**Examples:**
```
Survival Category: Hunger=80%, Thirst=10%, Fatigue=50%

Is Any Critical in Category? (Survival)
→ Returns: True (Thirst is 10% = critical!)

Core Category: Health=90%, Stamina=80%, Energy=85%

Is Any Critical in Category? (Core)
→ Returns: False (all above 15%)
```

**Use When:**
- Category-specific warnings
- "Need food/water/rest" checks
- Grouped danger detection

**Multiplayer:** ✅ Works on server and clients

---

### 🎨 IS ANY CRITICAL IN LIST?

**What it does:** Checks if ANY stat in your custom list is below threshold

**Inputs:**
- `Stats To Check` (Array of EStatType, required) - Your custom stat list

**Outputs:**
- `Return Value` (bool) - True if any in list is critical

**Examples:**
```
// Check if bleeding (Health OR Blood low)
Make Array: [Health_Core, Blood_Level]
Is Any Critical in List?
→ If either is low = True

// Check combat stats only
Make Array: [Health_Core, Stamina]
Is Any Critical in List?
→ Ignores hunger, thirst, etc.
```

**Use When:**
- Custom danger checks
- Specific stat combinations
- Ignoring irrelevant stats
- Precise control needed

**Multiplayer:** ✅ Works on server and clients

---

### 📈 GET LOWEST STAT IN CATEGORY

**What it does:** Finds the stat with lowest percentage in a category

**Inputs:**
- `Category` (EStatCategory, required) - Which category?
- `Out Percentage` (float, output) - Percentage of lowest stat

**Outputs:**
- `Return Value` (EStatType) - The lowest stat type
- `Out Percentage` (float) - Its percentage (0-1)

**Examples:**
```
Core Category:
- Health: 90%
- Stamina: 30%  ← Lowest!
- Energy: 80%

Get Lowest Stat in Category (Core, OutPercent)
→ Returns: Stamina
→ OutPercent: 0.30
```

**Use When:**
- Smart healing AI
- Auto-priority systems
- Finding what needs healing most

**Multiplayer:** ✅ Works on server and clients

---

### 🎨 GET LOWEST STAT IN LIST

**What it does:** Finds the stat with lowest percentage in your custom list

**Inputs:**
- `Stats To Check` (Array of EStatType, required) - Your list
- `Out Percentage` (float, output) - Percentage of lowest

**Outputs:**
- `Return Value` (EStatType) - The lowest stat
- `Out Percentage` (float) - Its percentage

**Example:**
```
Check: [Hunger=80%, Thirst=20%, Fatigue=90%]

Get Lowest Stat in List
→ Returns: Thirst
→ OutPercent: 0.20
```

**Use When:**
- Smart item usage
- AI priority decisions
- Custom healing logic

**Multiplayer:** ✅ Works on server and clients

---

### ✏️ RESTORE ALL STATS IN CATEGORY

**What it does:** Adds amount to all stats in a category

**Inputs:**
- `Category` (EStatCategory, required) - Which category?
- `Amount` (float, required) - How much to restore (positive number)

**Outputs:** None

**Examples:**
```
// Rest at campfire - restore survival
Restore All Stats in Category (Survival, +50)
→ Hunger +50
→ Thirst +50
→ Fatigue +50

// Medical kit - heal core stats
Restore All Stats in Category (Core, +30)
→ Health +30
→ Stamina +30
→ Energy +30
```

**Use When:**
- Rest systems
- Medical items
- Buff potions
- Category-wide effects

**Multiplayer:** ✅ Server only! Auto-replicates

---

### 🎨 RESTORE STATS IN LIST

**What it does:** Adds amount to all stats in your custom list

**Inputs:**
- `Stats To Restore` (Array of EStatType, required) - Your custom list
- `Amount` (float, required) - How much to restore

**Outputs:** None

**Examples:**
```
// Water bottle - Thirst + Energy
Make Array: [Thirst, Energy]
Restore Stats in List (+40)
→ Thirst +40
→ Energy +40
→ Hunger unchanged!

// Bandage - Health + Blood
Make Array: [Health_Core, Blood_Level]
Restore Stats in List (+20)
→ Health +20
→ Blood +20
```

**Use When:**
- Items with unique effects
- Custom healing combinations
- Special mechanics

**Multiplayer:** ✅ Server only! Auto-replicates

---

## Advanced Use Cases

### 1. Dynamic Difficulty - Health Regen Based on Difficulty

```blueprint
Event BeginPlay
├─ Switch on Difficulty Setting:
    ├─ Easy:
        └─ Set Regeneration Rate (Health_Core, +5)
            └─ Health regens 5/sec
    ├─ Normal:
        └─ Set Regeneration Rate (Health_Core, +2)
            └─ Health regens 2/sec
    └─ Hard:
        └─ Set Regeneration Rate (Health_Core, 0)
            └─ No health regen!
```

---

### 2. Bleeding System with Bandage Tiers

```blueprint
On Player Bleeding
├─ Set Regeneration Rate (Blood_Level, -2)
    └─ Loses 2 blood per second

On Use Basic Bandage
├─ Set Regeneration Rate (Blood_Level, -0.5)
    └─ Still bleeding, but slower

On Use Advanced Bandage
├─ Set Regeneration Rate (Blood_Level, 0)
├─ Restore Stats in List ([Health_Core, Blood_Level], +30)
    └─ Stops bleeding AND heals!

On Use Medical Kit
├─ Set Regeneration Rate (Blood_Level, +1)
├─ Restore All Stats in Category (Core, +50)
    └─ Blood regenerates AND full heal!
```

---

### 3. Smart Auto-Heal System

```blueprint
Every 5 Seconds
├─ Get Lowest Stat in Category (Core, LowestPercent)
    ├─ Found: Which stat is worst?
    └─ Branch: LowestPercent < 0.5? (below 50%)
        └─ True:
            ├─ Print: "Auto-healing {LowestStat}"
            └─ Change Stat (LowestStat, +10)
                └─ Heal worst stat first!
```

---

### 4. Survival Game - Progressive Starvation

```blueprint
// Stage 1: Hungry (50-25%)
Event Tick (check every frame)
├─ Get Stat Percentage (Hunger)
    └─ Is between 0.25 and 0.5?
        └─ Set Regeneration Rate (Energy, -0.5)
            └─ Lose energy slowly

// Stage 2: Starving (25-10%)
├─ Is between 0.1 and 0.25?
    ├─ Set Regeneration Rate (Energy, -1)
    └─ Set Regeneration Rate (Stamina, -2)
        └─ Lose energy + stamina

// Stage 3: Critical (below 10%)
└─ Is below 0.1?
    ├─ Set Regeneration Rate (Energy, -2)
    ├─ Set Regeneration Rate (Stamina, -3)
    └─ Set Regeneration Rate (Health_Core, -1)
        └─ Start taking damage!
```

---

### 5. Temperature-Based Stat Changes

```blueprint
On Environment Changed (Temperature)
├─ Branch: Temp < 0°C (Freezing)
    ├─ True:
        ├─ Set Regeneration Rate (Body_Temperature, -0.5)
        ├─ Set Regeneration Rate (Stamina, -2)
        └─ Apply Status Effect ("Hypothermia")
    └─ False:
        └─ Branch: Temp > 35°C (Hot)
            ├─ True:
                ├─ Set Regeneration Rate (Thirst, -2)
                ├─ Set Regeneration Rate (Stamina, -1)
                └─ Apply Status Effect ("Heatstroke")
            └─ False:
                └─ // Normal temperature, clear effects
                    ├─ Set Regeneration Rate (Body_Temperature, 0)
                    └─ Remove effects
```

---

### 6. RPG Leveling System

```blueprint
On Level Up
├─ Get Current Level
    └─ For each stat in RPG Attributes category:
        └─ // +5 max to each attribute per level
            └─ Get Stat Maximum (CurrentStat)
                └─ Set Stat Maximum (CurrentStat, OldMax + 5)
```

---

### 7. Item Rarity System

```blueprint
On Use Health Potion
├─ Switch on Item Rarity:
    ├─ Common:
        └─ Change Stat (Health_Core, +20)
    ├─ Rare:
        ├─ Change Stat (Health_Core, +50)
        └─ Set Regeneration Rate (Health_Core, +2) for 10 sec
    └─ Legendary:
        ├─ Restore All Stats in Category (Core, +80)
        ├─ Set Regeneration Rate (Health_Core, +5) for 30 sec
        └─ Set Stat Maximum (Health_Core, OldMax + 50) for 60 sec
            └─ Temp max health boost!
```

---

### 8. Combat State Machine

```blueprint
// Detect Combat State
Every Frame
├─ Is Any Critical in Category? (Core)
    ├─ True = DANGER state
        ├─ Slow movement
        ├─ Enable emergency abilities
        └─ Play warning sounds
    └─ False:
        └─ Get Average Health in Category (Core)
            ├─ Below 0.5 = WOUNDED state
                ├─ Reduce max sprint speed
                └─ Show blood effects
            └─ Above 0.8 = HEALTHY state
                └─ Full capabilities
```

---

### 9. Environmental Hazard Zones

```blueprint
On Enter Radiation Zone
├─ Set Regeneration Rate (Health_Core, -5)
├─ Set Regeneration Rate (Toxicity, +3)
└─ Every 10 seconds while inside:
    └─ Get Stat Value (Toxicity)
        └─ Change Stat (Sanity, -(Toxicity * 0.1))
            └─ More toxic = more insane!

On Exit Radiation Zone
├─ Set Regeneration Rate (Health_Core, 0)
└─ Set Regeneration Rate (Toxicity, -1)
    └─ Toxicity slowly decreases
```

---

### 10. Boss Fight Mechanics

```blueprint
On Boss Phase Change
├─ Switch on Boss Health Percentage:
    ├─ 100-75% (Phase 1):
        └─ Player: Normal stats
    ├─ 75-50% (Phase 2):
        ├─ Boss enrages!
        └─ Player: Set Regeneration Rate (Stamina, -3)
            └─ Stamina drains (fear effect)
    ├─ 50-25% (Phase 3):
        ├─ Boss desperation!
        ├─ Set Regeneration Rate (Sanity, -2)
        └─ Apply Status Effect ("Madness")
            └─ Screen distortion!
    └─ Below 25% (Phase 4):
        └─ Boss death throes!
            ├─ Restore All Stats in Category (Core, +100)
            ├─ Set All Regen Rates (Core Category) to +10
            └─ Player powered up for final push!
```

---

### 11. Day/Night Survival Cycle

```blueprint
// Day System
On Time of Day Changed
├─ Switch on Time:
    ├─ Day (6 AM - 6 PM):
        ├─ Set Regeneration Rate (Energy, +1)
        ├─ Set Regeneration Rate (Sanity, +0.5)
        └─ Clear night debuffs
    ├─ Evening (6 PM - 8 PM):
        ├─ Set Regeneration Rate (Energy, 0)
        └─ Warning: "Night approaches!"
    ├─ Night (8 PM - 6 AM):
        ├─ Set Regeneration Rate (Fatigue, +2)
        ├─ Set Regeneration Rate (Sanity, -1)
        ├─ Set Regeneration Rate (Energy, -0.5)
        └─ Apply Status Effect ("Night Terror")
            └─ Harder survival at night!
```

---

### 12. Injury Recovery System

```blueprint
On Take Damage (Amount)
├─ Store Current Max Health
├─ Calculate Injury: Damage > 50?
    └─ True (Major Injury):
        ├─ Set Stat Maximum (Health_Core, CurrentMax * 0.8)
            └─ Max health reduced 20%!
        ├─ Set Regeneration Rate (Health_Core, -0.1)
            └─ Slowly bleeds
        └─ Start Recovery Timer (60 seconds)
            └─ On Timer End:
                ├─ Set Stat Maximum (Health_Core, OriginalMax)
                └─ Set Regeneration Rate (Health_Core, 0)
                    └─ Fully recovered!
```

---

These advanced use cases show how to combine multiple functions for complex game mechanics!
