// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * StatSystemPro Editor Module
 * Provides custom editor tools, details panels, and utilities for the StatSystemPro plugin
 */
class FStatSystemProEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Register custom details panels */
	void RegisterDetailsCustomizations();

	/** Unregister custom details panels */
	void UnregisterDetailsCustomizations();

	/** Register editor menu extensions */
	void RegisterMenuExtensions();

	/** Unregister editor menu extensions */
	void UnregisterMenuExtensions();

	/** Handle for the registered menu extension */
	TSharedPtr<class FExtensibilityManager> LevelEditorMenuExtensibilityManager;
};
