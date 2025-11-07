// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"

class IDetailLayoutBuilder;
class IDetailCategoryBuilder;
class UStatComponent;

/**
 * Custom details panel for StatComponent
 * Provides enhanced editor controls for stat management
 */
class FStatComponentDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	/** Cached reference to the selected StatComponent */
	TWeakObjectPtr<UStatComponent> CachedStatComponent;

	/** Build the Stat Configuration category */
	void BuildStatConfigurationCategory(IDetailLayoutBuilder& DetailBuilder);

	/** Build the Quick Actions category */
	void BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder);

	/** Build the Stat Testing category */
	void BuildStatTestingCategory(IDetailLayoutBuilder& DetailBuilder);

	/** Build the Visualization category */
	void BuildVisualizationCategory(IDetailLayoutBuilder& DetailBuilder);

	// Quick Action Handlers
	FReply OnRestoreAllStatsClicked();
	FReply OnDepleteAllStatsClicked();
	FReply OnResetAllRegenerationClicked();
	FReply OnTestCriticalConditionClicked();
	FReply OnRandomizeStatsClicked();

	/** Get the stat component being edited */
	UStatComponent* GetStatComponent() const;
};
