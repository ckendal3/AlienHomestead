// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseDefenseGameMode.generated.h"

class UWaveConfig;
class URoundConfig;
class UWaveManagerWorldSubsystem;

UCLASS()
class ALIENHOMESTEAD_API ABaseDefenseGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ABaseDefenseGameMode();

	virtual void PostInitializeComponents() override;
	virtual void StartMatch() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWaveConfig> TempConfig;

	UPROPERTY(BlueprintReadOnly)
	UWaveManagerWorldSubsystem* WaveSubsystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHordeInstance* HordeInstance;
	
	UFUNCTION()
	void OnWaveCompleted(UWaveConfig* CompletedWaveConfig);

	UFUNCTION()
	void OnRoundCompleted(URoundConfig* CompletedRoundConfig);
	
};
