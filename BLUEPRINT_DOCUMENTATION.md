# StatSystemPro - Complete Blueprint Documentation

## Table of Contents
1. [Quick Start (30 Seconds)](#quick-start-30-seconds)
2. [Simple Mode vs Advanced Mode](#simple-mode-vs-advanced-mode)
3. [Multiplayer Setup](#multiplayer-setup)
4. [All Blueprint Functions Explained](#all-blueprint-functions-explained)
5. [Common Use Cases](#common-use-cases)
6. [Events Reference](#events-reference)
7. [Performance Tips](#performance-tips)
8. [Troubleshooting](#troubleshooting)

---

## Quick Start (30 Seconds)

### Absolute Beginner Setup:

1. **Add Component to Your Character**
   - Open your Character Blueprint
   - Click "Add Component"
   - Search for "Stat Component"
   - Done! All stats are now 100 and working!

2. **Test It**
   ```
   Event BeginPlay
   ├─> Get Stat Value (Health_Core)
   └─> Print String (shows "100")
   ```

### First Damage Example:
```
On Any Damage Received
├─> Change Stat (Health_Core, Amount: -25)
└─> Get Stat Percentage (Health_Core)
    └─> Set Progress Bar Percent
```

**That's it!** You now have a working stat system!

---

## Simple Mode vs Advanced Mode

### SIMPLE MODE (Default - Recommended for Beginners)

**What it does:**
- Automatically creates ALL stats and sets them to 100
- No configuration needed
- Just add the component and go!

**Perfect for:**
- Prototyping
- Game jams
- Learning Unreal
- Quick projects

**How to use:**
```
1. Add Stat Component to Actor
2. Leave "Use Simple Mode" = TRUE (default)
3. Start using Get/Set functions immediately
```

**All stats available in Simple Mode:**
- Health_Core = 100
- Stamina = 100
- Energy = 100
- Hunger = 100
- Thirst = 100
- Fatigue = 100
- And 10 more... all = 100!

---

### ADVANCED MODE (For Production Games)

**What it unlocks:**
- Custom stat values (health = 150, stamina = 80, etc.)
- Custom regeneration rates
- Regeneration curves
- Per-stat configuration

**How to setup:**
```
1. Set "Use Simple Mode" = FALSE
2. Create a Data Table (Row Type: StatConfigRow)
3. Add rows for each stat you want to customize
4. Assign table to "Stat Config Table"
```

**Example Data Table:**
| Row Name | Stat Type | Default Max | Regen Rate |
|----------|-----------|-------------|------------|
| Health | Health_Core | 150 | 0.5 |
| Stamina | Stamina | 80 | 5.0 |
| Hunger | Hunger | 100 | -1.0 |

---

## Multiplayer Setup

### The Good News:
**Everything replicates automatically!** No extra setup needed.

### How it Works:

**Server:**
- Has full authority over all stats
- All changes happen on server first
- Server replicates to all clients

**Clients:**
- Receive stat updates automatically
- Can read all stat values
- Cannot change stats directly (must call server)

### Blueprint Example:
```blueprint
// THIS WORKS (Runs on Server):
Event BeginPlay (Server Only)
├─> Change Stat (Health_Core, -25)
└─> ✓ Replicates to all clients!

// THIS ALSO WORKS (From Client):
Event Any Damage
├─> Run On Server (Custom Event)
    └─> Change Stat (Health_Core, -25)
        └─> ✓ Replicates to all clients!
```

### Multiplayer Checklist:
- ✅ Stat changes on server? Auto-replicated!
- ✅ Reading stats on clients? Works perfectly!
- ✅ Events fire on all machines? Yes!
- ✅ UI updates? Bind to OnStatChanged event!

---

## All Blueprint Functions Explained

### 📊 BASIC GETTERS (Most Used)

#### **Get Stat Value**
```
Returns: Float (e.g., 75.0)
Use: Get exact amount of a stat
Example: How much health do I have? → 75
```

#### **Get Stat Maximum**
```
Returns: Float (e.g., 100.0)
Use: Get max value for percentage calculations
Example: Max health? → 100
```

#### **Get Stat Percentage (0-1)**
```
Returns: Float between 0.0 and 1.0
Use: Perfect for UI progress bars
Example: 75/100 health → 0.75
TIP: Plug directly into Progress Bar "Percent" pin!
```

#### **Get Stat Percentage (0-100)**
```
Returns: Float between 0 and 100
Use: When you want whole number percentages
Example: 75/100 health → 75.0
TIP: Better for text displays like "Health: 75%"
```

---

### ✏️ MODIFICATION FUNCTIONS

#### **Change Stat (Add/Subtract)**
```
MOST IMPORTANT FUNCTION!

Parameters:
- Stat Type: Which stat? (Health, Stamina, etc.)
- Amount: How much? (Positive = add, Negative = subtract)
- Source: Name for debugging ("Combat", "Food", etc.)
- Reason Tag: Optional gameplay tag

Examples:
✓ Damage: Amount = -25
✓ Heal: Amount = +50
✓ Eat Food: Amount = +30 (Hunger)
✓ Drink Water: Amount = +40 (Thirst)
✓ Rest: Amount = +20 (Energy)

Multiplayer: Automatically replicated!
```

#### **Set Stat to Exact Value**
```
WARNING: This OVERWRITES current value!

Use: When you need precise control
Example: Set health to exactly 50

When to use:
✓ Respawn (set health to 100)
✓ Cheat codes
✓ Loading saved game
✗ DON'T use for damage/healing (use Change Stat instead)
```

#### **Set Stat Maximum**
```
Use: Buffs/debuffs that change max values

Examples:
- +50 Max Health buff
- -20 Max Stamina debuff
- Double max stamina power-up

Note: Current value auto-clamps to new max
```

#### **Set Regeneration Rate**
```
Use: Make stats regen or decay

Parameters:
- Rate: Units per second

Examples:
✓ Health regen: +5 = gain 5 health/sec
✓ Hunger decay: -1 = lose 1 hunger/sec
✓ Stamina regen: +10 = gain 10 stamina/sec
✓ Stop regen: 0 = no change

Multiplayer: Replicated!
```

---

### 🔍 CHECKER FUNCTIONS

#### **Is Stat at Max?**
```
Returns: True/False
Use: Check if stat is full

Example:
If (Is Stat at Max? Health)
└─> Display "Fully Healed!"
```

#### **Is Stat at Zero?**
```
Returns: True/False
Use: Check for death, exhaustion, etc.

Examples:
✓ Is Health zero? → Player Dead
✓ Is Stamina zero? → Can't sprint
✓ Is Hunger zero? → Start starving
```

#### **Is Stat Critical?**
```
Returns: True/False
Use: Warning states
Default: Below 15%

Example:
If (Is Stat Critical? Health)
└─> Show red warning UI
```

#### **Is Any Stat Critical?**
```
Returns: True/False
Use: Overall danger check

Example:
If (Is Any Stat Critical?)
└─> Play warning sound
    └─> Screen edge red flash
```

---

### 📈 CALCULATION FUNCTIONS

#### **Get Stat Missing Amount**
```
Returns: Float (Max - Current)
Use: Calculate healing needed

Example:
Max Health: 100
Current Health: 75
Missing: 25 ← Perfect for healing items!

Use Case:
Health Pack heals 50, but player only missing 25
→ Don't waste 25 healing!
→ Heal exactly 25
```

#### **Get Average Stat Health**
```
Returns: Float 0.0 to 1.0
Use: Overall character condition

Example:
Health: 50% (0.5)
Stamina: 80% (0.8)
Hunger: 60% (0.6)
Average: 63% (0.63)

Use: Visual indicator of overall health
```

---

### 📋 ADVANCED FUNCTIONS

#### **Get Regen Rate**
```
Returns: Float (units per second)
Use: Check how fast stat is changing

Example:
If (Get Regen Rate Health) > 0
└─> Show green "+" icon (regenerating)
Else
└─> No regen
```

#### **Get Base Max Value**
```
Returns: Float (original max before buffs)
Use: See original max value

Example:
Base Max Health: 100
Current Max: 150 (buffed)
Base Max: 100 ← This function returns this
```

#### **Get All Stat Types**
```
Returns: Array of all stat types
Use: Loop through all stats for UI

Example:
ForEach Stat Type
├─> Get Stat Value
└─> Create UI widget for each stat
```

---

## Common Use Cases

### 1. HEALTH BAR UI

```blueprint
Event Tick
├─> Get Stat Percentage (Health_Core)
└─> Set Progress Bar Percent
```

**Better Version (Event-Driven):**
```blueprint
On Stat Changed (Event)
├─> Branch: Is Stat = Health_Core?
    └─> True: Set Progress Bar Percent
```

---

### 2. DAMAGE SYSTEM

```blueprint
On Take Damage
├─> Change Stat (Health_Core, Amount: -Damage)
├─> Play Hit Sound
└─> If (Is Stat at Zero? Health)
    └─> Kill Player
```

---

### 3. HUNGER DECAY

**Method 1: Auto Decay**
```blueprint
Event BeginPlay
└─> Set Regeneration Rate (Hunger, -0.5)
    // Loses 0.5 hunger per second
```

**Method 2: Manual Decay**
```blueprint
Event Tick (every 1 second)
└─> Change Stat (Hunger, -0.5)
```

---

### 4. EATING FOOD

```blueprint
On Eat Food Item
├─> Change Stat (Hunger, +30)
├─> Change Stat (Thirst, +10)
├─> Change Stat (Energy, +5)
└─> Play Eat Sound
```

---

### 5. STAMINA SPRINT SYSTEM

```blueprint
// When Sprint Starts
On Sprint Pressed
├─> If (Get Stat Value Stamina) > 10
    ├─> True:
        ├─> Set Regeneration Rate (Stamina, -20)
        └─> Increase Movement Speed
    └─> False:
        └─> Show "Not enough stamina!" message

// When Sprint Ends
On Sprint Released
└─> Set Regeneration Rate (Stamina, +10)
```

---

### 6. CRITICAL HEALTH WARNING

```blueprint
On Stat Changed
├─> Branch: Stat Type = Health_Core?
    └─> True:
        └─> If (Is Stat Critical? Health)
            ├─> True: Show Red Screen Effect
            └─> False: Hide Red Screen Effect
```

---

### 7. SAVE/LOAD SYSTEM

**Save:**
```blueprint
On Save Game
├─> Get Stat Value (Health_Core) → Save to struct
├─> Get Stat Value (Stamina) → Save to struct
└─> ... repeat for all stats
```

**Load:**
```blueprint
On Load Game
├─> Set Stat Value (Health_Core, SavedValue)
├─> Set Stat Value (Stamina, SavedValue)
└─> ... repeat for all stats
```

---

### 8. RESPAWN SYSTEM

```blueprint
On Player Death
└─> Delay 3 seconds
    └─> Respawn Player
        ├─> Set Stat Value (Health_Core, 100)
        ├─> Set Stat Value (Stamina, 100)
        ├─> Set Stat Value (Hunger, 100)
        └─> Set Stat Value (Thirst, 100)
```

---

## Events Reference

### **On Any Stat Changed**
```
Fires: When ANY stat value changes
Provides: StatType, OldValue, NewValue

Use: Update UI, play sounds, trigger effects

Example:
On Stat Changed
├─> Switch on Stat Type
    ├─> Health → Update Health Bar
    ├─> Stamina → Update Stamina Bar
    └─> Hunger → Update Hunger Bar
```

---

### **On Stat Reached Zero**
```
Fires: When a stat hits 0
Provides: StatType

Use: Death, exhaustion, starvation

Example:
On Stat Reached Zero
├─> Switch on Stat Type
    ├─> Health → Kill Player
    ├─> Stamina → Stop Sprinting
    └─> Hunger → Start Starvation Damage
```

---

### **On Stat Reached Max**
```
Fires: When a stat reaches maximum
Provides: StatType

Use: Healing complete, fully rested

Example:
On Stat Reached Max
└─> If Stat = Health
    └─> Achievement Unlocked: "Fully Healed"
```

---

### **On Stat Critical**
```
Fires: When stat drops below critical threshold (default 15%)
Provides: StatType, CurrentValue

Use: Warnings, danger states

Example:
On Stat Critical
├─> Play Warning Sound
├─> Screen Flash Red
└─> Show "Critical Health!" message
```

---

### **On Stat Max Changed**
```
Fires: When a stat's maximum value changes
Provides: StatType, NewMaxValue

Use: Update UI when max changes

Example:
On Stat Max Changed
└─> Update Health Bar Max Value
```

---

## Performance Tips

### 🚀 Optimization Guide:

#### **1. Use Events, Not Tick**
```
❌ BAD:
Event Tick
└─> Get Stat Value → Update UI

✅ GOOD:
On Stat Changed (Event)
└─> Update UI only when needed
```

#### **2. Disable Unused Features**
```
✓ Don't need auto-regen? Turn off "Auto Regeneration"
✓ Don't need a layer? Disable it!
✓ Save performance!
```

#### **3. Simple Mode is Faster**
```
Simple Mode: Instant initialization
Advanced Mode: Loads from data table (slower)

For most games: Simple Mode is perfect!
```

#### **4. Multiplayer**
```
✓ Stats only replicate when changed (not every frame)
✓ Bandwidth efficient
✓ No special optimization needed
```

---

## Troubleshooting

### Problem: "Stats are all 0!"

**Solution:**
```
1. Check if "Enable Stat System" = TRUE
2. Check if BeginPlay was called
3. Simple Mode turned on?
4. In multiplayer: Are you on the server?
```

---

### Problem: "Stats not replicating in multiplayer!"

**Solution:**
```
1. Are you changing stats on the SERVER?
2. Use "Run on Server" custom event
3. Check if Actor replicates (Replicates = TRUE)
```

---

### Problem: "UI not updating!"

**Solution:**
```
❌ Don't use Event Tick!
✅ Use "On Stat Changed" event!

Example:
On Stat Changed
└─> Update UI (only when stat actually changes)
```

---

### Problem: "Stats changing too fast/slow!"

**Solution:**
```
For Auto-Regen:
└─> Adjust "Regeneration Rate"
    - Too fast? Lower the number
    - Too slow? Increase the number

For Manual Decay:
└─> Change how often you call "Change Stat"
```

---

### Problem: "Can't find Stat Component!"

**Solution:**
```
1. Plugin enabled? (Edit → Plugins → StatSystemPro)
2. Project compiled? (Compile button)
3. Editor restarted?
```

---

## Quick Reference Card

### Most Used Functions:
```
Get Stat Value → Read current value
Get Stat Percentage → For UI bars (0-1)
Change Stat → Add/subtract from stat
Set Stat Value → Set to exact value
Is Stat at Zero? → Check for depletion
On Stat Changed → Event when stat changes
```

### Common Stat Types:
```
Health_Core → Main health
Stamina → Sprint/dodge energy
Hunger → Food meter
Thirst → Water meter
Energy → Tiredness
Fatigue → Sleep need
```

### Quick Setup:
```
1. Add Stat Component
2. Leave Simple Mode = ON
3. Use Change Stat for damage/healing
4. Bind UI to On Stat Changed event
5. Done!
```

---

## Learning Path

### Level 1: Beginner (Day 1)
- ✅ Add Stat Component
- ✅ Use Get Stat Value
- ✅ Use Change Stat
- ✅ Make a health bar

### Level 2: Intermediate (Week 1)
- ✅ Use events (On Stat Changed)
- ✅ Multiple stats (health, stamina, hunger)
- ✅ Auto-regeneration
- ✅ Critical state detection

### Level 3: Advanced (Month 1)
- ✅ Switch to Advanced Mode
- ✅ Data table configuration
- ✅ Multiplayer replication
- ✅ Complex stat interactions

---

## Need More Help?

**Check out:**
- README.md → Full system overview
- Example blueprints in plugin
- Community forums
- Video tutorials (coming soon!)

---

**Remember: Start simple, grow complex!**

The best way to learn is to:
1. Add the component
2. Try one function
3. See it work
4. Add more features gradually

Happy game developing! 🎮
