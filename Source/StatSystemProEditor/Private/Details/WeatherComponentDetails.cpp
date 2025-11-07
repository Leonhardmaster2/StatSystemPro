// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/WeatherComponentDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "WeatherSystem/WeatherComponent.h"
#include "WeatherSystem/WeatherTypes.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#define LOCTEXT_NAMESPACE "WeatherComponentDetails"

TSharedRef<IDetailCustomization> FWeatherComponentDetails::MakeInstance()
{
	return MakeShareable(new FWeatherComponentDetails);
}

void FWeatherComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

	if (ObjectsBeingCustomized.Num() > 0)
	{
		CachedWeatherComponent = Cast<UWeatherComponent>(ObjectsBeingCustomized[0].Get());
	}

	if (!CachedWeatherComponent.IsValid())
	{
		return;
	}

	BuildWeatherPresetsCategory(DetailBuilder);
	BuildClothingManagementCategory(DetailBuilder);
	BuildQuickActionsCategory(DetailBuilder);
}

void FWeatherComponentDetails::BuildWeatherPresetsCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Weather Presets", FText::GetEmpty(), ECategoryPriority::Important);

	Category.AddCustomRow(LOCTEXT("PresetsHelpRow", "Presets Help"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("PresetsHelpText", "Quickly apply weather presets to test different environmental conditions"))
		.AutoWrapText(true)
	];

	Category.AddCustomRow(LOCTEXT("WeatherPresetsRow", "Weather Presets"))
	.WholeRowContent()
	[
		SNew(SUniformGridPanel)
		.SlotPadding(2.0f)

		// Row 1
		+ SUniformGridPanel::Slot(0, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("ApplyBlizzard", "Apply Blizzard"))
			.ToolTipText(LOCTEXT("ApplyBlizzardTooltip", "Apply extreme cold blizzard preset (-15°C, heavy snow, high wind)"))
			.OnClicked(this, &FWeatherComponentDetails::OnApplyBlizzardPresetClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("ApplyHeatwave", "Apply Heatwave"))
			.ToolTipText(LOCTEXT("ApplyHeatwaveTooltip", "Apply extreme heat heatwave preset (42°C, clear sky)"))
			.OnClicked(this, &FWeatherComponentDetails::OnApplyHeatwavePresetClicked)
			.HAlign(HAlign_Center)
		]

		// Row 2
		+ SUniformGridPanel::Slot(0, 1)
		[
			SNew(SButton)
			.Text(LOCTEXT("ApplyThunderstorm", "Apply Thunderstorm"))
			.ToolTipText(LOCTEXT("ApplyThunderstormTooltip", "Apply thunderstorm preset (18°C, heavy rain, strong wind)"))
			.OnClicked(this, &FWeatherComponentDetails::OnApplyThunderstormPresetClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 1)
		[
			SNew(SButton)
			.Text(LOCTEXT("ApplyClear", "Apply Clear Weather"))
			.ToolTipText(LOCTEXT("ApplyClearTooltip", "Apply clear weather preset (22°C, sunny, light wind)"))
			.OnClicked(this, &FWeatherComponentDetails::OnApplyClearWeatherPresetClicked)
			.HAlign(HAlign_Center)
		]
	];
}

void FWeatherComponentDetails::BuildClothingManagementCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Clothing Management", FText::GetEmpty(), ECategoryPriority::Important);

	Category.AddCustomRow(LOCTEXT("ClothingRow", "Clothing Actions"))
	.WholeRowContent()
	[
		SNew(SUniformGridPanel)
		.SlotPadding(2.0f)

		// Row 1
		+ SUniformGridPanel::Slot(0, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("EquipWinterGear", "Equip Winter Gear"))
			.ToolTipText(LOCTEXT("EquipWinterGearTooltip", "Equip full winter clothing set for cold weather testing"))
			.OnClicked(this, &FWeatherComponentDetails::OnEquipWinterGearClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("EquipDesertGear", "Equip Desert Gear"))
			.ToolTipText(LOCTEXT("EquipDesertGearTooltip", "Equip breathable desert clothing for hot weather testing"))
			.OnClicked(this, &FWeatherComponentDetails::OnEquipDesertGearClicked)
			.HAlign(HAlign_Center)
		]

		// Row 2
		+ SUniformGridPanel::Slot(0, 1)
		[
			SNew(SButton)
			.Text(LOCTEXT("RemoveAllClothing", "Remove All Clothing"))
			.ToolTipText(LOCTEXT("RemoveAllClothingTooltip", "Remove all equipped clothing"))
			.OnClicked(this, &FWeatherComponentDetails::OnRemoveAllClothingClicked)
			.HAlign(HAlign_Center)
		]
	];
}

void FWeatherComponentDetails::BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Quick Actions", FText::GetEmpty(), ECategoryPriority::Default);

	Category.AddCustomRow(LOCTEXT("QuickActionsRow", "Quick Actions"))
	.WholeRowContent()
	[
		SNew(SUniformGridPanel)
		.SlotPadding(2.0f)

		+ SUniformGridPanel::Slot(0, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("SoakClothing", "Soak All Clothing"))
			.ToolTipText(LOCTEXT("SoakClothingTooltip", "Set all clothing wetness to 100% for testing"))
			.OnClicked(this, &FWeatherComponentDetails::OnSoakAllClothingClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("DryClothing", "Dry All Clothing"))
			.ToolTipText(LOCTEXT("DryClothingTooltip", "Set all clothing wetness to 0%"))
			.OnClicked(this, &FWeatherComponentDetails::OnDryAllClothingClicked)
			.HAlign(HAlign_Center)
		]
	];
}

FReply FWeatherComponentDetails::OnApplyBlizzardPresetClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		FWeatherPreset Preset = WeatherComp->GetBlizzardPreset();
		WeatherComp->ApplyWeatherPreset(Preset);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Applied Blizzard preset"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnApplyHeatwavePresetClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		FWeatherPreset Preset = WeatherComp->GetHeatwavePreset();
		WeatherComp->ApplyWeatherPreset(Preset);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Applied Heatwave preset"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnApplyThunderstormPresetClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		FWeatherPreset Preset = WeatherComp->GetThunderstormPreset();
		WeatherComp->ApplyWeatherPreset(Preset);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Applied Thunderstorm preset"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnApplyClearWeatherPresetClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		FWeatherPreset Preset = WeatherComp->GetClearWeatherPreset();
		WeatherComp->ApplyWeatherPreset(Preset);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Applied Clear Weather preset"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnEquipWinterGearClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		// Create winter gear
		FClothingItem FurHat;
		FurHat.DisplayName = FText::FromString(TEXT("Fur Hat"));
		FurHat.Slot = EClothingSlot::Head;
		FurHat.ColdInsulation = 70.0f;
		FurHat.HeatInsulation = 60.0f;
		FurHat.WaterResistance = 50.0f;
		FurHat.WindResistance = 80.0f;
		FurHat.Durability = 100.0f;

		FClothingItem Parka;
		Parka.DisplayName = FText::FromString(TEXT("Arctic Parka"));
		Parka.Slot = EClothingSlot::Torso;
		Parka.ColdInsulation = 95.0f;
		Parka.HeatInsulation = 90.0f;
		Parka.WaterResistance = 85.0f;
		Parka.WindResistance = 95.0f;
		Parka.Durability = 100.0f;

		FClothingItem InsulatedPants;
		InsulatedPants.DisplayName = FText::FromString(TEXT("Insulated Pants"));
		InsulatedPants.Slot = EClothingSlot::Legs;
		InsulatedPants.ColdInsulation = 75.0f;
		InsulatedPants.HeatInsulation = 70.0f;
		InsulatedPants.WaterResistance = 60.0f;
		InsulatedPants.WindResistance = 75.0f;
		InsulatedPants.Durability = 100.0f;

		FClothingItem WinterBoots;
		WinterBoots.DisplayName = FText::FromString(TEXT("Winter Boots"));
		WinterBoots.Slot = EClothingSlot::Feet;
		WinterBoots.ColdInsulation = 80.0f;
		WinterBoots.HeatInsulation = 75.0f;
		WinterBoots.WaterResistance = 90.0f;
		WinterBoots.WindResistance = 85.0f;
		WinterBoots.Durability = 100.0f;

		FClothingItem Gloves;
		Gloves.DisplayName = FText::FromString(TEXT("Insulated Gloves"));
		Gloves.Slot = EClothingSlot::Hands;
		Gloves.ColdInsulation = 65.0f;
		Gloves.HeatInsulation = 60.0f;
		Gloves.WaterResistance = 70.0f;
		Gloves.WindResistance = 80.0f;
		Gloves.Durability = 100.0f;

		WeatherComp->EquipClothing(EClothingSlot::Head, FurHat);
		WeatherComp->EquipClothing(EClothingSlot::Torso, Parka);
		WeatherComp->EquipClothing(EClothingSlot::Legs, InsulatedPants);
		WeatherComp->EquipClothing(EClothingSlot::Feet, WinterBoots);
		WeatherComp->EquipClothing(EClothingSlot::Hands, Gloves);

		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Equipped winter gear"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnEquipDesertGearClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		// Create desert gear (breathable, low heat insulation)
		FClothingItem DesertHat;
		DesertHat.DisplayName = FText::FromString(TEXT("Desert Hat"));
		DesertHat.Slot = EClothingSlot::Head;
		DesertHat.ColdInsulation = 10.0f;
		DesertHat.HeatInsulation = 15.0f; // Low = breathable
		DesertHat.WaterResistance = 20.0f;
		DesertHat.WindResistance = 30.0f;
		DesertHat.Durability = 100.0f;

		FClothingItem DesertRobe;
		DesertRobe.DisplayName = FText::FromString(TEXT("Desert Robe"));
		DesertRobe.Slot = EClothingSlot::Torso;
		DesertRobe.ColdInsulation = 15.0f;
		DesertRobe.HeatInsulation = 20.0f; // Breathable
		DesertRobe.WaterResistance = 25.0f;
		DesertRobe.WindResistance = 40.0f;
		DesertRobe.Durability = 100.0f;

		FClothingItem LightPants;
		LightPants.DisplayName = FText::FromString(TEXT("Light Pants"));
		LightPants.Slot = EClothingSlot::Legs;
		LightPants.ColdInsulation = 10.0f;
		LightPants.HeatInsulation = 15.0f;
		LightPants.WaterResistance = 20.0f;
		LightPants.WindResistance = 25.0f;
		LightPants.Durability = 100.0f;

		FClothingItem Sandals;
		Sandals.DisplayName = FText::FromString(TEXT("Sandals"));
		Sandals.Slot = EClothingSlot::Feet;
		Sandals.ColdInsulation = 5.0f;
		Sandals.HeatInsulation = 10.0f;
		Sandals.WaterResistance = 10.0f;
		Sandals.WindResistance = 15.0f;
		Sandals.Durability = 100.0f;

		WeatherComp->EquipClothing(EClothingSlot::Head, DesertHat);
		WeatherComp->EquipClothing(EClothingSlot::Torso, DesertRobe);
		WeatherComp->EquipClothing(EClothingSlot::Legs, LightPants);
		WeatherComp->EquipClothing(EClothingSlot::Feet, Sandals);

		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Equipped desert gear"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnRemoveAllClothingClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		WeatherComp->RemoveClothing(EClothingSlot::Head);
		WeatherComp->RemoveClothing(EClothingSlot::Torso);
		WeatherComp->RemoveClothing(EClothingSlot::Legs);
		WeatherComp->RemoveClothing(EClothingSlot::Feet);
		WeatherComp->RemoveClothing(EClothingSlot::Hands);
		WeatherComp->RemoveClothing(EClothingSlot::Back);

		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Removed all clothing"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnSoakAllClothingClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		WeatherComp->AddClothingWetness(100.0f);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Soaked all clothing to 100%%"));
	}
	return FReply::Handled();
}

FReply FWeatherComponentDetails::OnDryAllClothingClicked()
{
	if (UWeatherComponent* WeatherComp = GetWeatherComponent())
	{
		WeatherComp->DryClothing(100.0f);
		UE_LOG(LogTemp, Log, TEXT("WeatherComponentDetails: Dried all clothing to 0%%"));
	}
	return FReply::Handled();
}

UWeatherComponent* FWeatherComponentDetails::GetWeatherComponent() const
{
	return CachedWeatherComponent.Get();
}

#undef LOCTEXT_NAMESPACE
