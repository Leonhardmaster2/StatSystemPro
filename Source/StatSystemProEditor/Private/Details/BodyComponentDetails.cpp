// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/BodyComponentDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "BodyLayer/BodyComponent.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#define LOCTEXT_NAMESPACE "BodyComponentDetails"

TSharedRef<IDetailCustomization> FBodyComponentDetails::MakeInstance()
{
	return MakeShareable(new FBodyComponentDetails);
}

void FBodyComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

	if (ObjectsBeingCustomized.Num() > 0)
	{
		CachedBodyComponent = Cast<UBodyComponent>(ObjectsBeingCustomized[0].Get());
	}

	if (!CachedBodyComponent.IsValid())
	{
		return;
	}

	BuildQuickActionsCategory(DetailBuilder);
	BuildBodyPartTestingCategory(DetailBuilder);
	BuildVisualizationCategory(DetailBuilder);
}

void FBodyComponentDetails::BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder)
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
			.Text(LOCTEXT("HealAllBodyParts", "Heal All Body Parts"))
			.ToolTipText(LOCTEXT("HealAllBodyPartsTooltip", "Restore all body parts to full condition"))
			.OnClicked(this, &FBodyComponentDetails::OnHealAllBodyPartsClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("DamageAllBodyParts", "Damage All Body Parts"))
			.ToolTipText(LOCTEXT("DamageAllBodyPartsTooltip", "Apply 50% damage to all body parts"))
			.OnClicked(this, &FBodyComponentDetails::OnDamageAllBodyPartsClicked)
			.HAlign(HAlign_Center)
		]

		// Row 2
		+ SUniformGridPanel::Slot(0, 1)
		[
			SNew(SButton)
			.Text(LOCTEXT("HealAllFractures", "Heal All Fractures"))
			.ToolTipText(LOCTEXT("HealAllFracturesTooltip", "Remove all fractures from body parts"))
			.OnClicked(this, &FBodyComponentDetails::OnHealAllFracturesClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 1)
		[
			SNew(SButton)
			.Text(LOCTEXT("StopAllBleeding", "Stop All Bleeding"))
			.ToolTipText(LOCTEXT("StopAllBleedingTooltip", "Stop bleeding on all body parts"))
			.OnClicked(this, &FBodyComponentDetails::OnStopAllBleedingClicked)
			.HAlign(HAlign_Center)
		]
	];
}

void FBodyComponentDetails::BuildBodyPartTestingCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Body Part Testing", FText::GetEmpty(), ECategoryPriority::Default);

	Category.AddCustomRow(LOCTEXT("TestingActionsRow", "Testing Actions"))
	.WholeRowContent()
	[
		SNew(SUniformGridPanel)
		.SlotPadding(2.0f)

		+ SUniformGridPanel::Slot(0, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("FractureRandomPart", "Fracture Random Body Part"))
			.ToolTipText(LOCTEXT("FractureRandomPartTooltip", "Randomly fracture one body part for testing"))
			.OnClicked(this, &FBodyComponentDetails::OnFractureRandomBodyPartClicked)
			.HAlign(HAlign_Center)
		]

		+ SUniformGridPanel::Slot(1, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("CauseBleeding", "Cause Bleeding"))
			.ToolTipText(LOCTEXT("CauseBleedingTooltip", "Cause bleeding on random body parts"))
			.OnClicked(this, &FBodyComponentDetails::OnCauseBleedingClicked)
			.HAlign(HAlign_Center)
		]
	];
}

void FBodyComponentDetails::BuildVisualizationCategory(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Body Visualization", FText::GetEmpty(), ECategoryPriority::Default);

	Category.AddCustomRow(LOCTEXT("VisualizationRow", "Visualization"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("VisualizationText", "Body part visualization and health display. Use the Control Panel for advanced body management."))
		.AutoWrapText(true)
	];
}

FReply FBodyComponentDetails::OnHealAllBodyPartsClicked()
{
	if (UBodyComponent* BodyComp = GetBodyComponent())
	{
		BodyComp->HealAllBodyParts(100.0f);
		UE_LOG(LogTemp, Log, TEXT("BodyComponentDetails: Healed all body parts"));
	}
	return FReply::Handled();
}

FReply FBodyComponentDetails::OnDamageAllBodyPartsClicked()
{
	if (UBodyComponent* BodyComp = GetBodyComponent())
	{
		// Damage all body parts by 50%
		for (int32 i = 0; i < (int32)EBodyPart::MAX; ++i)
		{
			EBodyPart BodyPart = (EBodyPart)i;
			float MaxCondition = BodyComp->GetBodyPartMaxCondition(BodyPart);
			BodyComp->DamageBodyPart(BodyPart, MaxCondition * 0.5f);
		}
		UE_LOG(LogTemp, Log, TEXT("BodyComponentDetails: Damaged all body parts by 50%%"));
	}
	return FReply::Handled();
}

FReply FBodyComponentDetails::OnFractureRandomBodyPartClicked()
{
	if (UBodyComponent* BodyComp = GetBodyComponent())
	{
		// Pick a random body part (excluding MAX)
		int32 RandomIndex = FMath::RandRange(0, (int32)EBodyPart::MAX - 1);
		EBodyPart RandomPart = (EBodyPart)RandomIndex;

		BodyComp->FractureLimb(RandomPart);
		UE_LOG(LogTemp, Log, TEXT("BodyComponentDetails: Fractured body part %d"), RandomIndex);
	}
	return FReply::Handled();
}

FReply FBodyComponentDetails::OnCauseBleedingClicked()
{
	if (UBodyComponent* BodyComp = GetBodyComponent())
	{
		// Cause bleeding on 2-3 random body parts
		int32 BleedingCount = FMath::RandRange(2, 3);
		for (int32 i = 0; i < BleedingCount; ++i)
		{
			int32 RandomIndex = FMath::RandRange(0, (int32)EBodyPart::MAX - 1);
			EBodyPart RandomPart = (EBodyPart)RandomIndex;
			BodyComp->SetBodyPartBleedingRate(RandomPart, FMath::FRandRange(5.0f, 15.0f));
		}
		UE_LOG(LogTemp, Log, TEXT("BodyComponentDetails: Caused bleeding on %d body parts"), BleedingCount);
	}
	return FReply::Handled();
}

FReply FBodyComponentDetails::OnHealAllFracturesClicked()
{
	if (UBodyComponent* BodyComp = GetBodyComponent())
	{
		for (int32 i = 0; i < (int32)EBodyPart::MAX; ++i)
		{
			EBodyPart BodyPart = (EBodyPart)i;
			BodyComp->HealFracture(BodyPart);
		}
		UE_LOG(LogTemp, Log, TEXT("BodyComponentDetails: Healed all fractures"));
	}
	return FReply::Handled();
}

FReply FBodyComponentDetails::OnStopAllBleedingClicked()
{
	if (UBodyComponent* BodyComp = GetBodyComponent())
	{
		BodyComp->StopAllBleeding();
		UE_LOG(LogTemp, Log, TEXT("BodyComponentDetails: Stopped all bleeding"));
	}
	return FReply::Handled();
}

UBodyComponent* FBodyComponentDetails::GetBodyComponent() const
{
	return CachedBodyComponent.Get();
}

#undef LOCTEXT_NAMESPACE
