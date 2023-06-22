// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDefenseGameMode.h"
#include "AlienHomestead/WaveSystem/WaveConfig.h"
#include "AlienHomestead/WaveSystem/WaveManagerWorldSubsystem.h"

ABaseDefenseGameMode::ABaseDefenseGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseDefenseGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WaveSubsystem = GetWorld()->GetSubsystem<UWaveManagerWorldSubsystem>();
}

void ABaseDefenseGameMode::StartMatch()
{
	ensure(TempConfig);

	ensure(WaveSubsystem);

	if(WaveSubsystem)
	{
		HordeInstance = WaveSubsystem->RequestHordeInstance(UHordeInstance::StaticClass());

		HordeInstance->OnWaveCompleted.AddUniqueDynamic(this, &ABaseDefenseGameMode::OnWaveCompleted);
		HordeInstance->OnRoundCompleted.AddUniqueDynamic(this, &ABaseDefenseGameMode::OnRoundCompleted);
		
		UWaveConfig* NewWave = NewObject<UWaveConfig>(GetWorld(), TempConfig);
		
		HordeInstance->StartWave(NewWave);
	}
	
	Super::StartMatch();
}

void ABaseDefenseGameMode::OnWaveCompleted(UWaveConfig* CompletedWaveConfig)
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Completed."))
	
	//TODO: Proper wave ending
	HordeInstance->StartWave(CompletedWaveConfig);
}

void ABaseDefenseGameMode::OnRoundCompleted(URoundConfig* CompletedRoundConfig)
{
	UE_LOG(LogTemp, Warning, TEXT("Round Completed."))
}