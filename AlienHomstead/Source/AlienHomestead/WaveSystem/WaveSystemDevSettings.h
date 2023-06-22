// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WaveSystemDevSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Wave System Settings"))
class ALIENHOMESTEAD_API UWaveSystemDevSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category  = "Wave")
	TMap<TSubclassOf<class AActor>, int> TokenValueMap;
};
