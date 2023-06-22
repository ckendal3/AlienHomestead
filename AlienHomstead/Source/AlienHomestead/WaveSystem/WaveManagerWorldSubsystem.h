// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WaveManagerWorldSubsystem.generated.h"

UENUM()
enum class ESpawningStatus : uint8 
{
	ESpawning,
	ENotSpawning
};

UENUM()
enum class EWaveStatus : uint8 
{
	WS_NotStarted,
	WS_InProgress,
	WS_Ending
};

class UMutator;
class UWaveConfig;
class URoundConfig;
class USpawnGroupConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventWaveCompleted,  UWaveConfig*,  WaveConfig);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventRoundCompleted, URoundConfig*, RoundConfig);

UCLASS(Blueprintable, BlueprintType)
class UHordeInstance : public UObject
{
	GENERATED_BODY()
	
public:

	UHordeInstance();

	// Incremented ID assigned from the WaveSystemManager
	UPROPERTY(VisibleAnywhere)
	int ID = 0;

	// Used to determine which channel is used. Currently only used for spawn zones
	UPROPERTY(VisibleAnywhere)
	int ChannelID;
	
	UPROPERTY(BlueprintReadOnly)
	class UWaveManagerWorldSubsystem* WorldWaveManager;
	
	UPROPERTY(BlueprintReadOnly)
	EWaveStatus     WaveStatus     = EWaveStatus::WS_NotStarted;

	UPROPERTY(VisibleAnywhere, Transient)
	int CurrentWaveCount = 0;

	UPROPERTY(EditAnywhere)
	UWaveConfig* WaveConfig;
	
	UPROPERTY(BlueprintReadOnly)
	ESpawningStatus SpawningStatus = ESpawningStatus::ENotSpawning;	

	UPROPERTY(BlueprintReadOnly)
	int CurrentInPlayTokenPool = 0;

	UPROPERTY(BlueprintReadOnly)
	int TotalTokenPool = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<class ASpawnZone*> SpawnZones;

	UPROPERTY(VisibleAnywhere)
	TArray<class AActor*> SpawnedActors;

	UPROPERTY(VisibleAnywhere)
	URoundConfig* CurrentRoundConfig;

	UPROPERTY(VisibleAnywhere)
	int CurrentRoundIndex = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<UMutator*> Mutators;

	UPROPERTY()
	FTimerHandle   TimerHandle_Spawner;
	FTimerDelegate TimerDelegate_Spawner;

	UPROPERTY()
	FTimerHandle   TimerHandle_NextRound;
	FTimerDelegate TimerDelegate_NextRound;

	UPROPERTY(BlueprintAssignable)
	FEventWaveCompleted OnWaveCompleted;

	UPROPERTY(BlueprintAssignable)
	FEventRoundCompleted OnRoundCompleted;
	
	void SetupSpawnZoneBindings();

	UFUNCTION()
	virtual void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
	void StartWave(UWaveConfig* InConfig);
	void SetupRound(int RoundIndex);
	void StartRound();
	void SetSpawningState(ESpawningStatus NewSpawnStatus);

	void EndWave();
	void EndRound();
	
	UFUNCTION()
	void OnActorSpawned(class AActor* SpawnedActor);

	UFUNCTION()
	void OnActorDeath(AActor* KilledActor);

	int GetTokenValue(class AActor* InActor) const;
	void IncreaseTokenPoolValue(int InValue);
	void DecreaseTokenPoolValue(int InValue);

	bool AllowedToSpawn() const;
	bool IsCurrentlySpawning() const;
	bool IsRoundOver() const;

	void ApplyMutatorsToSpawnedActor(AActor* InActor);
	void SetupMutators(UWaveConfig* InWaveConfig, URoundConfig* RoundConfig);
	void ClearMutators();

	UFUNCTION(BlueprintCallable)
	void SetChannelID(int NewChannelID);
	
	USpawnGroupConfig* GetValidSpawnGroupConfig() const;

	virtual void Tick(float DeltaTime);
	virtual void Cleanup();

	virtual void BeginDestroy() override;
};


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ALIENHOMESTEAD_API UWaveManagerWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	int NextID = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<UHordeInstance*> HordeInstances;

	virtual TStatId GetStatId() const override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UHordeInstance* RequestHordeInstance(TSubclassOf<UHordeInstance> ClassType);
	
	void UnregisterHordeInstance(UHordeInstance* Instance);
	
};
