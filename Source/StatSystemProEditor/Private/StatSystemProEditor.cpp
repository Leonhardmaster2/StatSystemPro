// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatSystemProEditor.h"
#include "PropertyEditorModule.h"
#include "ToolMenus.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Docking/SDockTab.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

// Include custom details customizations
#include "Details/StatComponentDetails.h"
#include "Details/BodyComponentDetails.h"
#include "Details/WeatherComponentDetails.h"
#include "Details/StatusEffectComponentDetails.h"
#include "Details/ProgressionComponentDetails.h"

// Include component headers
#include "StatLayer/StatComponent.h"
#include "BodyLayer/BodyComponent.h"
#include "WeatherSystem/WeatherComponent.h"
#include "StatusEffectLayer/StatusEffectComponent.h"
#include "ProgressionLayer/ProgressionComponent.h"

#define LOCTEXT_NAMESPACE "FStatSystemProEditorModule"

static const FName StatSystemProTabName("StatSystemProControlPanel");

void FStatSystemProEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("StatSystemProEditor: Module Starting"));

	// Register custom details panels
	RegisterDetailsCustomizations();

	// Register menu extensions
	RegisterMenuExtensions();

	UE_LOG(LogTemp, Log, TEXT("StatSystemProEditor: Module Started Successfully"));
}

void FStatSystemProEditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("StatSystemProEditor: Module Shutting Down"));

	// Unregister details customizations
	UnregisterDetailsCustomizations();

	// Unregister menu extensions
	UnregisterMenuExtensions();

	UE_LOG(LogTemp, Log, TEXT("StatSystemProEditor: Module Shut Down"));
}

void FStatSystemProEditorModule::RegisterDetailsCustomizations()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// Register custom details for StatComponent
	PropertyModule.RegisterCustomClassLayout(
		UStatComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FStatComponentDetails::MakeInstance)
	);

	// Register custom details for BodyComponent
	PropertyModule.RegisterCustomClassLayout(
		UBodyComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FBodyComponentDetails::MakeInstance)
	);

	// Register custom details for WeatherComponent
	PropertyModule.RegisterCustomClassLayout(
		UWeatherComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FWeatherComponentDetails::MakeInstance)
	);

	// Register custom details for StatusEffectComponent
	PropertyModule.RegisterCustomClassLayout(
		UStatusEffectComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FStatusEffectComponentDetails::MakeInstance)
	);

	// Register custom details for ProgressionComponent
	PropertyModule.RegisterCustomClassLayout(
		UProgressionComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FProgressionComponentDetails::MakeInstance)
	);

	PropertyModule.NotifyCustomizationModuleChanged();

	UE_LOG(LogTemp, Log, TEXT("StatSystemProEditor: Details Customizations Registered"));
}

void FStatSystemProEditorModule::UnregisterDetailsCustomizations()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		PropertyModule.UnregisterCustomClassLayout(UStatComponent::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UBodyComponent::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UWeatherComponent::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UStatusEffectComponent::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UProgressionComponent::StaticClass()->GetFName());

		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

void FStatSystemProEditorModule::RegisterMenuExtensions()
{
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([]()
	{
		FToolMenuOwnerScoped OwnerScoped(UE_MODULE_NAME);

		// Add a menu entry to the level editor toolbar
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		FToolMenuSection& Section = Menu->AddSection("StatSystemPro", LOCTEXT("StatSystemProMenu", "Stat System Pro"));

		Section.AddMenuEntry(
			"OpenStatSystemProControlPanel",
			LOCTEXT("OpenControlPanel", "Stat System Pro Control Panel"),
			LOCTEXT("OpenControlPanelTooltip", "Open the Stat System Pro centralized control panel"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateLambda([]()
			{
				// Load and open the editor utility widget
				UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
				if (EditorUtilitySubsystem)
				{
					// Path to the editor utility widget (will be created)
					FString WidgetPath = TEXT("/StatSystemPro/Editor/EUW_StatSystemProControlPanel.EUW_StatSystemProControlPanel");

					UObject* Widget = LoadObject<UObject>(nullptr, *WidgetPath);
					if (Widget)
					{
						EditorUtilitySubsystem->SpawnAndRegisterTab(Cast<UEditorUtilityWidgetBlueprint>(Widget));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("StatSystemProEditor: Could not find control panel widget at %s"), *WidgetPath);
					}
				}
			}))
		);
	}));

	UE_LOG(LogTemp, Log, TEXT("StatSystemProEditor: Menu Extensions Registered"));
}

void FStatSystemProEditorModule::UnregisterMenuExtensions()
{
	UToolMenus::UnregisterOwner(UE_MODULE_NAME);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStatSystemProEditorModule, StatSystemProEditor)
