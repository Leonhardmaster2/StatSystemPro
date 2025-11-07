# StatSystemPro Editor Tools Guide

## Overview

StatSystemPro includes a comprehensive editor module that provides custom details panels and editor utilities for managing all plugin systems directly in the Unreal Editor.

**No Data Tables Required!** Configure everything visually through enhanced property panels with one-click actions.

---

## Table of Contents

1. [What's Included](#whats-included)
2. [Custom Details Panels](#custom-details-panels)
   - [Stat Component Details](#stat-component-details)
   - [Body Component Details](#body-component-details)
   - [Weather Component Details](#weather-component-details)
3. [Quick Actions](#quick-actions)
4. [Creating the Control Panel](#creating-the-control-panel)
5. [Advanced Customization](#advanced-customization)

---

## What's Included

The StatSystemProEditor module provides:

### **Custom Details Panels** ✨
Enhanced property panels for all components with:
- Quick action buttons for common operations
- Visual feedback and testing tools
- One-click presets
- Contextual help text

### **Supported Components:**
- ✅ **StatComponent** - Stat management with quick restore/deplete actions
- ✅ **BodyComponent** - Body part control with healing/damage/fracture tools
- ✅ **WeatherComponent** - Weather presets and clothing management
- ✅ **StatusEffectComponent** - Status effect management (basic panel)
- ✅ **ProgressionComponent** - Progression management (basic panel)

### **Editor Menu Integration**
- Tools menu entry for opening control panels
- Quick access from the main editor toolbar

---

## Custom Details Panels

When you select a component in the editor, you'll see enhanced property panels with custom categories and quick actions.

### Stat Component Details

**Location:** Select any actor with a `StatComponent` → Details Panel

#### **Categories:**

##### 1. **Quick Actions**
One-click buttons for common stat operations:

| Button | Description | Effect |
|--------|-------------|--------|
| **Restore All Stats** | Set all stats to maximum | All stats → 100% |
| **Deplete All Stats** | Set all stats to zero | All stats → 0% |
| **Randomize Stats** | Set random values for testing | Random 0-100% |
| **Reset Regeneration** | Reset all regen rates to defaults | Regen → 0 |

##### 2. **Stat Testing**
Testing-specific actions:

| Button | Description | Use Case |
|--------|-------------|----------|
| **Test Critical Condition** | Set Health_Core to 10% | Test critical health UI/effects |

##### 3. **Stat Configuration**
Standard property editing with enhanced tooltips and organization.

##### 4. **Stat Visualization**
Information about stat visualization options.

#### **Usage Example:**

```
1. Add StatComponent to your character Blueprint
2. Select the character in the viewport or outliner
3. In Details panel, find "Quick Actions" category
4. Click "Restore All Stats" to set all stats to 100%
5. Click "Test Critical Condition" to test low health
```

---

### Body Component Details

**Location:** Select any actor with a `BodyComponent` → Details Panel

#### **Categories:**

##### 1. **Quick Actions**

| Button | Description | Effect |
|--------|-------------|--------|
| **Heal All Body Parts** | Restore all parts to 100% | Full body recovery |
| **Damage All Body Parts** | Apply 50% damage to all parts | Quick damage test |
| **Heal All Fractures** | Remove all fractures | Fix all broken bones |
| **Stop All Bleeding** | Stop bleeding on all parts | Emergency heal |

##### 2. **Body Part Testing**

| Button | Description | Use Case |
|--------|-------------|----------|
| **Fracture Random Body Part** | Randomly break one limb | Test fracture mechanics |
| **Cause Bleeding** | Cause bleeding on 2-3 parts | Test bleeding system |

#### **Usage Example:**

```
1. Add BodyComponent to character
2. Select character in editor
3. Click "Damage All Body Parts" to test injured state
4. Click "Fracture Random Body Part" to test limping/movement penalties
5. Click "Cause Bleeding" to test blood loss mechanics
6. Click "Heal All Body Parts" when done testing
```

---

### Weather Component Details

**Location:** Select any actor with a `WeatherComponent` → Details Panel

#### **Categories:**

##### 1. **Weather Presets**

Apply complete weather configurations with one click:

| Button | Weather Type | Temperature | Conditions |
|--------|-------------|-------------|------------|
| **Apply Blizzard** | Heavy Snow | -15°C | High wind, low visibility |
| **Apply Heatwave** | Clear | 42°C | Extreme heat, no wind |
| **Apply Thunderstorm** | Thunderstorm | 18°C | Heavy rain, strong wind |
| **Apply Clear Weather** | Clear | 22°C | Normal conditions |

##### 2. **Clothing Management**

Quick-equip clothing sets:

| Button | Description | Contents |
|--------|-------------|----------|
| **Equip Winter Gear** | Full arctic clothing set | Fur Hat, Parka, Insulated Pants, Winter Boots, Gloves |
| **Equip Desert Gear** | Breathable hot-weather clothing | Desert Hat, Desert Robe, Light Pants, Sandals |
| **Remove All Clothing** | Strip all clothing | Empty all slots |

##### 3. **Quick Actions**

| Button | Description | Effect |
|--------|-------------|--------|
| **Soak All Clothing** | Set wetness to 100% | Test wet clothing penalty |
| **Dry All Clothing** | Set wetness to 0% | Instant dry |

#### **Usage Example - Testing Winter Survival:**

```
1. Add WeatherComponent to character
2. Select character in editor
3. Click "Apply Blizzard" preset
   → Temperature: -15°C
   → Heavy snow, high wind
4. Click "Equip Winter Gear"
   → Full arctic clothing equipped
   → Character can survive blizzard
5. Click "Remove All Clothing"
   → Character starts freezing rapidly
   → Test hypothermia stages
6. Click "Equip Winter Gear" again to save character
```

#### **Usage Example - Testing Hot Weather:**

```
1. Click "Apply Heatwave" preset
   → Temperature: 42°C
2. Click "Equip Winter Gear" (wrong clothing!)
   → Character overheats (winter jacket traps heat)
   → Temperature feels like 50°C+
   → Heatstroke occurs
3. Click "Equip Desert Gear" (correct clothing)
   → Breathable clothing
   → Temperature more manageable
```

---

## Quick Actions Reference

### Stat Component Quick Actions

**Restore All Stats**
```cpp
// Equivalent C++ code:
StatComponent->RestoreAllStatsToMax();
```

**Deplete All Stats**
```cpp
for (EStatType Stat : AllStats)
{
    StatComponent->SetStatValue(Stat, 0.0f);
}
```

**Test Critical Condition**
```cpp
float MaxHealth = StatComponent->GetStatMaxValue(EStatType::Health_Core);
StatComponent->SetStatValue(EStatType::Health_Core, MaxHealth * 0.10f);
```

### Body Component Quick Actions

**Heal All Body Parts**
```cpp
BodyComponent->HealAllBodyParts(100.0f);
```

**Fracture Random Body Part**
```cpp
EBodyPart RandomPart = (EBodyPart)FMath::RandRange(0, (int32)EBodyPart::MAX - 1);
BodyComponent->FractureLimb(RandomPart);
```

### Weather Component Quick Actions

**Apply Blizzard Preset**
```cpp
FWeatherPreset Blizzard = WeatherComponent->GetBlizzardPreset();
WeatherComponent->ApplyWeatherPreset(Blizzard);
```

**Equip Winter Gear**
```cpp
// Creates and equips full winter clothing set
// See WeatherComponentDetails.cpp for exact values
```

---

## Creating the Control Panel

The editor module includes menu integration for a centralized control panel. To create it:

### Option 1: Editor Utility Widget (Recommended)

**Step 1: Create Editor Utility Widget**
1. In Content Browser, right-click → Editor Utilities → Editor Utility Widget
2. Name it: `EUW_StatSystemProControlPanel`
3. Save to: `/StatSystemPro/Editor/` folder

**Step 2: Design the Control Panel**

Add these sections:

#### **Stat Management Tab**
- Dropdown to select stat type
- Slider to adjust value (0-100)
- Button: "Apply to All Selected Actors"
- Button: "Restore All Stats"
- Button: "Deplete All Stats"

#### **Body Management Tab**
- Dropdown to select body part
- Input field for damage amount
- Button: "Apply Damage"
- Button: "Fracture Limb"
- Button: "Start Bleeding"
- Button: "Heal Body Part"

#### **Weather Management Tab**
- Dropdown for weather presets
- Button: "Apply Preset"
- Slider for temperature (-50 to +50°C)
- Slider for wind speed (0-50 m/s)
- Slider for shelter level (0-100%)

#### **Clothing Management Tab**
- Dropdown for clothing slot
- Display: Current clothing in slot
- Button: "Equip Winter Gear"
- Button: "Equip Desert Gear"
- Button: "Remove Clothing"
- Slider: Clothing wetness
- Button: "Apply Wetness"

**Step 3: Wire Up Blueprint Logic**

Example Blueprint nodes for "Restore All Stats" button:

```
Event: OnClicked (RestoreAllStatsButton)
→ Get All Actors of Class (Character)
→ ForEachLoop
   → Get Component by Class (StatComponent)
   → Call: Restore All Stats To Max
→ Print String: "Restored all stats for X characters"
```

**Step 4: Register in Menu**

The editor module automatically adds a menu entry:
```
Tools → Stat System Pro → Stat System Pro Control Panel
```

This will open your Editor Utility Widget.

### Option 2: Slate Window (Advanced - C++)

For a fully native C++ control panel, extend the editor module with a Slate window.

Create: `SStatSystemProControlPanel.h` and `.cpp`

```cpp
class SStatSystemProControlPanel : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SStatSystemProControlPanel) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    // Tab switching
    TSharedPtr<SDockTab> OwningTab;

    // Widgets
    TSharedPtr<SComboBox<TSharedPtr<FString>>> StatTypeComboBox;
    TSharedPtr<SSlider> StatValueSlider;

    // Callbacks
    FReply OnRestoreAllStatsClicked();
    FReply OnApplyBlizzardClicked();
};
```

Register the tab spawner in `FStatSystemProEditorModule::StartupModule()`.

---

## Advanced Customization

### Creating Custom Details Panels

To add your own custom details panel for a new component:

**Step 1: Create Header File**

```cpp
// MyComponentDetails.h
#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FMyComponentDetails : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
    TWeakObjectPtr<UMyComponent> CachedComponent;

    void BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder);
    FReply OnMyActionClicked();

    UMyComponent* GetComponent() const;
};
```

**Step 2: Create Implementation**

```cpp
// MyComponentDetails.cpp
#include "Details/MyComponentDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "MyComponent.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "MyComponentDetails"

TSharedRef<IDetailCustomization> FMyComponentDetails::MakeInstance()
{
    return MakeShareable(new FMyComponentDetails);
}

void FMyComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> Objects;
    DetailBuilder.GetObjectsBeingCustomized(Objects);

    if (Objects.Num() > 0)
    {
        CachedComponent = Cast<UMyComponent>(Objects[0].Get());
    }

    if (!CachedComponent.IsValid())
    {
        return;
    }

    BuildQuickActionsCategory(DetailBuilder);
}

void FMyComponentDetails::BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder)
{
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Quick Actions");

    Category.AddCustomRow(LOCTEXT("ActionRow", "Actions"))
    .WholeRowContent()
    [
        SNew(SButton)
        .Text(LOCTEXT("MyAction", "My Action"))
        .OnClicked(this, &FMyComponentDetails::OnMyActionClicked)
        .HAlign(HAlign_Center)
    ];
}

FReply FMyComponentDetails::OnMyActionClicked()
{
    if (UMyComponent* Comp = GetComponent())
    {
        // Perform action
        UE_LOG(LogTemp, Log, TEXT("Action performed"));
    }
    return FReply::Handled();
}

UMyComponent* FMyComponentDetails::GetComponent() const
{
    return CachedComponent.Get();
}

#undef LOCTEXT_NAMESPACE
```

**Step 3: Register in Editor Module**

In `StatSystemProEditor.cpp`:

```cpp
#include "Details/MyComponentDetails.h"
#include "MyComponent.h"

void FStatSystemProEditorModule::RegisterDetailsCustomizations()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    // ... existing registrations ...

    PropertyModule.RegisterCustomClassLayout(
        UMyComponent::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FMyComponentDetails::MakeInstance)
    );

    PropertyModule.NotifyCustomizationModuleChanged();
}

void FStatSystemProEditorModule::UnregisterDetailsCustomizations()
{
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        // ... existing unregistrations ...

        PropertyModule.UnregisterCustomClassLayout(UMyComponent::StaticClass()->GetFName());

        PropertyModule.NotifyCustomizationModuleChanged();
    }
}
```

### Adding More Quick Actions

To add a new quick action button:

**1. Add method declaration in header:**
```cpp
FReply OnMyNewActionClicked();
```

**2. Add button in BuildQuickActionsCategory:**
```cpp
+ SUniformGridPanel::Slot(0, 2) // Row 3, Column 1
[
    SNew(SButton)
    .Text(LOCTEXT("MyNewAction", "My New Action"))
    .ToolTipText(LOCTEXT("MyNewActionTooltip", "Description of what this does"))
    .OnClicked(this, &FMyComponentDetails::OnMyNewActionClicked)
    .HAlign(HAlign_Center)
]
```

**3. Implement the handler:**
```cpp
FReply FMyComponentDetails::OnMyNewActionClicked()
{
    if (UMyComponent* Comp = GetComponent())
    {
        // Perform your action
        Comp->DoSomething();
        UE_LOG(LogTemp, Log, TEXT("Performed new action"));
    }
    return FReply::Handled();
}
```

---

## Troubleshooting

### Editor Module Not Loading

**Problem:** Editor features not appearing

**Solution:**
1. Check that `StatSystemProEditor` module is listed in `.uplugin`
2. Verify module type is `"Type": "Editor"`
3. Regenerate project files
4. Rebuild the project

### Custom Details Not Showing

**Problem:** Enhanced property panels not appearing

**Solution:**
1. Check that component is properly selected
2. Verify module is loading: Check Output Log for "StatSystemProEditor: Module Started Successfully"
3. Check for errors in Output Log
4. Restart the editor

### Control Panel Not Found

**Problem:** Menu entry exists but widget doesn't open

**Solution:**
1. Create the Editor Utility Widget first (see Creating the Control Panel)
2. Place it in correct path: `/StatSystemPro/Editor/EUW_StatSystemProControlPanel`
3. Widget must be named exactly: `EUW_StatSystemProControlPanel`
4. Check Output Log for path errors

### Buttons Not Working

**Problem:** Clicking buttons doesn't do anything

**Solution:**
1. Check that component is valid (not null)
2. Check Output Log for errors
3. Verify function exists on component
4. Ensure actor is not in a locked state

---

## Best Practices

### Testing Workflow

**Recommended workflow for testing survival mechanics:**

1. **Setup Phase:**
   - Add all components to test character
   - Use "Restore All Stats" to start fresh
   - Set initial weather with presets

2. **Test Scenario:**
   - Apply specific weather preset
   - Equip appropriate/inappropriate clothing
   - Monitor stat changes in real-time

3. **Quick Recovery:**
   - Use quick actions to instantly heal/restore
   - Faster than manually adjusting values
   - No need to restart PIE session

4. **Iteration:**
   - Quickly cycle through different configurations
   - Test edge cases with one-click actions
   - Compare different clothing/weather combinations

### Editor Organization

**Keep your editor workspace clean:**

- Use the Control Panel for batch operations
- Use Details Panels for individual component tweaking
- Save commonly used configurations as presets
- Document your test scenarios

### Performance Considerations

**Editor tools are editor-only:**
- No runtime overhead in packaged game
- Editor module is not included in builds
- All details customizations are editor-only

---

## Summary

The StatSystemPro Editor Tools provide:

✅ **No Data Table Setup Required** - Configure everything visually
✅ **One-Click Testing** - Quick action buttons for common scenarios
✅ **Weather Presets** - Instant environment configuration
✅ **Clothing Quick-Equip** - Test survival mechanics immediately
✅ **Enhanced Property Panels** - Better organization and tooltips
✅ **Fast Iteration** - Test scenarios in seconds, not minutes

**Key Features:**
- 20+ quick action buttons across all components
- 4 weather presets (Blizzard, Heatwave, Thunderstorm, Clear)
- 2 clothing loadouts (Winter Gear, Desert Gear)
- Custom details panels for 5 components
- Editor menu integration

**Use the editor tools to:**
- Rapidly prototype survival mechanics
- Test edge cases without manual setup
- Iterate on game balance
- Debug issues in the editor
- Showcase features to your team

No more tedious manual configuration - just click and test!
