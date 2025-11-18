// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;
class UStatSystemProComponent;

/**
 * Custom details panel for the UNIFIED StatSystemProComponent
 * Provides quick actions and layer management
 */
class FStatSystemProComponentDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	/** Get the component being customized */
	UStatSystemProComponent* GetComponent() const;

	/** Quick action buttons */
	FReply OnRestoreAllStatsClicked();
	FReply OnDepleteAllStatsClicked();
	FReply OnHealAllBodyPartsClicked();
	FReply OnStopAllBleedingClicked();
	FReply OnClearAllEffectsClicked();
	FReply OnQuickSaveClicked();
	FReply OnQuickLoadClicked();

	/** Weather presets */
	FReply OnApplyBlizzardClicked();
	FReply OnApplyHeatwaveClicked();
	FReply OnApplyClearWeatherClicked();

	/** Reference to the detail builder */
	IDetailLayoutBuilder* CachedDetailBuilder;
};
