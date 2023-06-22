// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WaveConfig.generated.h"

class URoundConfig;
class UMutatorConfig;


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ALIENHOMESTEAD_API UWaveConfig : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMutatorConfig> WaveMutatorConfig;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<URoundConfig>> RoundConfigs;

	UPROPERTY(EditAnywhere)
	float TimeBetweenRounds = 10.f;
	
	UPROPERTY(EditAnywhere)
	int MaxInPlayTokenPool = 10;
	
	UPROPERTY(EditAnywhere)
	int TotalTokenPoolForWave = 10;
};
