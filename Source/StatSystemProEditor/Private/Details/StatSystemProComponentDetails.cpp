// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/StatSystemProComponentDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "StatSystemProComponent.h"
#include "WeatherSystem/WeatherTypes.h"

#define LOCTEXT_NAMESPACE "StatSystemProComponentDetails"

TSharedRef<IDetailCustomization> FStatSystemProComponentDetails::MakeInstance()
{
	return MakeShareable(new FStatSystemProComponentDetails);
}

void FStatSystemProComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	CachedDetailBuilder = &DetailBuilder;

	// Add info category at the top
	IDetailCategoryBuilder& InfoCategory = DetailBuilder.EditCategory("Unified Component Info", FText::GetEmpty(), ECategoryPriority::Important);

	InfoCategory.AddCustomRow(LOCTEXT("UnifiedInfoRow", "Info"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("UnifiedInfoText",
			"UNIFIED COMPONENT - All-in-One StatSystemPro\n\n"
			"✓ ONE component with ALL features\n"
			"✓ Enable/disable layers with checkboxes below\n"
			"✓ QuickSave(\"Slot\") saves EVERYTHING in one call\n"
			"✓ Full multiplayer replication\n\n"
			"Use the quick actions below to test functionality."))
		.AutoWrapText(true)
	];

	// Add quick actions category
	IDetailCategoryBuilder& QuickActionsCategory = DetailBuilder.EditCategory("Quick Actions", FText::GetEmpty(), ECategoryPriority::Important);

	// Stat Layer Quick Actions
	QuickActionsCategory.AddCustomRow(LOCTEXT("StatActionsRow", "Stat Actions"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("StatActionsLabel", "Stat Layer"))
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SBox)
		.WidthOverride(300)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("RestoreAll", "Restore All Stats"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnRestoreAllStatsClicked)
				.ToolTipText(LOCTEXT("RestoreAllTooltip", "Restore all stats to maximum"))
			]
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("DepleteAll", "Deplete All Stats"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnDepleteAllStatsClicked)
				.ToolTipText(LOCTEXT("DepleteAllTooltip", "Set all stats to zero (for testing)"))
			]
		]
	];

	// Body Layer Quick Actions
	QuickActionsCategory.AddCustomRow(LOCTEXT("BodyActionsRow", "Body Actions"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("BodyActionsLabel", "Body Layer"))
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SBox)
		.WidthOverride(300)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("HealAll", "Heal All Body Parts"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnHealAllBodyPartsClicked)
				.ToolTipText(LOCTEXT("HealAllTooltip", "Heal all body parts to full health"))
			]
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("StopBleeding", "Stop All Bleeding"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnStopAllBleedingClicked)
				.ToolTipText(LOCTEXT("StopBleedingTooltip", "Stop bleeding from all body parts"))
			]
		]
	];

	// Weather Layer Quick Actions
	QuickActionsCategory.AddCustomRow(LOCTEXT("WeatherActionsRow", "Weather Actions"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("WeatherActionsLabel", "Weather Layer"))
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SBox)
		.WidthOverride(300)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Blizzard", "Blizzard"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnApplyBlizzardClicked)
				.ToolTipText(LOCTEXT("BlizzardTooltip", "Apply blizzard weather preset"))
			]
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Heatwave", "Heatwave"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnApplyHeatwaveClicked)
				.ToolTipText(LOCTEXT("HeatwaveTooltip", "Apply heatwave weather preset"))
			]
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Clear", "Clear"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnApplyClearWeatherClicked)
				.ToolTipText(LOCTEXT("ClearTooltip", "Apply clear weather"))
			]
		]
	];

	// Status Effects Quick Actions
	QuickActionsCategory.AddCustomRow(LOCTEXT("EffectsActionsRow", "Effects Actions"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("EffectsActionsLabel", "Status Effects"))
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SBox)
		.WidthOverride(300)
		[
			SNew(SButton)
			.Text(LOCTEXT("ClearEffects", "Clear All Effects"))
			.OnClicked(this, &FStatSystemProComponentDetails::OnClearAllEffectsClicked)
			.ToolTipText(LOCTEXT("ClearEffectsTooltip", "Remove all active status effects"))
		]
	];

	// Save/Load Quick Actions
	QuickActionsCategory.AddCustomRow(LOCTEXT("SaveLoadActionsRow", "Save/Load Actions"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("SaveLoadActionsLabel", "Save/Load System"))
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
	]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SBox)
		.WidthOverride(300)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("QuickSave", "QuickSave (\"Test\")"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnQuickSaveClicked)
				.ToolTipText(LOCTEXT("QuickSaveTooltip", "Save everything to slot 'Test'"))
			]
			+ SHorizontalBox::Slot()
			.Padding(2)
			[
				SNew(SButton)
				.Text(LOCTEXT("QuickLoad", "QuickLoad (\"Test\")"))
				.OnClicked(this, &FStatSystemProComponentDetails::OnQuickLoadClicked)
				.ToolTipText(LOCTEXT("QuickLoadTooltip", "Load everything from slot 'Test'"))
			]
		]
	];
}

UStatSystemProComponent* FStatSystemProComponentDetails::GetComponent() const
{
	if (CachedDetailBuilder)
	{
		TArray<TWeakObjectPtr<UObject>> Objects;
		CachedDetailBuilder->GetObjectsBeingCustomized(Objects);

		if (Objects.Num() > 0 && Objects[0].IsValid())
		{
			return Cast<UStatSystemProComponent>(Objects[0].Get());
		}
	}
	return nullptr;
}

FReply FStatSystemProComponentDetails::OnRestoreAllStatsClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		Component->RestoreAllStats();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnDepleteAllStatsClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		Component->DepleteAllStats();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnHealAllBodyPartsClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		Component->HealAllBodyParts();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnStopAllBleedingClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		Component->StopAllBleeding();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnClearAllEffectsClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		Component->ClearAllStatusEffects();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnQuickSaveClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		Component->QuickSave(TEXT("Test"));
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnQuickLoadClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		Component->QuickLoad(TEXT("Test"));
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnApplyBlizzardClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		FWeatherPreset Blizzard;
		Blizzard.PresetName = FName(TEXT("Blizzard"));
		Blizzard.WeatherType = EWeatherType::HeavySnow;
		Blizzard.BaseTemperature = -25.0f;
		Blizzard.TemperatureVariance = 5.0f;
		Blizzard.WindSpeed = 20.0f;
		Blizzard.WindVariance = 5.0f;
		Blizzard.SnowIntensity = 80.0f;

		Component->ApplyWeatherPreset(Blizzard);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnApplyHeatwaveClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		FWeatherPreset Heatwave;
		Heatwave.PresetName = FName(TEXT("Heatwave"));
		Heatwave.WeatherType = EWeatherType::Clear;
		Heatwave.BaseTemperature = 42.0f;
		Heatwave.TemperatureVariance = 3.0f;
		Heatwave.WindSpeed = 2.0f;
		Heatwave.WindVariance = 1.0f;
		Heatwave.RainIntensity = 0.0f;

		Component->ApplyWeatherPreset(Heatwave);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply FStatSystemProComponentDetails::OnApplyClearWeatherClicked()
{
	if (UStatSystemProComponent* Component = GetComponent())
	{
		FWeatherPreset Clear;
		Clear.PresetName = FName(TEXT("Clear"));
		Clear.WeatherType = EWeatherType::Clear;
		Clear.BaseTemperature = 20.0f;
		Clear.TemperatureVariance = 2.0f;
		Clear.WindSpeed = 5.0f;
		Clear.WindVariance = 2.0f;
		Clear.RainIntensity = 0.0f;

		Component->ApplyWeatherPreset(Clear);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
