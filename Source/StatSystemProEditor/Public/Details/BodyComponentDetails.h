// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"

class IDetailLayoutBuilder;
class UBodyComponent;

/**
 * Custom details panel for BodyComponent
 * Provides enhanced editor controls for body part management
 */
class FBodyComponentDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	TWeakObjectPtr<UBodyComponent> CachedBodyComponent;

	void BuildQuickActionsCategory(IDetailLayoutBuilder& DetailBuilder);
	void BuildBodyPartTestingCategory(IDetailLayoutBuilder& DetailBuilder);
	void BuildVisualizationCategory(IDetailLayoutBuilder& DetailBuilder);

	// Quick Actions
	FReply OnHealAllBodyPartsClicked();
	FReply OnDamageAllBodyPartsClicked();
	FReply OnFractureRandomBodyPartClicked();
	FReply OnCauseBleedingClicked();
	FReply OnHealAllFracturesClicked();
	FReply OnStopAllBleedingClicked();

	UBodyComponent* GetBodyComponent() const;
};
