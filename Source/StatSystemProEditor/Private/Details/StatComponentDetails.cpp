// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/StatComponentDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "StatLayer/StatComponent.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "StatComponentDetails"

TSharedRef<IDetailCustomization> FStatComponentDetails::MakeInstance()
{
	return MakeShareable(new FStatComponentDetails);
}

void FStatComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Get the object being customized
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

	if (ObjectsBeingCustomized.Num() > 0)
	{
		CachedStatComponent = Cast<UStatComponent>(ObjectsBeingCustomized[0].Get());
	}

	if (!CachedStatComponent.IsValid())
	{
		return;
	}

	// Build custom categories
	BuildQuickActionsCategory(DetailBuilder);
	BuildStatTestingCategory(DetailBuilder);
	BuildStatConfigurationCategory(DetailBuilder);
	BuildVisualizationCategory(DetailBuilder);
}

void FStatComponentDetails::BuildStatConfigurationCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Stat Configuration", FText::GetEmpty(), ECategoryPriority::Important);

	Category.AddCustomRow(LOCTEXT("StatConfigHelp", "Stat Configuration Help"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("StatConfigHelpText", "Configure individual stats below. Each stat has default values, regeneration rates, and optional curves for non-linear regeneration."))
		.AutoWrapText(true)
	];
}

void FStatComponentDetails::BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Quick Actions", FText::GetEmpty(), ECategoryPriority::Important);

	Category.AddCustomRow(LOCTEXT("QuickActionsRow", "Quick Actions"))
	.WholeRowContent()
	[
		SNew(SUniformGridPanel)
		.SlotPadding(2.0f)

		// Row 1
		+ SUniformGridPanel::Slot(0, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("RestoreAllStats", "Restore All Stats"))
			.ToolTipText(LOCTEXT("RestoreAllStatsTooltip", "Set all stats to their maximum values"))
			.OnClicked(this, &FStatComponentDetails::OnRestoreAllStatsClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("DepleteAllStats", "Deplete All Stats"))
			.ToolTipText(LOCTEXT("DepleteAllStatsTooltip", "Set all stats to zero (for testing critical conditions)"))
			.OnClicked(this, &FStatComponentDetails::OnDepleteAllStatsClicked)
			.HAlign(HAlign_Center)
		]

		// Row 2
		+ SUniformGridPanel::Slot(0, 1)
		[
			SNew(SButton)
			.Text(LOCTEXT("RandomizeStats", "Randomize Stats"))
			.ToolTipText(LOCTEXT("RandomizeStatsTooltip", "Set all stats to random values (for testing)"))
			.OnClicked(this, &FStatComponentDetails::OnRandomizeStatsClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 1)
		[
			SNew(SButton)
			.Text(LOCTEXT("ResetRegeneration", "Reset Regeneration"))
			.ToolTipText(LOCTEXT("ResetRegenerationTooltip", "Reset all regeneration rates to defaults"))
			.OnClicked(this, &FStatComponentDetails::OnResetAllRegenerationClicked)
			.HAlign(HAlign_Center)
		]
	];
}

void FStatComponentDetails::BuildStatTestingCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Stat Testing", FText::GetEmpty(), ECategoryPriority::Default);

	Category.AddCustomRow(LOCTEXT("TestingActionsRow", "Testing Actions"))
	.WholeRowContent()
	[
		SNew(SUniformGridPanel)
		.SlotPadding(2.0f)

		+ SUniformGridPanel::Slot(0, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("TestCritical", "Test Critical Condition"))
			.ToolTipText(LOCTEXT("TestCriticalTooltip", "Set Health_Core to 10% to test critical condition"))
			.OnClicked(this, &FStatComponentDetails::OnTestCriticalConditionClicked)
			.HAlign(HAlign_Center)
		]
	];

	// Add informational text
	Category.AddCustomRow(LOCTEXT("TestingInfo", "Testing Info"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("TestingInfoText", "Use these buttons to quickly test various stat conditions in the editor."))
		.AutoWrapText(true)
	];
}

void FStatComponentDetails::BuildVisualizationCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Stat Visualization", FText::GetEmpty(), ECategoryPriority::Default);

	Category.AddCustomRow(LOCTEXT("VisualizationRow", "Visualization"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("VisualizationText", "Stat visualization options will appear here. Use the Control Panel for advanced visualization."))
		.AutoWrapText(true)
	];
}

FReply FStatComponentDetails::OnRestoreAllStatsClicked()
{
	if (UStatComponent* StatComp = GetStatComponent())
	{
		StatComp->RestoreAllStatsToMax();
		UE_LOG(LogTemp, Log, TEXT("StatComponentDetails: Restored all stats to maximum"));
	}
	return FReply::Handled();
}

FReply FStatComponentDetails::OnDepleteAllStatsClicked()
{
	if (UStatComponent* StatComp = GetStatComponent())
	{
		// Set all stats to zero
		for (int32 i = 0; i < (int32)EStatType::MAX; ++i)
		{
			EStatType StatType = (EStatType)i;
			StatComp->SetStatValue(StatType, 0.0f);
		}
		UE_LOG(LogTemp, Log, TEXT("StatComponentDetails: Depleted all stats to zero"));
	}
	return FReply::Handled();
}

FReply FStatComponentDetails::OnResetAllRegenerationClicked()
{
	if (UStatComponent* StatComp = GetStatComponent())
	{
		// Reset regeneration rates to defaults
		for (int32 i = 0; i < (int32)EStatType::MAX; ++i)
		{
			EStatType StatType = (EStatType)i;
			StatComp->SetStatRegenerationRate(StatType, 0.0f);
		}
		UE_LOG(LogTemp, Log, TEXT("StatComponentDetails: Reset all regeneration rates"));
	}
	return FReply::Handled();
}

FReply FStatComponentDetails::OnTestCriticalConditionClicked()
{
	if (UStatComponent* StatComp = GetStatComponent())
	{
		// Set health to 10% to trigger critical condition
		float MaxHealth = StatComp->GetStatMaxValue(EStatType::Health_Core);
		StatComp->SetStatValue(EStatType::Health_Core, MaxHealth * 0.10f);
		UE_LOG(LogTemp, Log, TEXT("StatComponentDetails: Set to critical condition (10%% health)"));
	}
	return FReply::Handled();
}

FReply FStatComponentDetails::OnRandomizeStatsClicked()
{
	if (UStatComponent* StatComp = GetStatComponent())
	{
		// Randomize all stats
		for (int32 i = 0; i < (int32)EStatType::MAX; ++i)
		{
			EStatType StatType = (EStatType)i;
			float MaxValue = StatComp->GetStatMaxValue(StatType);
			float RandomValue = FMath::FRandRange(0.0f, MaxValue);
			StatComp->SetStatValue(StatType, RandomValue);
		}
		UE_LOG(LogTemp, Log, TEXT("StatComponentDetails: Randomized all stats"));
	}
	return FReply::Handled();
}

UStatComponent* FStatComponentDetails::GetStatComponent() const
{
	return CachedStatComponent.Get();
}

#undef LOCTEXT_NAMESPACE
