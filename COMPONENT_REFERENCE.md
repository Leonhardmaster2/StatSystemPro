# Component Reference - All Components & Functions Explained

## 📋 Table of Contents
1. [Stat Component](#stat-component)
2. [Body Component](#body-component)
3. [Status Effect Component](#status-effect-component)
4. [Environment Component](#environment-component)
5. [Progression Component](#progression-component)
6. [Time Component](#time-component)

---

# Stat Component

## What Is It?
Manages numeric character values like health, stamina, hunger, etc.

## When to Use?
- Health/damage systems
- Stamina/energy mechanics
- Survival needs (hunger, thirst)
- Any numeric stat tracking

---

## All Stat Component Functions Explained

### **Basic Getters**

#### `Get Stat Value`
**What:** Get current value of a stat (e.g., current health)
**Inputs:**
- Stat Type (enum dropdown) - Which stat?
**Outputs:**
- Float - Current value (e.g., 75)
**Example:**
```
Get Stat Value (Health_Core) → Returns 75
```

#### `Get Stat Maximum`
**What:** Get maximum value of a stat
**Inputs:**
- Stat Type (enum) - Which stat?
**Outputs:**
- Float - Max value (e.g., 100)
**Example:**
```
Get Stat Maximum (Health_Core) → Returns 100
Player has 75/100 health
```

#### `Get Stat Percentage (0-1)`
**What:** Get stat as decimal percentage
**Inputs:**
- Stat Type (enum)
**Outputs:**
- Float (0-1) - Percentage
**Example:**
```
Get Stat Percentage (Health_Core) → Returns 0.75
Perfect for progress bars!
```

#### `Get Stat Percentage (0-100)`
**What:** Get stat as whole number percentage
**Inputs:**
- Stat Type (enum)
**Outputs:**
- Float (0-100) - Percentage
**Example:**
```
Get Stat Percentage 100 (Health_Core) → Returns 75
Display: "Health: 75%"
```

---

### **Modification Functions**

#### `Change Stat (Add/Subtract)`
**What:** Add or subtract from a stat
**Inputs:**
- Stat Type (enum) - Which stat?
- Amount (float) - How much? (negative = decrease, positive = increase)
- Source (name) - For debugging (e.g., "Combat", "Food")
- Reason Tag (gameplay tag) - Optional categorization
**Effects:**
- Changes stat value
- Clamps to 0-Max
- Fires OnStatChanged event
- Auto-replicates in multiplayer
**Example:**
```
Change Stat (Health_Core, -25, "Combat", none)
→ Player takes 25 damage
→ Health: 100 → 75

Change Stat (Hunger, +30, "Food", none)
→ Player eats food
→ Hunger: 40 → 70
```

#### `Set Stat to Exact Value`
**What:** Override stat to specific value (ignores current)
**Inputs:**
- Stat Type (enum)
- New Value (float)
**Example:**
```
Set Stat Value (Health_Core, 50)
→ Health becomes exactly 50 (regardless of previous value)
Use for precise control!
```

#### `Set Stat Maximum`
**What:** Change a stat's max value (for buffs/debuffs)
**Inputs:**
- Stat Type (enum)
- New Max Value (float)
**Effects:**
- Changes max value
- Auto-clamps current value to new max
- Fires OnStatMaxChanged event
**Example:**
```
Player has 75/100 health

Set Stat Maximum (Health_Core, 150)
→ Health now 75/150 (buff!)
→ Can heal to 150 now

Set Stat Maximum (Health_Core, 50)
→ Health now 50/50 (debuff! auto-clamped)
```

#### `Set Regeneration Rate`
**What:** Set how fast a stat regenerates or decays
**Inputs:**
- Stat Type (enum)
- Rate (float) - Units per second (positive = regen, negative = decay)
**Example:**
```
Set Regeneration Rate (Health_Core, 5.0)
→ Regen +5 health per second

Set Regeneration Rate (Hunger, -1.0)
→ Lose -1 hunger per second (decay)
```

---

### **Check Functions**

#### `Is Stat at Max?`
**What:** Check if stat is full
**Inputs:**
- Stat Type (enum)
**Outputs:**
- Bool - True if at maximum
**Example:**
```
Is Stat at Max? (Health_Core)
→ Returns True if health = 100/100
→ Returns False if health = 75/100

Use for: "Already at full health!" messages
```

#### `Is Stat at Zero?`
**What:** Check if stat is empty
**Inputs:**
- Stat Type (enum)
**Outputs:**
- Bool - True if at zero
**Example:**
```
Is Stat at Zero? (Health_Core)
→ Returns True if health = 0
→ Trigger death!

Is Stat at Zero? (Stamina)
→ Returns True if stamina = 0
→ Can't sprint!
```

#### `Is Stat Critical?`
**What:** Check if stat is below critical threshold
**Inputs:**
- Stat Type (enum)
**Outputs:**
- Bool - True if below threshold (default 15%)
**Example:**
```
Is Stat Critical? (Health_Core)
→ Returns True if health < 15%
→ Show warning UI!
→ Play heartbeat sound!
```

#### `Is Any Stat Critical? (All Stats)`
**What:** Check if ANY stat is critical
**Outputs:**
- Bool - True if any stat below threshold
**Example:**
```
Is Any Stat Critical?
→ Checks ALL 16 stats
→ Returns True if ANY are below 15%
Use for: Overall danger indicator
```

#### `Is Any Stat Critical in Category?`
**What:** Check if any stat in category is critical
**Inputs:**
- Category (enum) - Which group? (Core, Survival, Environmental, etc.)
**Outputs:**
- Bool - True if any in category are critical
**Example:**
```
Is Any Critical in Category (Survival)
→ Checks only: Hunger, Thirst, Fatigue
→ Returns True if any < 15%

Is Any Critical in Category (Core)
→ Checks only: Health, Stamina, Energy
→ Good for combat checks!
```

#### `Is Any Stat Critical in List?`
**What:** Check specific stats you choose
**Inputs:**
- Stats to Check (array of stat types)
**Outputs:**
- Bool - True if any in list are critical
**Example:**
```
Is Any Critical in List ([Health_Core, BloodLevel])
→ Checks ONLY those two stats
→ Ignores everything else
→ Perfect for custom checks!
```

---

### **Advanced Getters**

#### `Get Lowest Stat (All Stats)`
**What:** Find which stat is weakest globally
**Outputs:**
- Stat Type (enum) - The weakest stat
- Percentage (float) - Its percentage
**Example:**
```
Player stats:
- Health: 90%
- Hunger: 25% ← LOWEST!
- Stamina: 80%

Get Lowest Stat
→ Returns: Hunger, 0.25
Use for: AI deciding what to restore
```

#### `Get Highest Stat (All Stats)`
**What:** Find which stat is strongest globally
**Outputs:**
- Stat Type - The strongest stat
- Percentage - Its percentage
**Example:**
```
Get Highest Stat
→ Returns stat in best condition
```

#### `Get Lowest Stat in Category`
**What:** Find weakest stat in a category
**Inputs:**
- Category (enum)
**Outputs:**
- Stat Type
- Percentage
**Example:**
```
Get Lowest Stat in Category (Survival)
→ Checks: Hunger, Thirst, Fatigue
→ Returns whichever is lowest
```

#### `Get Highest Stat in Category`
**What:** Find strongest stat in a category
**Inputs:**
- Category (enum)
**Outputs:**
- Stat Type
- Percentage
**Example:**
```
Get Highest Stat in Category (Core)
→ Returns best of: Health, Stamina, Energy
```

#### `Get Stats Below Threshold Count`
**What:** Count how many stats are below a threshold
**Inputs:**
- Threshold (float 0-1) - Percentage limit
**Outputs:**
- Int32 - Count of stats below threshold
**Example:**
```
Get Stats Below Threshold Count (0.3)
→ Returns: 5 (5 stats are below 30%)
Use for: "5 stats are critical!" warning
```

#### `Get Average Stat Health`
**What:** Get overall average of all stats
**Outputs:**
- Float (0-1) - Average percentage
**Example:**
```
Get Average Stat Health
→ Returns 0.65 (average 65% across all stats)
Use for: Overall health indicator
```

---

### **Batch Operations**

#### `Restore All Stats in Category`
**What:** Add amount to all stats in a category
**Inputs:**
- Category (enum)
- Amount (float)
**Example:**
```
Player rests at campfire:
Restore All Stats in Category (Survival, 50)
→ Hunger +50
→ Thirst +50
→ Fatigue +50
All survival stats improved!
```

#### `Restore Stats in List`
**What:** Add amount to specific stats you choose
**Inputs:**
- Stats to Restore (array)
- Amount (float)
**Example:**
```
Water bottle item:
Restore Stats in List ([Thirst, Energy], 40)
→ Thirst +40
→ Energy +40
→ Other stats unchanged
```

#### `Restore All Stats to Max`
**What:** Set ALL stats to maximum
**Example:**
```
Restore All Stats to Max
→ ALL stats = 100%
Use for: Full rest, respawn, cheat code
```

#### `Set All Stats to Value`
**What:** Set all stats to exact value
**Inputs:**
- Value (float)
**Example:**
```
Set All Stats to Value (50)
→ ALL stats = 50
Use for: Testing, debug
```

#### `Set Category Stats to Value`
**What:** Set category stats to value
**Inputs:**
- Category (enum)
- Value (float)
**Example:**
```
Set Category Stats to Value (Core, 100)
→ Health = 100
→ Stamina = 100
→ Energy = 100
```

---

### **Unique Functions**

#### `Transfer Stat Value`
**What:** Move value from one stat to another
**Inputs:**
- From Stat (enum)
- To Stat (enum)
- Amount (float)
**Example:**
```
Berserker Rage ability:
Transfer Stat Value (Health_Core, Stamina, 30)
→ Health -30
→ Stamina +30
→ Sacrifice health for stamina boost!

Life Tap spell:
Transfer Stat Value (Sanity, Energy, 50)
→ Trade sanity for magical energy!
```

#### `Compare Stats (A > B?)`
**What:** Check if one stat is higher than another
**Inputs:**
- Stat A (enum)
- Stat B (enum)
**Outputs:**
- Bool - True if A percentage > B percentage
**Example:**
```
Is Stat Greater Than (Health_Core, Stamina)
→ Health 80%, Stamina 50%
→ Returns True
```

---

# Body Component

## What Is It?
Tracks damage to specific body parts (head, torso, arms, legs)

## When to Use?
- Locational damage systems
- Medical/healing mechanics
- Realistic injury simulation
- Armor/protection per body part

---

## All Body Component Functions Explained

### **Damage Functions**

#### `Damage Body Part`
**What:** Damage a specific body part
**Inputs:**
- Body Part (enum dropdown) - Head, Torso, Left_Arm, Right_Arm, Left_Leg, Right_Leg
- Damage (float) - How much damage?
**Effects:**
- Reduces body part condition
- Increases pain
- May auto-fracture if damage is severe (condition < 20%)
- Fires OnBodyPartDamaged event
**Example:**
```
Player shot in leg:
Damage Body Part (Left_Leg, 40)
→ Left Leg condition: 100 → 60
→ Pain level increases
→ Movement speed reduced (leg damaged!)

Headshot:
Damage Body Part (Head, 80)
→ Head condition: 100 → 20 (critical!)
→ Vision blurred, aim shaky
```

#### `Fracture Limb`
**What:** Manually fracture (break) a body part
**Inputs:**
- Body Part (enum)
**Effects:**
- Sets fractured = true
- Adds +50 pain
- Severe penalties:
  - Fractured leg: -70% movement speed
  - Fractured arm: -80% weapon accuracy, can't use two-handed weapons
**Example:**
```
Fall from height:
Fracture Limb (Left_Leg)
→ Leg broken!
→ Movement: 0.3x normal speed
→ Severe pain
→ Need splint/medical treatment
```

#### `Set Bleeding Rate`
**What:** Set how fast a body part bleeds
**Inputs:**
- Body Part (enum)
- Rate (float) - Blood loss per second
**Effects:**
- Body part bleeds at specified rate
- Total bleeding = sum of all body parts
- Affects Blood_Level stat
**Example:**
```
Stabbed in arm:
Set Bleeding Rate (Right_Arm, 3.0)
→ Lose 3 blood per second from arm

Apply bandage:
Set Bleeding Rate (Right_Arm, 0.5)
→ Reduced to 0.5/sec (still bleeding slowly)

Apply advanced bandage:
Set Bleeding Rate (Right_Arm, 0)
→ Bleeding stopped!
```

#### `Set Burn Level`
**What:** Set burn severity on body part
**Inputs:**
- Body Part (enum)
- Burn Level (enum) - None, First_Degree, Second_Degree, Third_Degree
**Effects:**
- None: No burns
- First Degree: Minor (+10 pain)
- Second Degree: Serious (+30 pain, blisters)
- Third Degree: Severe (+50 pain, permanent damage risk)
**Example:**
```
Walk through fire:
Set Burn Level (Torso, Second_Degree)
→ Serious burns
→ Pain +30
→ Infection risk increased
→ Need burn treatment
```

#### `Apply Infection`
**What:** Add infection to a body part
**Inputs:**
- Body Part (enum)
- Infection Amount (float) - How much to add (0-100)
**Effects:**
- Increases infection rate
- Infection grows over time if untreated
- At 80+ infection: Risk of sepsis
**Example:**
```
Dirty wound:
Apply Infection (Left_Leg, 20)
→ Infection starts at 20

Over time without treatment:
→ Infection grows: 20 → 30 → 40 → 50...
→ At 80: Apply "Sepsis" status effect (critical!)

Use antibiotics:
Apply Infection (Left_Leg, -50)
→ Reduces infection by 50
```

---

### **Healing Functions**

#### `Heal Limb`
**What:** Restore condition to a body part
**Inputs:**
- Body Part (enum)
- Heal Amount (float)
**Effects:**
- Increases condition
- Reduces pain (by 30% of heal amount)
**Example:**
```
Use medical kit on damaged leg:
Heal Limb (Left_Leg, 30)
→ Condition: 40 → 70
→ Pain: 50 → 41 (reduced by 9)
→ Improved mobility!
```

---

### **Query Functions**

#### `Get Body Part State`
**What:** Get complete info about a body part
**Inputs:**
- Body Part (enum)
**Outputs:**
- FBodyPartState struct containing:
  - Condition (float 0-100)
  - Max Condition (float)
  - Is Fractured (bool)
  - Bleeding Rate (float per second)
  - Burn Level (enum)
  - Infection Rate (float 0-100)
  - Pain Level (float 0-100)
**Example:**
```
Get Body Part State (Left_Leg)
→ Returns struct:
  - Condition: 30/100 (badly damaged!)
  - Fractured: True
  - Bleeding: 2.0 per second
  - Burn Level: None
  - Infection: 40 (moderate)
  - Pain: 60 (high)
```

#### `Get Total Bleeding Rate`
**What:** Sum of bleeding from ALL body parts
**Outputs:**
- Float - Total blood loss per second
**Example:**
```
Left Arm bleeding: 2.0/sec
Right Leg bleeding: 3.0/sec
Torso bleeding: 1.0/sec

Get Total Bleeding Rate
→ Returns: 6.0 per second (critical!)
→ Blood_Level stat decreases 6 per second
```

#### `Get Total Pain Level`
**What:** Average pain across all body parts
**Outputs:**
- Float (0-100) - Average pain
**Effects of Pain:**
- Pain < 20: No effect
- Pain 20-50: Slight accuracy reduction
- Pain 50-80: Heavy penalties
- Pain > 80: Screen shake, can't aim
**Example:**
```
Head pain: 80 (severe headache!)
Legs pain: 40 each
Other parts: 10

Get Total Pain Level
→ Returns: 33.3 (average pain)
```

#### `Has Critical Injury`
**What:** Check if ANY body part is critically damaged
**Outputs:**
- Bool - True if any part is critical
**Critical Conditions:**
- Condition < 20
- Fractured = True
- 3rd degree burns
**Example:**
```
Left Leg:
- Condition: 15 (critical!)
- Fractured: True

Has Critical Injury
→ Returns: True
→ Player needs immediate medical attention!
```

#### `Calculate Effect Multipliers`
**What:** Get gameplay penalties from body damage
**Outputs:**
- FBodyPartEffectMultipliers struct:
  - Movement Speed multiplier (leg damage)
  - Stamina Drain multiplier (leg damage)
  - Weapon Accuracy multiplier (arm damage)
  - Weapon Sway multiplier (arm damage)
  - Max Health multiplier (torso damage)
  - Sanity Drain multiplier (head damage)
  - Unconscious Chance (head damage)
**Example:**
```
Legs damaged to 50% condition:

Calculate Effect Multipliers
→ Returns:
  - Movement Speed: 0.65x (35% slower!)
  - Stamina Drain: 1.5x (50% more drain)

Apply to character:
→ Max Walk Speed * 0.65 = slower movement
→ Sprint stamina cost * 1.5 = tires faster
```

---

# Status Effect Component

## What Is It?
Manages buffs, debuffs, and conditions (poison, bleeding, speed boost, etc.)

## When to Use?
- Temporary buffs/debuffs
- Damage over time (poison, bleeding)
- Environmental conditions (cold, wet)
- Power-ups and abilities

---

## All Status Effect Functions Explained

### **Apply/Remove Functions**

#### `Apply Status Effect`
**What:** Apply an effect by ID from data table
**Inputs:**
- Effect ID (name) - Which effect? (from data table row name)
- Stacks (int32) - How many stacks? (default 1)
**Outputs:**
- Bool - True if applied successfully
**Example:**
```
Player hit by poison arrow:
Apply Status Effect ("Poison", 1)
→ Poison applied, 1 stack
→ -2 health per second

Hit again:
Apply Status Effect ("Poison", 1)
→ Now 2 stacks of poison
→ -4 health per second (stacks multiply!)

Speed boost power-up:
Apply Status Effect ("SpeedBoost", 1)
→ +50% movement speed for 30 seconds
```

#### `Remove Effect`
**What:** Remove a specific effect
**Inputs:**
- Effect ID (name)
**Outputs:**
- Bool - True if removed
**Example:**
```
Use antidote:
Remove Effect ("Poison")
→ Poison cleared!
→ All stacks removed
```

#### `Remove Effects By Tag`
**What:** Remove all effects with a gameplay tag
**Inputs:**
- Tag (gameplay tag)
**Outputs:**
- Int32 - How many effects removed
**Example:**
```
Use cleansing item:
Remove Effects By Tag ("Effect.Negative")
→ Removes ALL debuffs at once!
→ Returns: 3 (removed 3 debuffs)
```

#### `Clear All Effects`
**What:** Remove EVERYTHING
**Example:**
```
On player death:
Clear All Effects
→ All buffs and debuffs removed
→ Clean slate for respawn
```

---

### **Query Functions**

#### `Has Effect`
**What:** Check if an effect is active
**Inputs:**
- Effect ID (name)
**Outputs:**
- Bool - True if active
**Example:**
```
Has Effect ("Poison")
→ Returns: True (player is poisoned)
→ Show poison icon in UI!

Has Effect ("SpeedBoost")
→ Returns: False (no speed boost)
```

#### `Get Effect Stacks`
**What:** Get current stack count
**Inputs:**
- Effect ID (name)
**Outputs:**
- Int32 - Current stacks (0 if not active)
**Example:**
```
Get Effect Stacks ("Poison")
→ Returns: 3 (3 stacks of poison!)
→ UI: Show "Poison x3"

Get Effect Stacks ("NotActive")
→ Returns: 0 (no effect)
```

#### `Get Effect Time Remaining`
**What:** Get time left for temporary effect
**Inputs:**
- Effect ID (name)
**Outputs:**
- Float - Seconds remaining (0 if not active)
**Example:**
```
Get Effect Time Remaining ("SpeedBoost")
→ Returns: 7.5 (7.5 seconds left)
→ UI: Show countdown timer
```

---

# Environment Component

## What Is It?
Tracks environmental conditions (temperature, weather, radiation)

## When to Use?
- Weather systems
- Temperature mechanics
- Environmental hazards
- Survival games

---

## All Environment Functions Explained

### **Environment Setters**

#### `Set Ambient Temperature`
**What:** Set the environment temperature
**Inputs:**
- Temperature (float) - Celsius (-50 to +50 typical)
**Effects:**
- Affects body temperature over time
- Combined with wind/wetness/clothing/shelter
**Example:**
```
Desert during day:
Set Ambient Temperature (45.0)
→ Very hot! Body temp rises
→ Risk of heatstroke

Arctic night:
Set Ambient Temperature (-30.0)
→ Freezing! Need heavy clothing
→ Risk of hypothermia
```

#### `Set Wind Speed`
**What:** Set wind force
**Inputs:**
- Wind Speed (float) - Meters per second (0-50)
**Effects:**
- Wind chill (makes cold feel colder)
- Movement impediment at high speeds
**Example:**
```
Storm:
Set Wind Speed (25.0)
→ Strong wind
→ Temperature feels -10°C colder (wind chill!)
→ Slight movement penalty
```

#### `Set Rain Intensity`
**What:** Set rainfall amount
**Inputs:**
- Intensity (float 0-100)
**Effects:**
- Increases wetness (unless sheltered)
- Visibility reduction
- Temperature feels colder when wet
**Example:**
```
Heavy storm:
Set Rain Intensity (80.0)
→ Torrential rain!
→ Wetness increases +5 per second (without shelter)
→ Need to find cover!
```

#### `Set Shelter Level`
**What:** Set protection from elements
**Inputs:**
- Shelter Level (float 0-100)
**Sources:**
- 0: Open field (no protection)
- 30: Under tree
- 60: Cave entrance
- 80: Building
- 100: Indoor (complete protection)
**Effects:**
- Reduces rain wetness gain
- Reduces wind chill
- Helps stabilize temperature
**Example:**
```
Enter building:
Set Shelter Level (90.0)
→ Protected from rain!
→ Wind chill reduced
→ Temperature stabilizes
```

#### `Set Clothing Insulation`
**What:** Set warmth from clothing
**Inputs:**
- Insulation (float 0-100)
**Examples:**
- 0: Naked
- 20: Light clothes
- 40: Normal clothes
- 60: Winter clothes
- 80: Arctic gear
- 100: Extreme cold suit
**Example:**
```
Equip winter coat:
Set Clothing Insulation (70.0)
→ Much warmer!
→ Can survive in -20°C comfortably

Remove coat:
Set Clothing Insulation (20.0)
→ Cold again!
→ Need to find warmth
```

#### `Apply Radiation`
**What:** Add or remove radiation exposure
**Inputs:**
- Amount (float) - Radiation to add (negative to remove)
**Example:**
```
Enter radioactive zone:
Every second:
Apply Radiation (1.0)
→ Radiation builds up!

Exit zone + use Rad-Away:
Apply Radiation (-50.0)
→ Radiation reduced by 50
```

---

### **Environment Getters**

#### `Get Effective Temperature`
**What:** Get "feels like" temperature
**Outputs:**
- Float - Effective temperature in Celsius
**Calculation Includes:**
- Ambient temperature
- Wind chill
- Wetness (wet = colder)
- Clothing insulation
- Shelter level
**Example:**
```
Ambient: 0°C
Wind: 20 m/s (strong wind)
Wetness: 80% (soaked!)
Insulation: 20 (light clothes)

Get Effective Temperature
→ Returns: -15°C (feels MUCH colder!)
```

#### `Get Exposure Level`
**What:** Overall environmental danger level
**Outputs:**
- Float (0-100) - Danger level
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
→ Player in severe danger!
```

#### `Is Hypothermia Risk`
**What:** Check if at risk of hypothermia
**Outputs:**
- Bool - True if at risk
**Conditions:**
- Body temperature < 35°C
- Or effective temperature < 5°C for extended time
**Example:**
```
Body Temp: 33°C (too cold!)

Is Hypothermia Risk
→ Returns: True
→ Apply "Hypothermia" status effect
→ Show warning UI
```

#### `Is Hyperthermia Risk`
**What:** Check if at risk of heatstroke
**Outputs:**
- Bool - True if at risk
**Conditions:**
- Body temperature > 40°C
- Or effective temperature > 35°C for extended time

#### `Calculate Wetness Change`
**What:** How fast wetness changes
**Outputs:**
- Float - Wetness change per second
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
→ Returns: +3.5 wetness per second
→ Will be soaked in ~30 seconds!
```

---

# Progression Component

## What Is It?
Character advancement (XP, levels, skills)

## When to Use?
- RPG systems
- Skill trees
- Character progression
- Leveling mechanics

---

## All Progression Functions Explained

### **XP & Leveling**

#### `Add Experience`
**What:** Give XP to character
**Inputs:**
- Amount (int32) - How much XP?
- Source (string) - What gave XP? ("Combat", "Quest", etc.)
**Effects:**
- Adds XP
- May trigger level up automatically
- Fires OnLevelUp event if leveled
**Example:**
```
Kill enemy:
Add Experience (50, "Combat")
→ +50 XP
→ Total: 850/1000 XP

Kill another:
Add Experience (50, "Combat")
→ +50 XP
→ Total: 900/1000 XP

Complete quest:
Add Experience (200, "Quest")
→ +200 XP
→ Total: 1100/1000 → LEVEL UP!
→ Now level 2, XP: 100/282
```

---

### **Skill Functions**

#### `Add Skill Level`
**What:** Increase a skill level (spend skill point)
**Inputs:**
- Skill Name (FName) - Which skill?
- Levels (int32) - How many levels? (default 1)
**Checks:**
- Enough skill points?
- Prerequisites met?
- Not at max level?
**Example:**
```
Player has 3 skill points

Add Skill Level ("WeaponMastery", 1)
→ Weapon Mastery: 0 → 1
→ Skill Points: 3 → 2
→ Effect: +10% weapon damage
```

#### `Get Skill Level`
**What:** Get current level of a skill
**Inputs:**
- Skill Name (FName)
**Outputs:**
- Int32 - Current level (0 if not learned)
**Example:**
```
Get Skill Level ("WeaponMastery")
→ Returns: 3 (level 3)

Get Skill Level ("NotLearned")
→ Returns: 0 (haven't learned)
```

---

# Time Component

## What Is It?
Global time management (day/night cycle, seasons)

## When to Use?
- Day/night cycle
- Time-based events
- Season systems
- Survival time tracking

---

## All Time Component Functions Explained

### **Time Control**

#### `Set Time`
**What:** Jump to specific time
**Inputs:**
- Day (int32)
- Hour (int32 0-23)
- Minute (int32 0-59)
**Example:**
```
Set Time (10, 14, 30)
→ Jump to Day 10, 2:30 PM
Use for: Time skip, debug
```

#### `Advance Time By Hours`
**What:** Skip forward in time
**Inputs:**
- Hours (float)
**Example:**
```
Player sleeps:
Advance Time By Hours (8.0)
→ Skip 8 hours
→ If was 22:00, now 06:00 (next day)
```

#### `Pause Time` / `Resume Time`
**What:** Freeze/unfreeze time
**Example:**
```
Open menu:
Pause Time
→ Time stops

Close menu:
Resume Time
→ Time resumes
```

#### `Set Time Speed Multiplier`
**What:** Change how fast time passes
**Inputs:**
- Multiplier (float)
  - 1.0 = Normal
  - 2.0 = Double speed
  - 0.5 = Half speed
**Example:**
```
Set Time Speed Multiplier (3.0)
→ Time passes 3x faster
→ 1 real minute = 3 game hours!
```

---

### **Time Getters**

#### `Get Current Day`
**Outputs:** Int32 - Day number (starts at 1)

#### `Get Current Hour`
**Outputs:** Int32 - Hour (0-23)

#### `Get Current Minute`
**Outputs:** Int32 - Minute (0-59)

#### `Get Time String`
**Outputs:** String - "Day 5, 14:30"

#### `Get Clock Time String`
**Outputs:** String - "14:30"

#### `Get Time of Day Period`
**Outputs:** Enum - Night, Dawn, Morning, Noon, Afternoon, Dusk, Evening

#### `Get Current Season`
**Outputs:** Enum - Spring, Summer, Autumn, Winter

#### `Is Daytime?` / `Is Nighttime?`
**Outputs:** Bool

#### `Get Day Progress (0-1)`
**What:** Percentage through day
**Outputs:** Float (0-1)
- 0.0 = Midnight
- 0.25 = 6:00 AM
- 0.5 = Noon
- 0.75 = 6:00 PM
- 1.0 = Next midnight
**Example:**
```
Get Day Progress
→ Returns: 0.6 (60% through day)
→ Perfect for sun position in sky!
```

---

## Summary

**Stat Component:** Numeric values (health, stamina, etc.)
**Body Component:** Locational damage (fractures, bleeding, burns)
**Status Effect Component:** Buffs/debuffs (poison, speed boost, etc.)
**Environment Component:** Weather & hazards (temperature, radiation)
**Progression Component:** XP, levels, skills
**Time Component:** Day/night cycle, seasons

Each component works independently but can interact with others for deep gameplay!
