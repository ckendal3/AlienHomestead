// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnGroupConfig.h"
#include "WaveSystemDevSettings.h"

int USpawnGroupConfig::GetTokenValue() const
{
	return GroupTokenValue;
}

void USpawnGroupConfig::CalculateTokenValue()
{
	// TODO: Define the token value rules in a uobject in developersettings? So it can be used easily across multiple games

	int TotalTokenValue = 0;
	
	if(const UWaveSystemDevSettings* WaveDevSettings = GetDefault<UWaveSystemDevSettings>())
	{
		for(auto& SpawnData : ActorSpawnMap)
		{
			if(WaveDevSettings->TokenValueMap.Contains(SpawnData.Key))
			{
				TotalTokenValue += (WaveDevSettings->TokenValueMap[SpawnData.Key] * SpawnData.Value);
			}
		}
	}

	GroupTokenValue = TotalTokenValue;
}

#if WITH_EDITOR
void USpawnGroupConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.GetPropertyName() : NAME_None;

	if(PropName == GET_MEMBER_NAME_CHECKED(USpawnGroupConfig, TokenCalculation) || PropName == GET_MEMBER_NAME_CHECKED(USpawnGroupConfig, ActorSpawnMap))
	{
		if(TokenCalculation == ESpawnTokenCalculation::ECalculateFromGroup)
		{
			CalculateTokenValue();
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif WITH_EDITOR