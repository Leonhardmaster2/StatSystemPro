// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProgressionLayer/ProgressionComponent.h"
#include "StatLayer/StatComponent.h"
#include "StatusEffectLayer/StatusEffectComponent.h"
#include "Engine/DataTable.h"

UProgressionComponent::UProgressionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
	StatComponent = nullptr;
	StatusEffectComponent = nullptr;
	BaseXPPerLevel = 100;
	XPMultiplierPerLevel = 1.5f;
	bGrantSurvivalXP = true;
	XPPerMinuteSurvived = 5.0f;
	SurvivalXPTimer = 0.0f;
	XPCurve = nullptr;
	SkillTable = nullptr;
}

void UProgressionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Try to find components on the same actor
	if (!StatComponent)
	{
		StatComponent = GetOwner()->FindComponentByClass<UStatComponent>();
	}

	if (!StatusEffectComponent)
	{
		StatusEffectComponent = GetOwner()->FindComponentByClass<UStatusEffectComponent>();
	}

	// Calculate initial XP requirement
	ProgressionData.XPForNextLevel = CalculateXPForLevel(ProgressionData.CurrentLevel + 1);
}

void UProgressionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnabled)
	{
		return;
	}

	// Track survival time
	ProgressionData.TotalTimeSurvived += DeltaTime;

	// Grant survival XP
	if (bGrantSurvivalXP && XPPerMinuteSurvived > 0.0f)
	{
		SurvivalXPTimer += DeltaTime;

		if (SurvivalXPTimer >= 60.0f) // Every minute
		{
			int32 XPToGrant = FMath::FloorToInt(XPPerMinuteSurvived);
			AwardXP(XPToGrant, EXPSource::Survival);
			SurvivalXPTimer -= 60.0f;
		}
	}

	ApplySkillEffects();
}

void UProgressionComponent::AwardXP(int32 Amount, EXPSource Source)
{
	if (!bEnabled || Amount <= 0)
	{
		return;
	}

	ProgressionData.CurrentXP += Amount;
	OnXPGained.Broadcast(Amount, Source);

	UE_LOG(LogTemp, Log, TEXT("XP Gained: %d | Source: %d | Total: %d/%d"),
		Amount, (int32)Source, ProgressionData.CurrentXP, ProgressionData.XPForNextLevel);

	CheckLevelUp();
}

bool UProgressionComponent::UnlockSkill(FName SkillID)
{
	if (!bEnabled)
	{
		return false;
	}

	// Check if already unlocked
	if (IsSkillUnlocked(SkillID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill already unlocked: %s"), *SkillID.ToString());
		return false;
	}

	// Check if can unlock
	if (!CanUnlockSkill(SkillID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot unlock skill: %s (requirements not met)"), *SkillID.ToString());
		return false;
	}

	FSkillData* SkillData = FindSkillData(SkillID);
	if (!SkillData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill not found: %s"), *SkillID.ToString());
		return false;
	}

	// Spend skill points
	if (!SpendSkillPoints(SkillData->SkillPointCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough skill points to unlock: %s"), *SkillID.ToString());
		return false;
	}

	// Unlock the skill
	FActiveSkill NewSkill(*SkillData);
	NewSkill.TimeUnlocked = GetWorld()->GetTimeSeconds();
	UnlockedSkills.Add(NewSkill);

	OnSkillUnlocked.Broadcast(SkillID, 1);

	UE_LOG(LogTemp, Log, TEXT("Skill Unlocked: %s"), *SkillID.ToString());

	return true;
}

bool UProgressionComponent::LevelUpSkill(FName SkillID)
{
	int32 SkillIndex = FindActiveSkillIndex(SkillID);
	if (SkillIndex == INDEX_NONE)
	{
		return false;
	}

	FActiveSkill& Skill = UnlockedSkills[SkillIndex];

	if (Skill.IsMaxLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill already at max level: %s"), *SkillID.ToString());
		return false;
	}

	// Spend skill points
	if (!SpendSkillPoints(Skill.SkillData.SkillPointCost))
	{
		return false;
	}

	Skill.CurrentLevel++;
	OnSkillUnlocked.Broadcast(SkillID, Skill.CurrentLevel);

	UE_LOG(LogTemp, Log, TEXT("Skill Leveled Up: %s | New Level: %d"), *SkillID.ToString(), Skill.CurrentLevel);

	return true;
}

bool UProgressionComponent::IsSkillUnlocked(FName SkillID) const
{
	return FindActiveSkillIndex(SkillID) != INDEX_NONE;
}

int32 UProgressionComponent::GetSkillLevel(FName SkillID) const
{
	int32 Index = FindActiveSkillIndex(SkillID);
	if (Index != INDEX_NONE)
	{
		return UnlockedSkills[Index].CurrentLevel;
	}
	return 0;
}

bool UProgressionComponent::CanUnlockSkill(FName SkillID) const
{
	FSkillData* SkillData = const_cast<UProgressionComponent*>(this)->FindSkillData(SkillID);
	if (!SkillData)
	{
		return false;
	}

	// Check level requirement
	if (ProgressionData.CurrentLevel < SkillData->RequiredLevel)
	{
		return false;
	}

	// Check skill point requirement
	if (ProgressionData.SkillPoints < SkillData->SkillPointCost)
	{
		return false;
	}

	// Check prerequisites
	for (const FName& Prerequisite : SkillData->Prerequisites)
	{
		if (!IsSkillUnlocked(Prerequisite))
		{
			return false;
		}
	}

	return true;
}

bool UProgressionComponent::SpendAttributePoints(int32 Amount)
{
	if (ProgressionData.AttributePoints < Amount)
	{
		return false;
	}

	ProgressionData.AttributePoints -= Amount;
	OnAttributePointsChanged.Broadcast(ProgressionData.AttributePoints, -Amount);
	return true;
}

bool UProgressionComponent::SpendSkillPoints(int32 Amount)
{
	if (ProgressionData.SkillPoints < Amount)
	{
		return false;
	}

	ProgressionData.SkillPoints -= Amount;
	OnSkillPointsChanged.Broadcast(ProgressionData.SkillPoints, -Amount);
	return true;
}

void UProgressionComponent::CheckLevelUp()
{
	while (ProgressionData.CurrentXP >= ProgressionData.XPForNextLevel)
	{
		// Level up!
		ProgressionData.CurrentLevel++;
		ProgressionData.CurrentXP -= ProgressionData.XPForNextLevel;

		// Grant attribute and skill points
		ProgressionData.AttributePoints += 3; // Grant 3 attribute points per level
		ProgressionData.SkillPoints += 1;     // Grant 1 skill point per level

		OnLevelUp.Broadcast(ProgressionData.CurrentLevel);
		OnAttributePointsChanged.Broadcast(ProgressionData.AttributePoints, 3);
		OnSkillPointsChanged.Broadcast(ProgressionData.SkillPoints, 1);

		// Calculate new XP requirement
		ProgressionData.XPForNextLevel = CalculateXPForLevel(ProgressionData.CurrentLevel + 1);

		UE_LOG(LogTemp, Log, TEXT("LEVEL UP! New Level: %d | XP for next: %d"),
			ProgressionData.CurrentLevel, ProgressionData.XPForNextLevel);
	}
}

int32 UProgressionComponent::CalculateXPForLevel(int32 Level) const
{
	if (XPCurve)
	{
		return FMath::FloorToInt(XPCurve->GetFloatValue((float)Level));
	}

	// Default formula: BaseXP * (Multiplier ^ (Level - 1))
	return FMath::FloorToInt(BaseXPPerLevel * FMath::Pow(XPMultiplierPerLevel, Level - 1));
}

FSkillData* UProgressionComponent::FindSkillData(FName SkillID)
{
	if (!SkillTable)
	{
		return nullptr;
	}

	FString ContextString(TEXT("ProgressionComponent::FindSkillData"));
	FSkillTableRow* Row = SkillTable->FindRow<FSkillTableRow>(SkillID, ContextString);

	if (Row)
	{
		return &Row->SkillData;
	}

	return nullptr;
}

int32 UProgressionComponent::FindActiveSkillIndex(FName SkillID) const
{
	for (int32 i = 0; i < UnlockedSkills.Num(); ++i)
	{
		if (UnlockedSkills[i].SkillData.SkillID == SkillID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

void UProgressionComponent::ApplySkillEffects()
{
	// This would apply passive effects from skills
	// For example:
	// - "Resilient" reduces fatigue buildup
	// - "Iron Lungs" increases stamina regeneration
	// - "Pathfinder" reduces energy consumption while moving

	// This is a placeholder - in a full implementation, you would
	// iterate through unlocked skills and apply their effects
	// to the stat component or status effect component

	if (!StatComponent)
	{
		return;
	}

	// Example: Check for specific skills and apply their effects
	if (IsSkillUnlocked(TEXT("IronLungs")))
	{
		int32 SkillLevel = GetSkillLevel(TEXT("IronLungs"));
		float BonusRegen = 0.1f * SkillLevel; // +10% stamina regen per level

		// Apply bonus to stamina regeneration rate
		float CurrentRegen = StatComponent->GetStat(EStatType::Stamina).RegenerationRate;
		// Note: In a full implementation, you'd want to track base values
		// and apply modifiers properly to avoid stacking issues
	}
}
