// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimeSystem/TimeComponent.h"
#include "Net/UnrealNetwork.h"

UTimeComponent::UTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bEnabled = true;
	TimeSpeedMultiplier = 1.0f;
	PreviousTimeOfDay = ETimeOfDay::Noon;
	PreviousHour = 12;
	PreviousDay = 1;

	// Enable replication
	SetIsReplicatedByDefault(true);
}

void UTimeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTimeComponent, CurrentTime);
	DOREPLIFETIME(UTimeComponent, bEnabled);
	DOREPLIFETIME(UTimeComponent, TimeSpeedMultiplier);
}

void UTimeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize tracking variables
	PreviousTimeOfDay = CurrentTime.GetTimeOfDay();
	PreviousHour = CurrentTime.Hour;
	PreviousDay = CurrentTime.Day;
}

void UTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnabled)
	{
		return;
	}

	// Only server updates time
	if (GetOwnerRole() == ROLE_Authority)
	{
		UpdateTime(DeltaTime);
	}

	// All clients check for events when time changes
	CheckTimeEvents();
}

void UTimeComponent::UpdateTime(float DeltaTime)
{
	if (!TimeSettings.bEnableDayNightCycle)
	{
		return;
	}

	// Apply time speed multiplier
	float AdjustedDelta = DeltaTime * TimeSpeedMultiplier;

	// Calculate how much game time passes
	// RealSecondsPerGameHour = how many real seconds for 1 game hour
	// So game seconds per real second = 3600 / RealSecondsPerGameHour
	float GameSecondsPerRealSecond = 3600.0f / TimeSettings.RealSecondsPerGameHour;
	float GameSecondsToAdd = AdjustedDelta * GameSecondsPerRealSecond;

	// Add to total
	CurrentTime.TotalSeconds += GameSecondsToAdd;
	CurrentTime.Second += GameSecondsToAdd;

	// Handle seconds overflow
	while (CurrentTime.Second >= 60.0f)
	{
		CurrentTime.Second -= 60.0f;
		CurrentTime.Minute++;
	}

	// Handle minutes overflow
	while (CurrentTime.Minute >= 60)
	{
		CurrentTime.Minute -= 60;
		CurrentTime.Hour++;
	}

	// Handle hours overflow
	while (CurrentTime.Hour >= 24)
	{
		CurrentTime.Hour -= 24;
		CurrentTime.Day++;
	}
}

void UTimeComponent::CheckTimeEvents()
{
	// Check for hour change
	if (CurrentTime.Hour != PreviousHour)
	{
		OnNewHour.Broadcast(CurrentTime.Hour);

		// Specific hour events
		if (CurrentTime.Hour == 0)
		{
			OnMidnight.Broadcast();
		}
		else if (CurrentTime.Hour == 6)
		{
			OnDawn.Broadcast();
		}
		else if (CurrentTime.Hour == 12)
		{
			OnNoon.Broadcast();
		}
		else if (CurrentTime.Hour == 18)
		{
			OnDusk.Broadcast();
		}

		PreviousHour = CurrentTime.Hour;
	}

	// Check for day change
	if (CurrentTime.Day != PreviousDay)
	{
		OnNewDay.Broadcast(CurrentTime.Day);

		// Check for season change
		if (TimeSettings.bEnableSeasons && TimeSettings.DaysPerSeason > 0)
		{
			int32 CurrentDayInCycle = (CurrentTime.Day - 1) % (TimeSettings.DaysPerSeason * 4);
			int32 PreviousDayInCycle = (PreviousDay - 1) % (TimeSettings.DaysPerSeason * 4);

			int32 CurrentSeasonIndex = CurrentDayInCycle / TimeSettings.DaysPerSeason;
			int32 PreviousSeasonIndex = PreviousDayInCycle / TimeSettings.DaysPerSeason;

			if (CurrentSeasonIndex != PreviousSeasonIndex)
			{
				OnSeasonChanged.Broadcast(GetCurrentSeason());
			}
		}

		PreviousDay = CurrentTime.Day;
	}

	// Check for time of day change
	ETimeOfDay CurrentTimeOfDay = CurrentTime.GetTimeOfDay();
	if (CurrentTimeOfDay != PreviousTimeOfDay)
	{
		OnTimeOfDayChanged.Broadcast(PreviousTimeOfDay, CurrentTimeOfDay);
		PreviousTimeOfDay = CurrentTimeOfDay;
	}
}

void UTimeComponent::SetTime(int32 Day, int32 Hour, int32 Minute)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	CurrentTime.Day = FMath::Max(1, Day);
	CurrentTime.Hour = FMath::Clamp(Hour, 0, 23);
	CurrentTime.Minute = FMath::Clamp(Minute, 0, 59);
	CurrentTime.Second = 0.0f;

	// Recalculate total seconds
	CurrentTime.TotalSeconds = ((CurrentTime.Day - 1) * 86400.0f) + (CurrentTime.Hour * 3600.0f) + (CurrentTime.Minute * 60.0f);

	UE_LOG(LogTemp, Log, TEXT("TimeComponent: Set time to %s"), *CurrentTime.ToString());
}

void UTimeComponent::AdvanceTimeByHours(float Hours)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	float SecondsToAdd = Hours * 3600.0f;
	CurrentTime.TotalSeconds += SecondsToAdd;
	CurrentTime.Second += SecondsToAdd;

	// Process overflows
	while (CurrentTime.Second >= 60.0f)
	{
		CurrentTime.Second -= 60.0f;
		CurrentTime.Minute++;
	}

	while (CurrentTime.Minute >= 60)
	{
		CurrentTime.Minute -= 60;
		CurrentTime.Hour++;
	}

	while (CurrentTime.Hour >= 24)
	{
		CurrentTime.Hour -= 24;
		CurrentTime.Day++;
	}

	UE_LOG(LogTemp, Log, TEXT("TimeComponent: Advanced time by %.2f hours"), Hours);
}

void UTimeComponent::AdvanceTimeByDays(int32 Days)
{
	AdvanceTimeByHours(Days * 24.0f);
}

void UTimeComponent::PauseTime()
{
	bEnabled = false;
	UE_LOG(LogTemp, Log, TEXT("TimeComponent: Time paused"));
}

void UTimeComponent::ResumeTime()
{
	bEnabled = true;
	UE_LOG(LogTemp, Log, TEXT("TimeComponent: Time resumed"));
}

void UTimeComponent::SetTimeSpeedMultiplier(float Multiplier)
{
	TimeSpeedMultiplier = FMath::Max(0.0f, Multiplier);
	UE_LOG(LogTemp, Log, TEXT("TimeComponent: Time speed set to %.2fx"), TimeSpeedMultiplier);
}

int32 UTimeComponent::GetCurrentDay() const
{
	return CurrentTime.Day;
}

int32 UTimeComponent::GetCurrentHour() const
{
	return CurrentTime.Hour;
}

int32 UTimeComponent::GetCurrentMinute() const
{
	return CurrentTime.Minute;
}

FString UTimeComponent::GetTimeString() const
{
	return CurrentTime.ToString();
}

FString UTimeComponent::GetClockTimeString() const
{
	return CurrentTime.GetTimeString();
}

ETimeOfDay UTimeComponent::GetTimeOfDayPeriod() const
{
	return CurrentTime.GetTimeOfDay();
}

ESeason UTimeComponent::GetCurrentSeason() const
{
	if (!TimeSettings.bEnableSeasons || TimeSettings.DaysPerSeason <= 0)
	{
		return ESeason::Spring;
	}

	// Calculate season based on day
	int32 DayInCycle = (CurrentTime.Day - 1) % (TimeSettings.DaysPerSeason * 4);
	int32 SeasonIndex = DayInCycle / TimeSettings.DaysPerSeason;

	return (ESeason)FMath::Clamp(SeasonIndex, 0, 3);
}

bool UTimeComponent::IsDaytime() const
{
	return CurrentTime.IsDaytime();
}

bool UTimeComponent::IsNighttime() const
{
	return CurrentTime.IsNighttime();
}

float UTimeComponent::GetDayProgress() const
{
	// Calculate progress through day (0 at midnight, 1 at next midnight)
	float TotalMinutesInDay = 24.0f * 60.0f;
	float CurrentMinuteOfDay = (CurrentTime.Hour * 60.0f) + CurrentTime.Minute + (CurrentTime.Second / 60.0f);
	return CurrentMinuteOfDay / TotalMinutesInDay;
}

float UTimeComponent::GetTotalElapsedHours() const
{
	return CurrentTime.GetTotalHours();
}

float UTimeComponent::GetTotalElapsedDays() const
{
	return CurrentTime.GetTotalDays();
}

void UTimeComponent::OnRep_GameTime()
{
	// Time was replicated, update tracking variables
	PreviousTimeOfDay = CurrentTime.GetTimeOfDay();
	PreviousHour = CurrentTime.Hour;
	PreviousDay = CurrentTime.Day;
}
