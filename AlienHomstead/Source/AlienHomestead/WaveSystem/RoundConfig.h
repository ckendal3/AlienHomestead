// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mutators/MutatorConfig.h"
#include "UObject/Object.h"
#include "RoundConfig.generated.h"

UENUM(BlueprintType)
enum class ERoundEndType : uint8
{
	RET_Time,
	RET_AllDead,
	RET_TimeOrAllDead
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ALIENHOMESTEAD_API URoundConfig : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMutatorConfig> RoundMutatorConfig;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERoundEndType> RoundEndType = ERoundEndType::RET_AllDead;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "RoundEndType != ERoundEndType::RET_AllDead"))
	float RoundDuration = 60.f;

	UPROPERTY(EditAnywhere)
	float TimeBetweenSpawns = 2.5;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class USpawnGroupConfig>> SpawnGroupConfigs;
};
