// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatSystemProBlueprintLibrary.h"

// ========== Component Access ==========

UStatSystemProComponent* UStatSystemProBlueprintLibrary::GetStatSystemComponent(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UStatSystemProComponent>();
}

UStatComponent* UStatSystemProBlueprintLibrary::GetStatLayer(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UStatComponent>();
}

UBodyComponent* UStatSystemProBlueprintLibrary::GetBodyLayer(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UBodyComponent>();
}

UStatusEffectComponent* UStatSystemProBlueprintLibrary::GetStatusEffectLayer(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UStatusEffectComponent>();
}

UEnvironmentComponent* UStatSystemProBlueprintLibrary::GetEnvironmentLayer(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UEnvironmentComponent>();
}

UProgressionComponent* UStatSystemProBlueprintLibrary::GetProgressionLayer(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Actor->FindComponentByClass<UProgressionComponent>();
}

// ========== Quick Stat Functions ==========

float UStatSystemProBlueprintLibrary::GetStatValue(AActor* Actor, EStatType StatType)
{
	UStatComponent* StatComp = GetStatLayer(Actor);
	if (StatComp)
	{
		return StatComp->GetStatValue(StatType);
	}
	return 0.0f;
}

void UStatSystemProBlueprintLibrary::SetStatValue(AActor* Actor, EStatType StatType, float Value)
{
	UStatComponent* StatComp = GetStatLayer(Actor);
	if (StatComp)
	{
		StatComp->SetStatValue(StatType, Value);
	}
}

void UStatSystemProBlueprintLibrary::ChangeStatValue(AActor* Actor, EStatType StatType, float Amount)
{
	UStatComponent* StatComp = GetStatLayer(Actor);
	if (StatComp)
	{
		StatComp->ApplyStatChange(StatType, Amount, TEXT("Blueprint"), FGameplayTag());
	}
}

float UStatSystemProBlueprintLibrary::GetStatPercentage(AActor* Actor, EStatType StatType)
{
	UStatComponent* StatComp = GetStatLayer(Actor);
	if (StatComp)
	{
		return StatComp->GetStatPercentage(StatType);
	}
	return 0.0f;
}

// ========== Quick Body Functions ==========

void UStatSystemProBlueprintLibrary::DamageBodyPart(AActor* Actor, EBodyPart BodyPart, float Damage)
{
	UBodyComponent* BodyComp = GetBodyLayer(Actor);
	if (BodyComp)
	{
		BodyComp->DamageBodyPart(BodyPart, Damage);
	}
}

void UStatSystemProBlueprintLibrary::FractureLimb(AActor* Actor, EBodyPart BodyPart)
{
	UBodyComponent* BodyComp = GetBodyLayer(Actor);
	if (BodyComp)
	{
		BodyComp->FractureLimb(BodyPart);
	}
}

void UStatSystemProBlueprintLibrary::HealBodyPart(AActor* Actor, EBodyPart BodyPart, float Amount)
{
	UBodyComponent* BodyComp = GetBodyLayer(Actor);
	if (BodyComp)
	{
		BodyComp->HealLimb(BodyPart, Amount);
	}
}

FBodyPartState UStatSystemProBlueprintLibrary::GetBodyPartState(AActor* Actor, EBodyPart BodyPart)
{
	UBodyComponent* BodyComp = GetBodyLayer(Actor);
	if (BodyComp)
	{
		return BodyComp->GetBodyPartState(BodyPart);
	}
	return FBodyPartState();
}

// ========== Quick Status Effect Functions ==========

bool UStatSystemProBlueprintLibrary::ApplyStatusEffect(AActor* Actor, FName EffectID, int32 Stacks)
{
	UStatusEffectComponent* EffectComp = GetStatusEffectLayer(Actor);
	if (EffectComp)
	{
		return EffectComp->ApplyStatusEffect(EffectID, Stacks);
	}
	return false;
}

bool UStatSystemProBlueprintLibrary::RemoveStatusEffect(AActor* Actor, FName EffectID)
{
	UStatusEffectComponent* EffectComp = GetStatusEffectLayer(Actor);
	if (EffectComp)
	{
		return EffectComp->RemoveEffect(EffectID);
	}
	return false;
}

bool UStatSystemProBlueprintLibrary::HasStatusEffect(AActor* Actor, FName EffectID)
{
	UStatusEffectComponent* EffectComp = GetStatusEffectLayer(Actor);
	if (EffectComp)
	{
		return EffectComp->HasEffect(EffectID);
	}
	return false;
}

// ========== Quick Environment Functions ==========

void UStatSystemProBlueprintLibrary::SetAmbientTemperature(AActor* Actor, float Temperature)
{
	UEnvironmentComponent* EnvComp = GetEnvironmentLayer(Actor);
	if (EnvComp)
	{
		EnvComp->SetAmbientTemperature(Temperature);
	}
}

void UStatSystemProBlueprintLibrary::SetShelterState(AActor* Actor, EShelterState ShelterState)
{
	UEnvironmentComponent* EnvComp = GetEnvironmentLayer(Actor);
	if (EnvComp)
	{
		EnvComp->SetShelterState(ShelterState);
	}
}

void UStatSystemProBlueprintLibrary::SetClothingInsulation(AActor* Actor, float Insulation)
{
	UEnvironmentComponent* EnvComp = GetEnvironmentLayer(Actor);
	if (EnvComp)
	{
		EnvComp->SetClothingInsulation(Insulation);
	}
}

// ========== Quick Progression Functions ==========

void UStatSystemProBlueprintLibrary::AwardXP(AActor* Actor, int32 Amount, EXPSource Source)
{
	UProgressionComponent* ProgComp = GetProgressionLayer(Actor);
	if (ProgComp)
	{
		ProgComp->AwardXP(Amount, Source);
	}
}

int32 UStatSystemProBlueprintLibrary::GetLevel(AActor* Actor)
{
	UProgressionComponent* ProgComp = GetProgressionLayer(Actor);
	if (ProgComp)
	{
		return ProgComp->GetCurrentLevel();
	}
	return 1;
}

bool UStatSystemProBlueprintLibrary::UnlockSkill(AActor* Actor, FName SkillID)
{
	UProgressionComponent* ProgComp = GetProgressionLayer(Actor);
	if (ProgComp)
	{
		return ProgComp->UnlockSkill(SkillID);
	}
	return false;
}

// ========== Query Functions ==========

bool UStatSystemProBlueprintLibrary::IsCritical(AActor* Actor)
{
	UStatSystemProComponent* StatSys = GetStatSystemComponent(Actor);
	if (StatSys)
	{
		return StatSys->IsCritical();
	}
	return false;
}

bool UStatSystemProBlueprintLibrary::IsDehydrated(AActor* Actor)
{
	UStatSystemProComponent* StatSys = GetStatSystemComponent(Actor);
	if (StatSys)
	{
		return StatSys->IsDehydrated();
	}
	return false;
}

bool UStatSystemProBlueprintLibrary::IsStarving(AActor* Actor)
{
	UStatSystemProComponent* StatSys = GetStatSystemComponent(Actor);
	if (StatSys)
	{
		return StatSys->IsStarving();
	}
	return false;
}

bool UStatSystemProBlueprintLibrary::IsInShock(AActor* Actor)
{
	UStatSystemProComponent* StatSys = GetStatSystemComponent(Actor);
	if (StatSys)
	{
		return StatSys->IsInShock();
	}
	return false;
}
