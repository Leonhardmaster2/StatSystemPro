// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/StatusEffectComponentDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "StatusEffectComponentDetails"

TSharedRef<IDetailCustomization> FStatusEffectComponentDetails::MakeInstance()
{
	return MakeShareable(new FStatusEffectComponentDetails);
}

void FStatusEffectComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Status Effects", FText::GetEmpty(), ECategoryPriority::Important);

	Category.AddCustomRow(LOCTEXT("InfoRow", "Info"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("InfoText", "Status Effect Component provides buff/debuff management. Use the Control Panel for advanced status effect tools."))
		.AutoWrapText(true)
	];
}

#undef LOCTEXT_NAMESPACE
