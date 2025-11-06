// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatSystemProComponent.h"
#include "GameFramework/Actor.h"

UStatSystemProComponent::UStatSystemProComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	StatLayer = nullptr;
	BodyLayer = nullptr;
	StatusEffectLayer = nullptr;
	EnvironmentLayer = nullptr;
	ProgressionLayer = nullptr;
}

void UStatSystemProComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeLayers();
}

void UStatSystemProComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStatSystemProComponent::InitializeLayers()
{
	EnsureLayerComponents();
	LinkLayerComponents();

	UE_LOG(LogTemp, Log, TEXT("StatSystemPro: All layers initialized and linked"));
}

void UStatSystemProComponent::SetLayerEnabled(FName LayerName, bool bEnabled)
{
	if (LayerName == TEXT("Stat") && StatLayer)
	{
		StatLayer->bEnabled = bEnabled;
	}
	else if (LayerName == TEXT("Body") && BodyLayer)
	{
		BodyLayer->bEnabled = bEnabled;
	}
	else if (LayerName == TEXT("StatusEffect") && StatusEffectLayer)
	{
		StatusEffectLayer->bEnabled = bEnabled;
	}
	else if (LayerName == TEXT("Environment") && EnvironmentLayer)
	{
		EnvironmentLayer->bEnabled = bEnabled;
	}
	else if (LayerName == TEXT("Progression") && ProgressionLayer)
	{
		ProgressionLayer->bEnabled = bEnabled;
	}
}

bool UStatSystemProComponent::IsLayerEnabled(FName LayerName) const
{
	if (LayerName == TEXT("Stat") && StatLayer)
	{
		return StatLayer->bEnabled;
	}
	else if (LayerName == TEXT("Body") && BodyLayer)
	{
		return BodyLayer->bEnabled;
	}
	else if (LayerName == TEXT("StatusEffect") && StatusEffectLayer)
	{
		return StatusEffectLayer->bEnabled;
	}
	else if (LayerName == TEXT("Environment") && EnvironmentLayer)
	{
		return EnvironmentLayer->bEnabled;
	}
	else if (LayerName == TEXT("Progression") && ProgressionLayer)
	{
		return ProgressionLayer->bEnabled;
	}

	return false;
}

bool UStatSystemProComponent::IsCritical() const
{
	if (!StatLayer)
	{
		return false;
	}

	// Critical if health is very low
	if (StatLayer->GetStatPercentage(EStatType::Health_Core) < 0.15f)
	{
		return true;
	}

	// Critical if body has critical injuries
	if (BodyLayer && BodyLayer->HasCriticalInjury())
	{
		return true;
	}

	// Critical if blood level is very low
	if (StatLayer->GetStatPercentage(EStatType::BloodLevel) < 0.3f)
	{
		return true;
	}

	return false;
}

bool UStatSystemProComponent::IsHypothermic() const
{
	if (EnvironmentLayer)
	{
		return EnvironmentLayer->IsHypothermic();
	}

	// Fallback check
	if (StatLayer)
	{
		return StatLayer->GetStatValue(EStatType::BodyTemperature) < 35.0f;
	}

	return false;
}

bool UStatSystemProComponent::IsDehydrated() const
{
	if (!StatLayer)
	{
		return false;
	}

	return StatLayer->GetStatPercentage(EStatType::Thirst) < 0.2f;
}

bool UStatSystemProComponent::IsStarving() const
{
	if (!StatLayer)
	{
		return false;
	}

	return StatLayer->GetStatPercentage(EStatType::Hunger) < 0.15f;
}

bool UStatSystemProComponent::IsInShock() const
{
	if (!StatLayer || !BodyLayer)
	{
		return false;
	}

	// Check for severe blood loss
	if (StatLayer->GetStatPercentage(EStatType::BloodLevel) < 0.4f)
	{
		return true;
	}

	// Check for extreme pain
	if (BodyLayer->GetTotalPainLevel() > 80.0f)
	{
		return true;
	}

	return false;
}

float UStatSystemProComponent::GetOverallHealthStatus() const
{
	if (!StatLayer)
	{
		return 1.0f;
	}

	float HealthPercent = StatLayer->GetStatPercentage(EStatType::Health_Core);
	float BloodPercent = StatLayer->GetStatPercentage(EStatType::BloodLevel);

	float OverallHealth = (HealthPercent * 0.5f) + (BloodPercent * 0.5f);

	// Factor in body condition if available
	if (BodyLayer)
	{
		float BodyCondition = BodyLayer->GetTotalBodyCondition() / 100.0f;
		OverallHealth = (OverallHealth * 0.7f) + (BodyCondition * 0.3f);
	}

	return FMath::Clamp(OverallHealth, 0.0f, 1.0f);
}

void UStatSystemProComponent::ApplyDamageToBodyPart(EBodyPart BodyPart, float Damage)
{
	if (BodyLayer)
	{
		BodyLayer->DamageBodyPart(BodyPart, Damage);
	}

	// Also apply some damage to health
	if (StatLayer)
	{
		StatLayer->ApplyStatChange(EStatType::Health_Core, -Damage * 0.3f, TEXT("BodyDamage"), FGameplayTag());
	}
}

void UStatSystemProComponent::ApplyGeneralDamage(float Damage)
{
	if (StatLayer)
	{
		StatLayer->ApplyStatChange(EStatType::Health_Core, -Damage, TEXT("GeneralDamage"), FGameplayTag());
	}
}

void UStatSystemProComponent::Heal(float Amount)
{
	if (StatLayer)
	{
		StatLayer->ApplyStatChange(EStatType::Health_Core, Amount, TEXT("Healing"), FGameplayTag());
	}
}

void UStatSystemProComponent::Consume(float HungerRestore, float ThirstRestore, float EnergyRestore)
{
	if (!StatLayer)
	{
		return;
	}

	if (HungerRestore > 0.0f)
	{
		StatLayer->ApplyStatChange(EStatType::Hunger, HungerRestore, TEXT("Consume"), FGameplayTag());
	}

	if (ThirstRestore > 0.0f)
	{
		StatLayer->ApplyStatChange(EStatType::Thirst, ThirstRestore, TEXT("Consume"), FGameplayTag());
	}

	if (EnergyRestore > 0.0f)
	{
		StatLayer->ApplyStatChange(EStatType::Energy, EnergyRestore, TEXT("Consume"), FGameplayTag());
	}
}

void UStatSystemProComponent::ApplyEffect(FName EffectID, int32 Stacks)
{
	if (StatusEffectLayer)
	{
		StatusEffectLayer->ApplyStatusEffect(EffectID, Stacks);
	}
}

void UStatSystemProComponent::RemoveEffect(FName EffectID)
{
	if (StatusEffectLayer)
	{
		StatusEffectLayer->RemoveEffect(EffectID);
	}
}

void UStatSystemProComponent::AwardExperience(int32 Amount, EXPSource Source)
{
	if (ProgressionLayer)
	{
		ProgressionLayer->AwardXP(Amount, Source);
	}
}

FString UStatSystemProComponent::GetDebugInfo() const
{
	FString Info = TEXT("=== StatSystemPro Debug Info ===\n\n");

	// Stat Layer
	if (StatLayer && StatLayer->bEnabled)
	{
		Info += TEXT("STAT LAYER:\n");
		Info += FString::Printf(TEXT("  Health: %.1f/%.1f (%.0f%%)\n"),
			StatLayer->GetStatValue(EStatType::Health_Core),
			StatLayer->GetStatMaxValue(EStatType::Health_Core),
			StatLayer->GetStatPercentage(EStatType::Health_Core) * 100.0f);
		Info += FString::Printf(TEXT("  Stamina: %.1f/%.1f\n"),
			StatLayer->GetStatValue(EStatType::Stamina),
			StatLayer->GetStatMaxValue(EStatType::Stamina));
		Info += FString::Printf(TEXT("  Hunger: %.1f%%\n"), StatLayer->GetStatPercentage(EStatType::Hunger) * 100.0f);
		Info += FString::Printf(TEXT("  Thirst: %.1f%%\n"), StatLayer->GetStatPercentage(EStatType::Thirst) * 100.0f);
		Info += TEXT("\n");
	}

	// Body Layer
	if (BodyLayer && BodyLayer->bEnabled)
	{
		Info += TEXT("BODY LAYER:\n");
		Info += FString::Printf(TEXT("  Total Body Condition: %.1f%%\n"), BodyLayer->GetTotalBodyCondition());
		Info += FString::Printf(TEXT("  Total Bleeding: %.2f/s\n"), BodyLayer->GetTotalBleedingRate());
		Info += FString::Printf(TEXT("  Pain Level: %.1f\n"), BodyLayer->GetTotalPainLevel());
		Info += FString::Printf(TEXT("  Critical Injury: %s\n"), BodyLayer->HasCriticalInjury() ? TEXT("YES") : TEXT("NO"));
		Info += TEXT("\n");
	}

	// Status Effect Layer
	if (StatusEffectLayer && StatusEffectLayer->bEnabled)
	{
		Info += FString::Printf(TEXT("STATUS EFFECTS: %d active\n\n"), StatusEffectLayer->ActiveEffects.Num());
	}

	// Environment Layer
	if (EnvironmentLayer && EnvironmentLayer->bEnabled)
	{
		FEnvironmentState Env = EnvironmentLayer->GetEnvironmentState();
		Info += TEXT("ENVIRONMENT LAYER:\n");
		Info += FString::Printf(TEXT("  Ambient Temp: %.1f°C\n"), Env.AmbientTemperature);
		Info += FString::Printf(TEXT("  Body Temp: %.1f°C\n"), StatLayer ? StatLayer->GetStatValue(EStatType::BodyTemperature) : 0.0f);
		Info += FString::Printf(TEXT("  Wetness: %.1f%%\n"), StatLayer ? StatLayer->GetStatPercentage(EStatType::Wetness) * 100.0f : 0.0f);
		Info += TEXT("\n");
	}

	// Progression Layer
	if (ProgressionLayer && ProgressionLayer->bEnabled)
	{
		Info += TEXT("PROGRESSION LAYER:\n");
		Info += FString::Printf(TEXT("  Level: %d\n"), ProgressionLayer->GetCurrentLevel());
		Info += FString::Printf(TEXT("  XP: %d/%d (%.0f%%)\n"),
			ProgressionLayer->GetCurrentXP(),
			ProgressionLayer->GetXPForNextLevel(),
			ProgressionLayer->GetLevelProgress() * 100.0f);
		Info += FString::Printf(TEXT("  Unlocked Skills: %d\n"), ProgressionLayer->UnlockedSkills.Num());
		Info += TEXT("\n");
	}

	// Overall Status
	Info += TEXT("OVERALL STATUS:\n");
	Info += FString::Printf(TEXT("  Health Status: %.0f%%\n"), GetOverallHealthStatus() * 100.0f);
	Info += FString::Printf(TEXT("  Critical: %s\n"), IsCritical() ? TEXT("YES") : TEXT("NO"));
	Info += FString::Printf(TEXT("  In Shock: %s\n"), IsInShock() ? TEXT("YES") : TEXT("NO"));

	return Info;
}

void UStatSystemProComponent::EnsureLayerComponents()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// Find or create Stat Layer
	if (!StatLayer)
	{
		StatLayer = Owner->FindComponentByClass<UStatComponent>();
		if (!StatLayer)
		{
			StatLayer = NewObject<UStatComponent>(Owner, UStatComponent::StaticClass(), TEXT("StatLayer"));
			StatLayer->RegisterComponent();
		}
	}

	// Find or create Body Layer
	if (!BodyLayer)
	{
		BodyLayer = Owner->FindComponentByClass<UBodyComponent>();
		if (!BodyLayer)
		{
			BodyLayer = NewObject<UBodyComponent>(Owner, UBodyComponent::StaticClass(), TEXT("BodyLayer"));
			BodyLayer->RegisterComponent();
		}
	}

	// Find or create Status Effect Layer
	if (!StatusEffectLayer)
	{
		StatusEffectLayer = Owner->FindComponentByClass<UStatusEffectComponent>();
		if (!StatusEffectLayer)
		{
			StatusEffectLayer = NewObject<UStatusEffectComponent>(Owner, UStatusEffectComponent::StaticClass(), TEXT("StatusEffectLayer"));
			StatusEffectLayer->RegisterComponent();
		}
	}

	// Find or create Environment Layer
	if (!EnvironmentLayer)
	{
		EnvironmentLayer = Owner->FindComponentByClass<UEnvironmentComponent>();
		if (!EnvironmentLayer)
		{
			EnvironmentLayer = NewObject<UEnvironmentComponent>(Owner, UEnvironmentComponent::StaticClass(), TEXT("EnvironmentLayer"));
			EnvironmentLayer->RegisterComponent();
		}
	}

	// Find or create Progression Layer
	if (!ProgressionLayer)
	{
		ProgressionLayer = Owner->FindComponentByClass<UProgressionComponent>();
		if (!ProgressionLayer)
		{
			ProgressionLayer = NewObject<UProgressionComponent>(Owner, UProgressionComponent::StaticClass(), TEXT("ProgressionLayer"));
			ProgressionLayer->RegisterComponent();
		}
	}
}

void UStatSystemProComponent::LinkLayerComponents()
{
	// Link Body Layer to Stat Layer
	if (BodyLayer && StatLayer)
	{
		BodyLayer->StatComponent = StatLayer;
	}

	// Link Status Effect Layer to Stat Layer
	if (StatusEffectLayer && StatLayer)
	{
		StatusEffectLayer->StatComponent = StatLayer;
	}

	// Link Environment Layer to Stat and Status Effect Layers
	if (EnvironmentLayer)
	{
		if (StatLayer)
		{
			EnvironmentLayer->StatComponent = StatLayer;
		}
		if (StatusEffectLayer)
		{
			EnvironmentLayer->StatusEffectComponent = StatusEffectLayer;
		}
	}

	// Link Progression Layer to Stat and Status Effect Layers
	if (ProgressionLayer)
	{
		if (StatLayer)
		{
			ProgressionLayer->StatComponent = StatLayer;
		}
		if (StatusEffectLayer)
		{
			ProgressionLayer->StatusEffectComponent = StatusEffectLayer;
		}
	}
}
