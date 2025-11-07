// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"

class IDetailLayoutBuilder;
class UWeatherComponent;

/**
 * Custom details panel for WeatherComponent
 * Provides enhanced editor controls for weather and clothing management
 */
class FWeatherComponentDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	TWeakObjectPtr<UWeatherComponent> CachedWeatherComponent;

	void BuildWeatherPresetsCategory(IDetailLayoutBuilder& DetailBuilder);
	void BuildClothingManagementCategory(IDetailLayoutBuilder& DetailBuilder);
	void BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder);

	// Weather Preset Actions
	FReply OnApplyBlizzardPresetClicked();
	FReply OnApplyHeatwavePresetClicked();
	FReply OnApplyThunderstormPresetClicked();
	FReply OnApplyClearWeatherPresetClicked();

	// Clothing Actions
	FReply OnEquipWinterGearClicked();
	FReply OnEquipDesertGearClicked();
	FReply OnRemoveAllClothingClicked();
	FReply OnSoakAllClothingClicked();
	FReply OnDryAllClothingClicked();

	UWeatherComponent* GetWeatherComponent() const;
};
