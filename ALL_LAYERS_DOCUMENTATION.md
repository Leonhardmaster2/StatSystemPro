# All Layers Documentation - Status Effects, Body, Environment, Progression

## Table of Contents
1. [Status Effect System](#status-effect-system)
2. [Body Layer System](#body-layer-system)
3. [Environment Layer System](#environment-layer-system)
4. [Progression Layer System](#progression-layer-system)
5. [How Layers Work Together](#how-layers-work-together)

---

# Status Effect System

## What Are Status Effects?

**Status Effects are buffs and debuffs that modify your character.**

**Examples:**
- 🟢 **Buffs:** Health regeneration, speed boost, damage bonus
- 🔴 **Debuffs:** Poison, bleeding, exhaustion, stun
- 🟡 **Conditions:** Hypothermia, heatstroke, starvation

---

## Effect Types Explained

### 1. TEMPORARY Effects

**What:** Effects that last for a fixed duration, then disappear

**Examples:**
```
AdrenalineRush:
- Type: Temporary
- Duration: 10 seconds
- Effect: +20% stamina, ignore pain
- After 10 seconds: Effect ends

SpeedBoost:
- Type: Temporary
- Duration: 30 seconds
- Effect: +50% movement speed
```

**Use When:**
- Power-ups
- Temporary buffs from items
- Timed abilities

---

### 2. CONDITIONAL Effects

**What:** Effects that persist WHILE a condition is met, then auto-remove

**Examples:**
```
Cold:
- Type: Conditional
- Condition: Body temperature < 15°C
- Effect: Stamina regen -50%
- When warm again: Effect auto-removes!

Wet:
- Type: Conditional
- Condition: Wetness > 50%
- Effect: Movement speed -20%
- When dry: Effect auto-removes!
```

**Use When:**
- Environmental effects
- State-based conditions
- Things that should auto-clear

---

### 3. PERMANENT Effects

**What:** Effects that last forever (until manually removed)

**Examples:**
```
VampireTrait:
- Type: Permanent
- Effect: Drain health from enemies
- Removal: Never (it's a trait!)

MutationStrength:
- Type: Permanent
- Effect: +10 max strength
- Removal: Only via cure item
```

**Use When:**
- Character traits
- Mutations
- Permanent upgrades
- Perks

---

### 4. STACKABLE Effects

**What:** Effects that can apply multiple times, with cumulative effects

**THIS IS WHERE STACKS COME IN!**

### 🎯 WHAT ARE STACKS?

**Stacks = How many times an effect is applied**

**Example: Poison**
```
Poison:
- Type: Stackable
- Max Stacks: 5
- Effect PER STACK: -2 health per second

1 Stack:  -2 health/sec
2 Stacks: -4 health/sec
3 Stacks: -6 health/sec
4 Stacks: -8 health/sec
5 Stacks: -10 health/sec (max!)
6th poison application: Ignored (already at max)
```

**How Stacks Work:**
```
Player gets hit by poison arrow:
└─ Apply Status Effect ("Poison", 1 stack)
    └─ Now has 1 stack of Poison (-2 hp/sec)

Player gets hit AGAIN:
└─ Apply Status Effect ("Poison", 1 stack)
    └─ Now has 2 stacks of Poison (-4 hp/sec)

Each application ADDS a stack (up to max)
```

**More Examples:**
```
Bleeding:
- Max Stacks: 3
- Per Stack: -5 blood/sec
- 3 Stacks: -15 blood/sec (very bad!)

Armor Buff:
- Max Stacks: 5
- Per Stack: +10 armor
- 5 Stacks: +50 armor total

Rage:
- Max Stacks: 10
- Per Stack: +5% damage
- 10 Stacks: +50% damage (berserker mode!)
```

**Use When:**
- Damage over time (poison, bleed)
- Stackable buffs
- Intensity-based effects
- Build-up mechanics

---

## Effect Triggers Explained

**Triggers = What causes the effect to apply**

### MANUAL Trigger
```
Applied by blueprint/code manually

Example:
On Use Potion:
└─ Apply Status Effect ("HealthRegen")
```

### ON DAMAGE Trigger
```
Applied when taking damage

Example:
Thorns Armor:
└─ On Take Damage:
    └─ Apply Effect ("Bleeding") to Attacker
```

### OVER TIME Trigger
```
Applied periodically

Example:
In Poisonous Gas:
└─ Every 5 seconds:
    └─ Apply Effect ("Poison", +1 stack)
```

### ENVIRONMENT Trigger
```
Applied based on environment

Example:
Temperature < 0°C:
└─ Apply Effect ("Hypothermia")
```

### BODY PART Trigger
```
Applied when body part damaged

Example:
Leg Fractured:
└─ Apply Effect ("Limping")
```

---

## Creating Status Effects

### Method 1: Data Table (Recommended)

**Step 1: Create Data Table**
```
1. Right-click in Content Browser
2. Miscellaneous → Data Table
3. Row Type: StatusEffectTableRow
4. Name: DT_StatusEffects
```

**Step 2: Add Effect Rows**
```
Row: Poison
- Display Name: "Poisoned"
- Description: "Taking damage over time"
- Effect Type: Stackable
- Duration: 10 seconds
- Max Stacks: 5
- Stat Modifiers:
  └─ Health: -2 per second (per stack)
```

**Step 3: Assign to Component**
```
Status Effect Component:
└─ Status Effect Table: DT_StatusEffects
```

### Method 2: Code/Blueprint

```cpp
// Create effect data
FStatusEffectData PoisonEffect;
PoisonEffect.EffectID = "Poison";
PoisonEffect.DisplayName = FText::FromString("Poisoned");
PoisonEffect.EffectType = EStatusEffectType::Stackable;
PoisonEffect.Duration = 10.0f;
PoisonEffect.MaxStacks = 5;

// Add stat modifier
FStatusEffectStatModifier HealthMod;
HealthMod.StatName = "Health";
HealthMod.FlatModifier = -2.0f;
PoisonEffect.StatModifiers.Add(HealthMod);

// Apply effect
StatusEffectComp->ApplyStatusEffectFromData(PoisonEffect, 1);
```

---

## Status Effect Functions

### APPLY STATUS EFFECT

**What:** Applies an effect by ID from data table

**Inputs:**
- `Effect ID` (Name, required) - Which effect? **[Name from data table row]**
- `Stacks` (int32, optional, default 1) - How many stacks? **[1-5 usually]**

**Outputs:**
- `Return Value` (bool) - True if applied successfully

**Examples:**
```blueprint
// Apply poison (1 stack)
Apply Status Effect ("Poison", 1)
→ Returns: True

// Apply poison (3 stacks at once)
Apply Status Effect ("Poison", 3)
→ Returns: True

// Apply speed boost
Apply Status Effect ("SpeedBoost", 1)
→ Returns: True
```

**Multiplayer:** Server only, auto-replicates

---

### APPLY STATUS EFFECT FROM DATA

**What:** Applies an effect from a data struct (no data table needed)

**Inputs:**
- `Effect Data` (FStatusEffectData, required) - The complete effect data
- `Stacks` (int32, optional) - How many stacks?

**Use When:**
- Dynamic effects
- Procedurally generated effects
- No data table available

---

### REMOVE EFFECT

**What:** Removes an effect by ID

**Inputs:**
- `Effect ID` (Name, required) - Which effect to remove?

**Outputs:**
- `Return Value` (bool) - True if removed

**Example:**
```blueprint
// Remove poison
Remove Effect ("Poison")
→ Effect removed, all stacks cleared
```

**Multiplayer:** Server only, auto-replicates

---

### REMOVE EFFECTS BY TAG

**What:** Removes all effects with a specific gameplay tag

**Inputs:**
- `Tag` (Gameplay Tag, required) - Tag to match

**Outputs:**
- `Return Value` (int32) - How many effects removed

**Example:**
```blueprint
// Remove all negative effects
Remove Effects By Tag ("Effect.Negative")
→ Returns: 3 (removed 3 debuffs)
```

**Use When:**
- Cleansing items (remove all debuffs)
- Cure-all potions
- Immunity abilities

---

### HAS EFFECT

**What:** Checks if an effect is currently active

**Inputs:**
- `Effect ID` (Name, required) - Which effect?

**Outputs:**
- `Return Value` (bool) - True if active

**Example:**
```blueprint
// Check if poisoned
Has Effect ("Poison")
→ Returns: True (player is poisoned!)
```

**Use When:**
- Conditional logic
- UI indicators
- Preventing duplicate effects

---

### GET EFFECT STACKS

**What:** Gets current stack count for an effect

**Inputs:**
- `Effect ID` (Name, required) - Which effect?

**Outputs:**
- `Return Value` (int32) - Current stacks (0 if not active)

**Example:**
```blueprint
// How poisoned are we?
Get Effect Stacks ("Poison")
→ Returns: 3 (3 stacks of poison)

// Not poisoned
Get Effect Stacks ("Poison")
→ Returns: 0 (no poison)
```

**Use When:**
- UI display (show stack count)
- Conditional logic based on intensity
- Warning thresholds

---

### GET EFFECT TIME REMAINING

**What:** Gets time left for a temporary effect

**Inputs:**
- `Effect ID` (Name, required) - Which effect?

**Outputs:**
- `Return Value` (float) - Seconds remaining (0 if not active)

**Example:**
```blueprint
// How long until speed boost ends?
Get Effect Time Remaining ("SpeedBoost")
→ Returns: 7.5 (7.5 seconds left)
```

**Use When:**
- Timer UI
- Countdown displays
- Reapplication timing

---

### CLEAR ALL EFFECTS

**What:** Removes ALL active effects

**Inputs:** None

**Outputs:** None

**Example:**
```blueprint
// Remove everything
Clear All Effects
→ All buffs and debuffs removed
```

**Use When:**
- Death
- Respawn
- Reset character state
- Debugging

---

## Status Effect Events

### ON STATUS EFFECT APPLIED
```
Fires when: Effect is applied or stacks added
Provides: Effect ID, Stack Count

Use For:
- Play sound effect
- Show UI notification
- Trigger animations
```

### ON STATUS EFFECT REMOVED
```
Fires when: Effect is manually removed
Provides: Effect ID

Use For:
- UI updates
- Effect ended notifications
```

### ON STATUS EFFECT EXPIRED
```
Fires when: Effect duration reaches 0
Provides: Effect ID, Duration

Use For:
- Auto-cleanup
- "Effect wore off" messages
```

---

## Status Effect Examples

### Example 1: Poison System

**Data Table Setup:**
```
Row: Poison
- Effect Type: Stackable
- Duration: 10 seconds
- Max Stacks: 5
- Stat Modifiers:
  └─ Health: -2/sec (per stack)
```

**Blueprint:**
```
On Enemy Hit Player:
├─ Apply Status Effect ("Poison", 1)
└─ Show "Poisoned!" message

On Use Antidote:
└─ Remove Effect ("Poison")
```

---

### Example 2: Buff Stack System

**Data Table:**
```
Row: DamageBoost
- Effect Type: Stackable
- Duration: -1 (permanent until removed)
- Max Stacks: 10
- Per Stack: +5% damage
```

**Blueprint:**
```
On Kill Enemy:
├─ Apply Status Effect ("DamageBoost", 1)
└─ Current Stacks:
    └─ Get Effect Stacks ("DamageBoost")
        └─ 5 stacks = +25% damage!

On Take Damage:
└─ Remove Effect ("DamageBoost")
    └─ Lose all stacks!
```

---

### Example 3: Environmental Condition

**Blueprint:**
```
On Environment Changed:
├─ If Temperature < 0°C:
    └─ Apply Status Effect ("Hypothermia", 1)
├─ Else If Temperature > 35°C:
    └─ Apply Status Effect ("Heatstroke", 1)
└─ Else:
    ├─ Remove Effect ("Hypothermia")
    └─ Remove Effect ("Heatstroke")
```

---

### Example 4: Cleanse System

**Blueprint:**
```
On Use Holy Water:
├─ Remove Effects By Tag ("Effect.Negative")
    └─ Removes ALL debuffs!
├─ Apply Status Effect ("Blessed", 1)
    └─ Immune to debuffs for 30 seconds
```

---

### Example 5: Build-Up Mechanic

**Blueprint:**
```
Every Attack:
├─ Apply Status Effect ("Rage", 1)
    └─ Adds 1 rage stack (+5% damage)

Get Effect Stacks ("Rage")
├─ 10 stacks (max)?
    └─ Trigger Berserk Mode!
        ├─ Apply Effect ("Berserk", 1)
        └─ Clear Effect ("Rage")
            └─ Reset stacks, enter berserk!
```

---

# Body Layer System

## What Is the Body Layer?

**The Body Layer tracks damage to specific body parts.**

**Body Parts:**
1. Head
2. Torso
3. Left Arm
4. Right Arm
5. Left Leg
6. Right Leg

**Each part has:**
- Condition (0-100, like health per part)
- Fracture state (broken or not)
- Bleeding rate (units per second)
- Burn level (none, 1st, 2nd, 3rd degree)
- Infection rate (0-100)
- Pain level (0-100)

---

## Body Part States Explained

### CONDITION (0-100)

**What:** Overall health of this body part

```
100 = Perfect condition
75  = Bruised
50  = Injured
25  = Badly damaged
0   = Destroyed
```

**Example:**
```
Left Leg Condition: 30
→ Badly damaged leg
→ Walking is painful and slow
```

---

### FRACTURE (True/False)

**What:** Is this bone broken?

**Effects:**
```
Fractured Leg:
- Movement speed: -70%
- Stamina drain: +100%
- Pain level: +50

Fractured Arm:
- Weapon accuracy: -80%
- Weapon sway: +150%
- Can't use two-handed weapons
```

**Example:**
```
Damage Body Part (Left_Leg, 50)
→ Condition drops below 20
→ Auto-fracture!
```

---

### BLEEDING RATE (Units/Second)

**What:** How fast this part is losing blood

**Calculation:**
```
Total Bleeding = Sum of all body parts

Left Arm bleeding: 2/sec
Right Leg bleeding: 3/sec
Total bleeding: 5/sec

Blood Level decreases 5 per second!
```

**Example:**
```
Set Bleeding Rate (Left_Arm, 2.0)
→ Arm bleeds 2 blood per second
→ Bandage reduces to 0.5/sec
→ Advanced bandage stops it (0/sec)
```

---

### BURN LEVEL (None, 1st, 2nd, 3rd Degree)

**What:** Severity of burns

```
None:        No burns
1st Degree:  Minor burns, +10 pain
2nd Degree:  Serious burns, +30 pain, blisters
3rd Degree:  Severe burns, +50 pain, permanent damage
```

**Example:**
```
Walk through fire:
└─ Set Burn Level (Torso, Second_Degree)
    ├─ Pain +30
    ├─ Infection rate increases
    └─ Needs medical treatment
```

---

### INFECTION RATE (0-100)

**What:** How infected this part is

**Effects:**
```
0     = Clean
1-25  = Minor infection
26-50 = Infection spreading
51-75 = Serious infection
76-100= Sepsis (critical!)
```

**Mechanics:**
```
Infection grows over time if untreated:
- Per second: +0.5 infection
- When reaches 100: Apply "Sepsis" effect
- Treatment: Antibiotics reduce infection rate
```

---

### PAIN LEVEL (0-100)

**What:** How much this part hurts

**Total Pain:**
```
Total Pain = Average of all body parts

Head: 30
Torso: 20
Arms: 10 each
Legs: 40 each

Total Pain: 25 (affects character)
```

**Effects:**
```
Pain < 20:   No effect
Pain 20-50:  Slight accuracy reduction
Pain 50-80:  Heavy penalties
Pain > 80:   Screen shake, can't aim
```

---

## Body Layer Functions

### DAMAGE BODY PART

**What:** Damages a specific body part

**Inputs:**
- `Body Part` (EBodyPart, required) - Which part? **[Dropdown: Head, Torso, etc.]**
- `Damage` (float, required) - How much damage?

**Effects:**
- Reduces condition
- Increases pain
- May auto-fracture if condition too low

**Example:**
```
// Shot in the leg!
Damage Body Part (Left_Leg, 30)
→ Condition: 100 → 70
→ Pain: +15
```

**Multiplayer:** Server only, auto-replicates

---

### FRACTURE LIMB

**What:** Manually fractures a body part

**Inputs:**
- `Body Part` (EBodyPart, required) - Which part?

**Effects:**
- Sets fractured = true
- Adds +50 pain
- Applies movement/accuracy penalties

**Example:**
```
// Fall from height
Fracture Limb (Left_Leg)
→ Leg broken!
→ Movement speed -70%
```

---

### SET BLEEDING RATE

**What:** Sets how fast a part bleeds

**Inputs:**
- `Body Part` (EBodyPart, required) - Which part?
- `Rate` (float, required) - Units per second

**Example:**
```
// Stabbed in the arm!
Set Bleeding Rate (Right_Arm, 3.0)
→ Bleeds 3 blood per second

// Apply bandage
Set Bleeding Rate (Right_Arm, 0.5)
→ Still bleeding, but slower

// Advanced bandage
Set Bleeding Rate (Right_Arm, 0)
→ Bleeding stopped!
```

---

### SET BURN LEVEL

**What:** Sets burn severity for a part

**Inputs:**
- `Body Part` (EBodyPart, required) - Which part?
- `Burn Level` (EBurnLevel, required) - **[Dropdown: None, First_Degree, etc.]**

**Example:**
```
// Walk through fire
Set Burn Level (Torso, Second_Degree)
→ Serious burns!
→ Pain +30
→ Needs treatment
```

---

### APPLY INFECTION

**What:** Adds infection to a body part

**Inputs:**
- `Body Part` (EBodyPart, required) - Which part?
- `Infection Amount` (float, required) - How much to add?

**Example:**
```
// Dirty wound
Apply Infection (Left_Arm, 20)
→ Infection rate: 0 → 20

// Gets worse over time
Every second: +0.5 infection
After 60 seconds: 50 infection (serious!)
```

---

### HEAL LIMB

**What:** Heals a body part

**Inputs:**
- `Body Part` (EBodyPart, required) - Which part?
- `Heal Amount` (float, required) - How much?

**Effects:**
- Increases condition
- Reduces pain (by 30% of heal amount)

**Example:**
```
// Use medical kit on leg
Heal Limb (Left_Leg, 30)
→ Condition: 40 → 70
→ Pain: 50 → 41
```

---

### GET BODY PART STATE

**What:** Gets all info about a body part

**Inputs:**
- `Body Part` (EBodyPart, required) - Which part?

**Outputs:**
- `Return Value` (FBodyPartState) - Complete state struct

**Struct Contains:**
- Condition
- Max Condition
- Fractured (bool)
- Bleeding Rate
- Burn Level
- Infection Rate
- Pain Level

**Example:**
```
Get Body Part State (Left_Leg)
→ Condition: 30
→ Fractured: True
→ Bleeding: 2.0/sec
→ Pain: 60
```

---

### GET TOTAL BLEEDING RATE

**What:** Sum of bleeding from ALL body parts

**Outputs:**
- `Return Value` (float) - Total bleeding per second

**Example:**
```
Left Arm: 2/sec
Right Leg: 3/sec
Get Total Bleeding Rate
→ Returns: 5.0 (total)
```

---

### GET TOTAL PAIN LEVEL

**What:** Average pain across all body parts

**Outputs:**
- `Return Value` (float) - Average pain (0-100)

**Example:**
```
Head: 80 (bad headache!)
Legs: 40 each
Other parts: 10

Get Total Pain Level
→ Returns: 33.3 (average)
```

---

### HAS CRITICAL INJURY

**What:** Checks if ANY body part is critically damaged

**Critical Conditions:**
- Condition < 20
- Fractured
- 3rd degree burns

**Outputs:**
- `Return Value` (bool) - True if any part is critical

**Example:**
```
Left Leg:
- Condition: 15 (< 20 = critical!)
- Fractured: True

Has Critical Injury
→ Returns: True
```

---

### CALCULATE EFFECT MULTIPLIERS

**What:** Gets all gameplay effect multipliers from body state

**Outputs:**
- `Return Value` (FBodyPartEffectMultipliers) - All multipliers

**Multipliers Included:**
- Movement Speed (leg damage)
- Stamina Drain (leg damage)
- Accuracy (arm damage)
- Weapon Sway (arm damage)
- Max Health (torso damage)
- Sanity Drain Rate (head damage)
- Unconscious Chance (head damage)

**Example:**
```
Legs damaged to 50%:
Calculate Effect Multipliers
→ Movement Speed: 0.65x (35% slower!)
→ Stamina Drain: 1.5x (50% more drain)

Apply to character:
└─ Max Walk Speed * 0.65
```

---

## Body Layer Examples

### Example 1: Combat Damage System

```blueprint
On Projectile Hit:
├─ Determine Body Part from hit bone
    └─ Head? Torso? Leg?
├─ Damage Body Part (HitPart, Damage)
├─ If Critical Hit:
    └─ Set Bleeding Rate (HitPart, 2.0)
└─ Update Character Effects:
    └─ Calculate Effect Multipliers
        └─ Apply movement/accuracy penalties
```

---

### Example 2: Medical Treatment

```blueprint
On Use Bandage (Body Part):
├─ Set Bleeding Rate (BodyPart, 0)
├─ Heal Limb (BodyPart, 10)
└─ Show "Bandaged {BodyPart}" message

On Use Splint (Body Part):
├─ If Fractured:
    ├─ Heal Limb (BodyPart, 20)
    └─ Show "Splinted - move carefully"
└─ Else:
    └─ Show "Not fractured!"

On Use Antibiotics:
├─ For Each Body Part:
    └─ Apply Infection (BodyPart, -50)
        └─ Reduces infection by 50
```

---

### Example 3: Fall Damage with Fractures

```blueprint
On Land After Fall (Velocity):
├─ Fall Damage = Velocity * 0.5
├─ If Damage > 30:
    ├─ Fracture Limb (Left_Leg)
    ├─ Fracture Limb (Right_Leg)
    └─ Show "Legs broken!"
└─ Else:
    └─ Damage Body Part (Left_Leg, Damage)
```

---

### Example 4: Injury Progression

```blueprint
Every Frame:
├─ Get Total Bleeding Rate
    └─ If > 0:
        └─ Change Stat (Blood_Level, -BleedRate * DeltaTime)
├─ For Each Body Part:
    └─ If Infection > 0:
        ├─ Apply Infection (+0.5 * DeltaTime)
        └─ If Infection > 80:
            └─ Apply Status Effect ("Sepsis")
```

---

# Environment Layer System

## What Is the Environment Layer?

**The Environment Layer tracks environmental conditions and their effects on the character.**

**Environmental Stats Tracked:**
1. **Temperature** - Body temperature in Celsius (35-40°C normal)
2. **Wind Speed** - Wind force in m/s
3. **Rain Intensity** - Rainfall amount (0-100)
4. **Snow Intensity** - Snowfall amount (0-100)
5. **Shelter Level** - Protection from elements (0-100)
6. **Clothing Insulation** - Protection from temperature (0-100)
7. **Radiation Level** - Radiation exposure (0-100)

---

## Environmental Stats Explained

### TEMPERATURE (Celsius)

**What:** Character's body temperature

**Normal Range:** 35-40°C (95-104°F)

**Effects:**
```
< 30°C:  Severe hypothermia (unconscious, death risk)
30-35°C: Hypothermia (shivering, stamina drain)
35-40°C: Normal (no effects)
40-42°C: Hyperthermia (fatigue, weakness)
> 42°C:  Heat stroke (organ damage, death risk)
```

**What Changes It:**
- Ambient temperature (hot/cold environment)
- Clothing insulation (warmer in cold)
- Shelter level (protected from elements)
- Wind speed (wind chill)
- Wetness (wet = colder)
- Fire proximity (warms you up)
- Activity level (running = warmer)

**Example:**
```
Winter night:
- Ambient: -10°C
- No shelter: Body temp drops fast!
- With coat (insulation 50): Drops slower
- Near campfire: Temperature stable
```

---

### WIND SPEED (m/s)

**What:** Wind force affecting character

**Effects:**
```
0-5 m/s:    Calm, no effect
6-10 m/s:   Windy, slight cold increase
11-20 m/s:  Strong wind, stamina drain
21-30 m/s:  Very strong, movement penalty
> 30 m/s:   Extreme, can't move straight
```

**Wind Chill:**
```
Temperature: 0°C
Wind: 0 m/s → Feels like: 0°C

Temperature: 0°C
Wind: 20 m/s → Feels like: -12°C!
```

---

### RAIN INTENSITY (0-100)

**What:** How much rain is falling

**Effects:**
```
0:       No rain
1-25:    Light drizzle, slight wetness increase
26-50:   Moderate rain, wetness increases fast
51-75:   Heavy rain, visibility reduced
76-100:  Torrential, severe wetness, cold
```

**Wetness Increase:**
```
No shelter:
- Light rain: +1 wetness/sec
- Heavy rain: +5 wetness/sec
- Torrential: +10 wetness/sec

Under shelter (Shelter Level 80):
- Any rain: +0 wetness (protected!)
```

---

### SNOW INTENSITY (0-100)

**What:** Snowfall amount

**Effects:**
```
Similar to rain but:
- Colder (temperature drops faster)
- Slower wetness increase
- Movement speed penalty
- Visibility reduction
```

---

### SHELTER LEVEL (0-100)

**What:** Protection from weather

**Sources:**
```
0:    Open field (no protection)
30:   Tree cover (slight protection)
60:   Cave entrance (moderate protection)
80:   Building (good protection)
100:  Indoor (complete protection)
```

**What It Protects From:**
- Rain (reduces wetness gain)
- Wind (reduces wind chill)
- Temperature extremes (helps stabilize)
- Radiation (some shelters)

**Example:**
```
Heavy rain outside:
- No shelter: +5 wetness/sec
- Shelter 50: +2 wetness/sec
- Shelter 100: +0 wetness/sec (dry!)
```

---

### CLOTHING INSULATION (0-100)

**What:** Temperature protection from clothing

**Examples:**
```
0:    Naked (no protection)
20:   Light clothes (t-shirt, shorts)
40:   Normal clothes (jeans, jacket)
60:   Winter clothes (coat, gloves)
80:   Arctic gear (heavy coat, thermal)
100:  Extreme cold suit (survive anywhere)
```

**Calculation:**
```
Effective Temperature = Ambient + (Insulation * 0.3)

Ambient: -20°C
Insulation 0:   Effective: -20°C (freezing!)
Insulation 40:  Effective: -8°C (still cold)
Insulation 80:  Effective: +4°C (comfortable!)
```

---

### RADIATION LEVEL (0-100)

**What:** Radioactive exposure

**Effects:**
```
0:       Safe
1-25:    Minor exposure, slow health drain
26-50:   Moderate radiation, status effects
51-75:   Severe radiation, sickness
76-100:  Critical, rapid damage, death
```

**Mechanics:**
```
Exposure Buildup:
- In radiated zone: +1 radiation/sec
- With hazmat suit: +0.1 radiation/sec
- Out of zone: -0.5 radiation/sec (slow recovery)

Effects:
- Radiation > 50: Apply "Radiation Sickness" effect
- Radiation > 75: -20 health per second
```

---

## Environment Layer Functions

### SET AMBIENT TEMPERATURE

**What:** Sets the environment temperature

**Inputs:**
- `Temperature` (float, required) - Celsius **[-50 to +50 typical]**

**Effects:**
- Affects body temperature over time
- Combined with wind/wetness/clothing

**Example:**
```
// Desert during day
Set Ambient Temperature (45.0)
→ Very hot! Body temp rises

// Arctic night
Set Ambient Temperature (-30.0)
→ Freezing! Need heavy clothing
```

---

### SET WIND SPEED

**What:** Sets wind force

**Inputs:**
- `Wind Speed` (float, required) - Meters per second **[0-50]**

**Example:**
```
// Storm
Set Wind Speed (25.0)
→ Strong wind!
→ Temperature feels much colder
→ Movement slightly impeded
```

---

### SET RAIN INTENSITY

**What:** Sets rainfall amount

**Inputs:**
- `Intensity` (float, required) - Rain amount **[0-100]**

**Example:**
```
// Heavy storm
Set Rain Intensity (80.0)
→ Torrential rain!
→ Wetness increases rapidly
→ Need shelter!
```

---

### SET SHELTER LEVEL

**What:** Sets environmental protection

**Inputs:**
- `Shelter Level` (float, required) - Protection amount **[0-100]**

**Example:**
```
// Enter building
Set Shelter Level (90.0)
→ Protected from rain!
→ Wind chill reduced
→ Temperature stabilizes
```

---

### SET CLOTHING INSULATION

**What:** Sets clothing warmth

**Inputs:**
- `Insulation` (float, required) - Warmth level **[0-100]**

**Example:**
```
// Equip winter coat
Set Clothing Insulation (70.0)
→ Much warmer!
→ Can survive colder temps

// Remove coat
Set Clothing Insulation (20.0)
→ Cold again!
```

---

### APPLY RADIATION

**What:** Adds radiation exposure

**Inputs:**
- `Amount` (float, required) - Radiation to add

**Example:**
```
// Enter radioactive zone
Every Second:
└─ Apply Radiation (1.0)
    └─ Radiation builds up!

// Exit zone
Every Second:
└─ Apply Radiation (-0.5)
    └─ Slowly recovers
```

---

### GET EFFECTIVE TEMPERATURE

**What:** Gets "feels like" temperature considering all factors

**Outputs:**
- `Return Value` (float) - Effective temperature in Celsius

**Calculation Includes:**
- Ambient temperature
- Wind chill
- Wetness (wet = colder)
- Clothing insulation
- Shelter level

**Example:**
```
Ambient: 0°C
Wind: 20 m/s
Wetness: 80%
Insulation: 20

Get Effective Temperature
→ Returns: -15°C (feels much colder!)
```

---

### GET EXPOSURE LEVEL

**What:** Overall environmental danger level

**Outputs:**
- `Return Value` (float) - Exposure level **[0-100]**

**Calculation:**
```
Exposure =
  Temperature deviation +
  Wind intensity +
  Wetness +
  Radiation

Higher = More dangerous!
```

**Example:**
```
Freezing storm:
- Temp: -20°C (deviation: 55)
- Wind: 25 m/s
- Wetness: 70
- Radiation: 0

Get Exposure Level
→ Returns: 85 (critical exposure!)
```

---

### IS HYPOTHERMIA RISK

**What:** Check if at risk of hypothermia

**Outputs:**
- `Return Value` (bool) - True if at risk

**Conditions:**
- Body temperature < 35°C
- Or effective temperature < 5°C for extended time

**Example:**
```
Body Temp: 33°C

Is Hypothermia Risk
→ Returns: True (danger!)
```

---

### IS HYPERTHERMIA RISK

**What:** Check if at risk of heat stroke

**Outputs:**
- `Return Value` (bool) - True if at risk

**Conditions:**
- Body temperature > 40°C
- Or effective temperature > 35°C for extended time

---

### CALCULATE WETNESS CHANGE

**What:** Calculates how fast wetness changes

**Outputs:**
- `Return Value` (float) - Wetness change per second

**Considers:**
- Rain intensity
- Snow intensity
- Shelter level
- Near fire (dries you)

**Example:**
```
Heavy rain (80)
Shelter level (30)

Calculate Wetness Change
→ Returns: +3.5 wetness/sec
```

---

## Environment Layer Events

### ON HYPOTHERMIA

```
Fires when: Body temperature < 35°C
Provides: Current Temperature

Use For:
- Apply "Hypothermia" status effect
- Warning messages
- Screen effects (shivering)
```

### ON HYPERTHERMIA

```
Fires when: Body temperature > 40°C
Provides: Current Temperature

Use For:
- Apply "Heatstroke" status effect
- Warning messages
- Exhaustion mechanics
```

### ON RADIATION POISONING

```
Fires when: Radiation level > 75
Provides: Radiation Level

Use For:
- Apply "Radiation Sickness" effect
- Critical warnings
- UI alerts
```

---

## Environment Examples

### Example 1: Weather System

```blueprint
Weather Manager (Tick):
├─ Update Time of Day
├─ Calculate Ambient Temperature
    └─ Day: +25°C, Night: -5°C
├─ Random Weather Events:
    └─ 30% chance rain per hour
        └─ Set Rain Intensity (60.0)
└─ Wind varies by weather
    └─ Set Wind Speed (Random 5-20)
```

---

### Example 2: Shelter Detection

```blueprint
Every Second:
├─ Sphere Overlap (Above Player)
    └─ Hit roof/ceiling?
        ├─ Yes: Set Shelter Level (100)
        └─ No: Set Shelter Level (0)
├─ Or check for trees:
    └─ Under tree? Set Shelter Level (30)
```

---

### Example 3: Temperature Effects

```blueprint
On Environment Changed:
├─ Get Effective Temperature
    ├─ < 5°C:
        ├─ Apply Status Effect ("Cold")
        └─ Change Stat (Energy, -1/sec)
    ├─ > 35°C:
        ├─ Apply Status Effect ("Hot")
        └─ Change Stat (Stamina, -2/sec)
    └─ Normal (5-35°C):
        └─ Remove Effects ("Cold", "Hot")
```

---

### Example 4: Campfire Warmth

```blueprint
Near Campfire:
├─ Distance to fire = 2 meters
├─ Set Ambient Temperature (+10°C bonus)
├─ Calculate Wetness Change
    └─ -5 wetness/sec (drying!)
└─ Show "Warming Up" message
```

---

### Example 5: Radiation Zone

```blueprint
Inside Radiation Zone (Tick):
├─ Apply Radiation (1.0 * DeltaTime)
├─ Get Radiation Level
    └─ If > 50:
        ├─ Apply Status Effect ("Radiation Sickness")
        └─ Change Stat (Health, -2/sec)
└─ Show Geiger Counter UI
    └─ Radiation meter + warning beeps
```

---

# Progression Layer System

## What Is the Progression Layer?

**The Progression Layer handles character advancement through XP, levels, and skills.**

**Components:**
1. **Experience Points (XP)** - Earned from actions
2. **Character Level** - Overall progression
3. **Skill Categories** - Combat, Survival, Crafting, etc.
4. **Individual Skills** - Specific abilities within categories
5. **Skill Prerequisites** - Requirements to unlock skills
6. **XP Multipliers** - Bonuses to XP gain

---

## Progression Concepts Explained

### EXPERIENCE POINTS (XP)

**What:** Currency for character progression

**How to Earn:**
```
Kill Enemy:        +50 XP
Complete Quest:    +500 XP
Craft Item:        +10 XP
Discover Location: +100 XP
Survive Night:     +50 XP
```

**XP to Level Formula:**
```
XP Required = Base * (Level ^ Exponent)

Example (Base: 100, Exponent: 1.5):
Level 1→2: 100 XP
Level 2→3: 282 XP
Level 3→4: 519 XP
Level 10→11: 3,162 XP
```

---

### CHARACTER LEVEL

**What:** Overall character progression

**Effects:**
```
Each Level:
- Gain skill points
- Stat increases (optional)
- Unlock new content
- Higher XP requirements
```

**Example:**
```
Level 1:
- Max Health: 100
- Skill Points: 0
- XP to Next: 100

Level 10:
- Max Health: 145 (+5 per level)
- Skill Points: 9 (1 per level)
- XP to Next: 3,162
```

---

### SKILL CATEGORIES

**What:** Organized groups of related skills

**Default Categories:**
1. **Combat** - Weapon proficiency, critical hits
2. **Survival** - Gathering, crafting, fire starting
3. **Medical** - Healing, surgery, medicine
4. **Stealth** - Sneaking, lockpicking, pickpocketing
5. **Athletics** - Running, swimming, climbing
6. **Social** - Trading, persuasion, leadership

---

### SKILLS

**What:** Individual abilities to unlock and level up

**Skill Properties:**
```
- Name: "Weapon Mastery"
- Category: Combat
- Max Level: 5
- Current Level: 0
- Cost: 1 skill point per level
- Prerequisites: None (or other skills)
- Effects: +10% weapon damage per level
```

**Example Skill Tree:**
```
Combat Category:
├─ Basic Weapon Handling (Max: 3)
    ├─ Effect: +5% damage per level
    └─ Unlocks:
        ├─ Weapon Mastery (Max: 5)
            ├─ Requires: Basic Weapon Handling 3
            └─ Effect: +10% damage per level
        └─ Critical Strikes (Max: 3)
            └─ Effect: +2% crit chance per level
```

---

### PREREQUISITES

**What:** Requirements to unlock skills

**Types:**
```
1. Level Requirement:
   - "Must be level 5"

2. Skill Requirement:
   - "Must have Medical Basics level 2"

3. Stat Requirement:
   - "Must have 15 Intelligence"

4. Quest Requirement:
   - "Must complete 'Training Quest'"
```

**Example:**
```
Skill: Advanced Surgery
Prerequisites:
- Character Level: 10
- Medical Basics: Level 3
- Intelligence: 15
- Quest: "Medical Training" completed
```

---

### XP MULTIPLIERS

**What:** Bonuses to XP gain

**Sources:**
```
Well Rested:     +50% XP
Group Bonus:     +25% XP
Premium Item:    +100% XP
Weekend Event:   +200% XP

Total: +375% XP gain!
```

**Calculation:**
```
Base XP: 100
Multiplier: 3.75x
Earned: 375 XP!
```

---

## Progression Layer Functions

### ADD EXPERIENCE

**What:** Adds XP to character

**Inputs:**
- `Amount` (int32, required) - XP to add
- `Source` (FString, optional) - What gave XP? **["Combat", "Quest", etc.]**

**Effects:**
- Adds XP
- May level up automatically
- Triggers events

**Example:**
```
// Kill enemy
Add Experience (50, "Combat")
→ +50 XP
→ Total: 850/1000 XP

// Kill another
Add Experience (50, "Combat")
→ +50 XP
→ Total: 900/1000 XP
```

**Multiplayer:** Server only, auto-replicates

---

### ADD SKILL LEVEL

**What:** Increases a skill level

**Inputs:**
- `Skill Name` (FName, required) - Which skill? **[Name from skill data]**
- `Levels` (int32, optional, default 1) - How many levels?

**Checks:**
- Enough skill points?
- Prerequisites met?
- Not at max level?

**Example:**
```
// Learn weapon mastery
Add Skill Level ("WeaponMastery", 1)
→ Weapon Mastery: 0 → 1
→ Skill Points: 3 → 2
→ Effect: +10% damage
```

---

### REMOVE SKILL LEVEL (Respec)

**What:** Removes skill levels (refunds points)

**Inputs:**
- `Skill Name` (FName, required) - Which skill?
- `Levels` (int32, optional) - How many to remove?

**Use When:**
- Respec system
- Skill reset items
- Testing/debugging

---

### GET SKILL LEVEL

**What:** Gets current level of a skill

**Inputs:**
- `Skill Name` (FName, required) - Which skill?

**Outputs:**
- `Return Value` (int32) - Current level (0 if not learned)

**Example:**
```
Get Skill Level ("WeaponMastery")
→ Returns: 3 (level 3)

Get Skill Level ("NotLearned")
→ Returns: 0 (not learned)
```

---

### HAS SKILL UNLOCKED

**What:** Checks if skill prerequisites are met

**Inputs:**
- `Skill Name` (FName, required) - Which skill?

**Outputs:**
- `Return Value` (bool) - True if can unlock

**Example:**
```
Advanced Surgery Prerequisites:
- Level 10 (player is 12) ✓
- Medical Basics 3 (player has 2) ✗
- Intelligence 15 (player has 20) ✓

Has Skill Unlocked ("AdvancedSurgery")
→ Returns: False (missing prerequisite)
```

---

### GET AVAILABLE SKILL POINTS

**What:** Gets unspent skill points

**Outputs:**
- `Return Value` (int32) - Skill points available

**Example:**
```
Level 10:
- Total Points Earned: 9 (1 per level)
- Points Spent: 6
- Available: 3

Get Available Skill Points
→ Returns: 3
```

---

### SET XP MULTIPLIER

**What:** Sets XP gain multiplier

**Inputs:**
- `Multiplier` (float, required) - Multiplier value **[1.0 = normal, 2.0 = double]**
- `Source` (FString, optional) - Multiplier name

**Example:**
```
// Well rested buff
Set XP Multiplier (1.5, "WellRested")
→ +50% XP gain!

// Stack with premium item
Set XP Multiplier (2.0, "Premium")
→ Now +150% total XP!
```

---

### GET TOTAL XP MULTIPLIER

**What:** Gets combined XP multiplier

**Outputs:**
- `Return Value` (float) - Total multiplier

**Example:**
```
Multipliers:
- Well Rested: 1.5x
- Premium: 2.0x
- Event: 3.0x

Get Total XP Multiplier
→ Returns: 4.5x (additive: 1.0 + 0.5 + 1.0 + 2.0)
```

---

### GET XP PROGRESS (0-1)

**What:** Gets percentage to next level

**Outputs:**
- `Return Value` (float) - Progress **[0.0-1.0]**

**Example:**
```
Current XP: 750
Next Level: 1000
Progress: 750/1000 = 0.75

Get XP Progress
→ Returns: 0.75 (75% to next level)
```

---

### GET SKILLS BY CATEGORY

**What:** Gets all skills in a category

**Inputs:**
- `Category` (FName, required) - Which category? **["Combat", "Survival", etc.]**

**Outputs:**
- `Return Value` (TArray<FSkillData>) - All skills in category

**Use When:**
- Building skill tree UI
- Showing available skills
- Category filtering

---

### RESET ALL SKILLS (Respec)

**What:** Removes all skill levels, refunds all points

**Inputs:** None

**Effects:**
- All skills → Level 0
- Refund all spent skill points
- Trigger event

**Use When:**
- Respec potion
- Character reset
- Testing

---

## Progression Layer Events

### ON LEVEL UP

```
Fires when: Character gains a level
Provides: New Level, Skill Points Gained

Use For:
- Level up animation
- UI notification
- Sound effects
- Reward player
```

### ON SKILL LEARNED

```
Fires when: Skill level increases
Provides: Skill Name, New Level

Use For:
- Skill animation
- UI update
- Notification
```

### ON SKILL UNLOCKED

```
Fires when: Skill prerequisites met
Provides: Skill Name

Use For:
- UI highlighting
- Notification: "New skill available!"
```

---

## Progression Examples

### Example 1: Kill Enemy XP

```blueprint
On Enemy Killed:
├─ Get Enemy Level
    └─ XP = Enemy Level * 10
├─ Add Experience (XP, "Combat")
└─ Show "+{XP} XP" floating text
```

---

### Example 2: Level Up Rewards

```blueprint
On Level Up (Event):
├─ Add Skill Points (+1)
├─ Heal to Full (all stats 100%)
├─ Play Level Up Effect
    ├─ Particle system
    ├─ Sound effect
    └─ Camera shake
└─ Show UI:
    └─ "LEVEL UP! Level {NewLevel}"
```

---

### Example 3: Skill Tree UI

```blueprint
Create Skill Tree Widget:
├─ Get Skills By Category ("Combat")
    └─ For Each Skill:
        ├─ Create Skill Button
        ├─ Get Skill Level → Show level
        ├─ Has Skill Unlocked → Enable/Disable
        └─ On Button Click:
            └─ Add Skill Level (SkillName, 1)
```

---

### Example 4: XP Boosts

```blueprint
On Rest at Inn:
├─ Apply Status Effect ("WellRested", 8 hours)
└─ Set XP Multiplier (1.5, "WellRested")
    └─ +50% XP for 8 hours!

On Effect Expired ("WellRested"):
└─ Set XP Multiplier (1.0, "WellRested")
    └─ Bonus removed
```

---

### Example 5: Skill Effects

```blueprint
Calculate Damage:
├─ Base Damage: 50
├─ Get Skill Level ("WeaponMastery")
    └─ Level 3 = +30% damage
├─ Get Skill Level ("CriticalStrikes")
    └─ Level 2 = +4% crit chance
└─ Final Damage:
    ├─ 50 * 1.30 = 65
    └─ 4% chance for 130 (crit)
```

---

# How Layers Work Together

## Integration Overview

**All 5 layers can interact and affect each other:**

```
Stat Layer ↔ Status Effect Layer
     ↕              ↕
Body Layer ↔ Environment Layer
     ↕              ↕
      Progression Layer
```

---

## Integration Example 1: Complete Combat System

```blueprint
On Projectile Hit:

1. BODY LAYER - Locational Damage:
├─ Determine Body Part (hit detection)
└─ Damage Body Part (Leg, 40)
    ├─ Condition: 100 → 60
    ├─ Auto-fracture (damage > 35)
    └─ Set Bleeding Rate (Leg, 2.0)

2. STAT LAYER - Health Reduction:
├─ Change Stat (Health, -40)
└─ If (Is Stat Critical? Health):
    └─ Warning UI

3. STATUS EFFECT LAYER - Apply Effects:
├─ Apply Status Effect ("Bleeding", 1 stack)
└─ Apply Status Effect ("Pain", 2 stacks)

4. BODY LAYER - Calculate Penalties:
├─ Calculate Effect Multipliers
    ├─ Movement Speed: 0.3x (fractured leg!)
    └─ Stamina Drain: 2.0x
└─ Apply to character movement

5. PROGRESSION LAYER - Award XP:
└─ Add Experience (50, "Combat")

6. ENVIRONMENT LAYER - Exposure Risk:
└─ If bleeding + cold weather:
    └─ Hypothermia risk increases
```

---

## Integration Example 2: Medical Treatment

```blueprint
On Use Advanced Medical Kit:

1. STAT LAYER - Restore Health:
└─ Change Stat (Health, +50)

2. BODY LAYER - Heal Injuries:
├─ Heal Limb (All Parts, 30)
├─ Set Bleeding Rate (All Parts, 0)
└─ Apply Infection (All Parts, -50)

3. STATUS EFFECT LAYER - Remove Debuffs:
├─ Remove Effect ("Bleeding")
├─ Remove Effect ("Pain")
└─ Remove Effects By Tag ("Medical.Curable")

4. STATUS EFFECT LAYER - Apply Buffs:
└─ Apply Status Effect ("HealthRegen", 60 seconds)
    └─ +5 health per second

5. PROGRESSION LAYER - Skill Check:
├─ Get Skill Level ("Medical")
    └─ Level 3+ = Bonus healing
        └─ Heal Limb (All Parts, +20 bonus!)
```

---

## Integration Example 3: Environmental Survival

```blueprint
Survival System (Tick):

1. ENVIRONMENT LAYER - Calculate Conditions:
├─ Get Effective Temperature → -15°C
├─ Calculate Wetness Change → +5/sec (rain)
└─ Get Exposure Level → 75 (high!)

2. STAT LAYER - Exposure Effects:
├─ If Exposure > 50:
    ├─ Change Stat (Energy, -1/sec)
    ├─ Change Stat (Body Temperature, -0.5/sec)
    └─ Change Stat (Wetness, +5/sec)

3. ENVIRONMENT LAYER - Check Risks:
├─ Is Hypothermia Risk? → True
    └─ Apply Status Effect ("Hypothermia")
        └─ Stamina regen -50%
        └─ Movement speed -30%

4. BODY LAYER - Cold Damage:
├─ If Body Temp < 30°C:
    └─ Damage Body Part (All Parts, 1/sec)
        └─ Frostbite damage!

5. STATUS EFFECT LAYER - Stacking Effects:
└─ Every 30 seconds in cold:
    └─ Apply Status Effect ("Frostbite", +1 stack)
        └─ Max 5 stacks = severe damage

6. STAT LAYER - Death Check:
└─ If (Is Stat at Zero? Body Temperature):
    └─ Player Death (hypothermia)
```

---

## Integration Example 4: Skill-Based Crafting

```blueprint
On Craft Item:

1. PROGRESSION LAYER - Check Skill:
├─ Get Skill Level ("Crafting") → Level 4
└─ Has Skill Unlocked ("AdvancedCrafting") → True

2. PROGRESSION LAYER - Quality Bonus:
├─ Item Quality = Base + (Skill Level * 10%)
    └─ Level 4 = +40% quality!

3. STAT LAYER - Resource Cost:
├─ Change Stat (Energy, -20)
└─ If (Get Skill Level "Efficiency") > 2:
    └─ Energy cost reduced by 50%

4. PROGRESSION LAYER - XP Reward:
└─ Add Experience (10 * Item Tier, "Crafting")

5. STATUS EFFECT LAYER - Crafting Buffs:
├─ If (Has Effect "Crafting Inspiration"):
    ├─ Doubled success chance
    └─ 50% faster crafting

6. PROGRESSION LAYER - Level Up Check:
└─ On Level Up (Event):
    └─ Unlock new crafting recipes
```

---

## Integration Example 5: Radiation + Medical

```blueprint
Radiation Poisoning System:

1. ENVIRONMENT LAYER - Exposure:
├─ In Radiation Zone (100 radiation)
└─ Every Second:
    └─ Apply Radiation (1.0)

2. STATUS EFFECT LAYER - Radiation Sickness:
├─ If Radiation > 50:
    └─ Apply Status Effect ("Radiation Sickness", Stacks: Radiation/10)
        └─ 80 radiation = 8 stacks!

3. STAT LAYER - Health Drain:
├─ Get Effect Stacks ("Radiation Sickness")
    └─ Stacks * 2 = health drain per sec
        └─ 8 stacks = -16 health/sec!

4. BODY LAYER - Organ Damage:
├─ Radiation damages internal organs:
    ├─ Damage Body Part (Torso, 0.5/sec)
    └─ Apply Infection (All Parts, +1/sec)

5. PROGRESSION LAYER - Survival XP:
└─ If Survive 60 seconds in rad zone:
    └─ Add Experience (100, "Survival")

6. STAT LAYER + STATUS EFFECT - Treatment:
└─ On Use Rad-Away:
    ├─ Apply Radiation (-50)
    ├─ Remove Effect ("Radiation Sickness")
    └─ Apply Effect ("Rad-X Protection", 5 min)
```

---

## Integration Example 6: Complete Character State

```blueprint
Update Character (Every Frame):

1. STAT LAYER - Core Stats:
├─ Health: 45/100 (critical!)
├─ Stamina: 20/100
└─ Body Temperature: 33°C (hypothermia)

2. BODY LAYER - Injuries:
├─ Left Leg: Fractured, Bleeding 2/sec, Condition 25
├─ Right Arm: Infection 40, Pain 60
└─ Calculate Multipliers:
    ├─ Movement: 0.3x
    └─ Accuracy: 0.5x

3. STATUS EFFECTS - Active:
├─ Bleeding (3 stacks) → -6 health/sec
├─ Hypothermia (1 stack) → -50% stamina regen
├─ Pain (2 stacks) → Screen blur
└─ Infection Build-up → Approaching sepsis

4. ENVIRONMENT - Conditions:
├─ Temperature: -10°C
├─ Rain: 60 (heavy)
├─ Wetness: 85% (soaked!)
└─ Shelter: 0 (exposed!)

5. PROGRESSION - Character:
├─ Level: 8
├─ XP: 1,580/2,000
├─ Skill Points: 2 available
└─ Medical skill (level 3) helps healing

6. OVERALL STATE - Critical:
├─ Death in 7 seconds (bleeding + hypothermia)
├─ Need immediate:
    ├─ Bandage (stop bleeding)
    ├─ Shelter (warm up)
    └─ Medical kit (restore health)
```

---

## Design Patterns for Layer Integration

### Pattern 1: Trigger Chain
```
One action triggers multiple layers sequentially
Example: Combat hit → Body damage → Stat reduction → Status effect → XP gain
```

### Pattern 2: Feedback Loop
```
Layers affect each other in cycle
Example: Cold → Lower body temp → Hypothermia effect → Reduced energy → Can't generate heat → Colder!
```

### Pattern 3: Skill Modifier
```
Progression layer modifies other layer effectiveness
Example: Medical skill level → Better healing → Faster body part recovery
```

### Pattern 4: Environmental Pressure
```
Environment creates challenges, other layers provide solutions
Example: Radiation zone → Status effects → Use medical skill + items to survive
```

### Pattern 5: Comprehensive State
```
All layers combine for overall character condition
Example: Character state UI shows: Stats + Body + Effects + Environment + Level
```

---

## Summary

**5 Layers Working Together:**

1. **Stat Layer** - Core character stats (health, stamina, etc.)
2. **Status Effect Layer** - Buffs, debuffs, conditions
3. **Body Layer** - Locational damage and injuries
4. **Environment Layer** - Weather, temperature, radiation
5. **Progression Layer** - XP, levels, skills

**Each layer is independent but can interact with others for deep gameplay!**

**Key Takeaways:**
- ✅ Use layers individually for simple games
- ✅ Combine layers for complex survival/RPG games
- ✅ All systems replicate in multiplayer automatically
- ✅ All systems use enum-based interfaces (no text typing!)
- ✅ Extensively documented with examples
- ✅ Blueprint-friendly with clear tooltips
- ✅ Performance-optimized for production games

**Happy Game Developing! 🎮**
