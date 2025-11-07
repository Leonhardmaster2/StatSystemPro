// Copyright Epic Games, Inc. All Rights Reserved.

#include "Details/ProgressionComponentDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "ProgressionComponentDetails"

TSharedRef<IDetailCustomization> FProgressionComponentDetails::MakeInstance()
{
	return MakeShareable(new FProgressionComponentDetails);
}

void FProgressionComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Progression", FText::GetEmpty(), ECategoryPriority::Important);

	Category.AddCustomRow(LOCTEXT("InfoRow", "Info"))
	.WholeRowContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("InfoText", "Progression Component manages XP, levels, and skills. Use the Control Panel for advanced progression tools."))
		.AutoWrapText(true)
	];
}

#undef LOCTEXT_NAMESPACE
