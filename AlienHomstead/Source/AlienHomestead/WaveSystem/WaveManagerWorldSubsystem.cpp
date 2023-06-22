// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManagerWorldSubsystem.h"
#include "RoundConfig.h"
#include "SpawnGroupConfig.h"
#include "SpawnZone.h"
#include "WaveConfig.h"
#include "WaveSystemDevSettings.h"
#include "AlienHomestead/Characters/CharacterBase.h"
#include "AlienHomestead/Component/HealthComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

UHordeInstance::UHordeInstance()
{
	TimerDelegate_Spawner.BindUObject(this, &UHordeInstance::SpawnEnemy);
	TimerDelegate_NextRound.BindUObject(this, &UHordeInstance::StartRound);
}

void UHordeInstance::StartWave(UWaveConfig* InConfig)
{	
	WaveConfig = InConfig;
	WaveStatus = EWaveStatus::WS_NotStarted;

	CurrentWaveCount += 1;
	
	TArray<AActor*> OutActors;

	SetupSpawnZoneBindings();

	CurrentRoundIndex = 0;
	SetupRound(CurrentRoundIndex);
}

void UHordeInstance::SetupRound(int RoundIndex)
{
	if(RoundIndex >= WaveConfig->RoundConfigs.Num())
	{
		// TODO: Handle all rounds finished
		return;
	}
	
	WaveStatus = EWaveStatus::WS_InProgress;
	
	CurrentRoundConfig = Cast<URoundConfig>(WaveConfig->RoundConfigs[RoundIndex]->GetDefaultObject(true));
	CurrentRoundIndex  = RoundIndex;
	
	TotalTokenPool = 0;
	CurrentInPlayTokenPool = 0;

	SetupMutators(WaveConfig, CurrentRoundConfig);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_NextRound, TimerDelegate_NextRound, WaveConfig->TimeBetweenRounds, false, WaveConfig->TimeBetweenRounds);
}

void UHordeInstance::StartRound()
{
	SetSpawningState(ESpawningStatus::ESpawning);
}

void UHordeInstance::SetSpawningState(ESpawningStatus NewSpawnStatus)
{
	if(NewSpawnStatus != SpawningStatus)
	{
		SpawningStatus = NewSpawnStatus;
		
		switch(SpawningStatus)
		{
			case ESpawningStatus::ESpawning:
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_Spawner, TimerDelegate_Spawner, CurrentRoundConfig->TimeBetweenSpawns, true, 0.f);
				break;
			case ESpawningStatus::ENotSpawning:
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Spawner);
				break;
		}
	}
}

void UHordeInstance::EndWave()
{
	WaveStatus = EWaveStatus::WS_Ending;

	if(OnWaveCompleted.IsBound())
	{
		OnWaveCompleted.Broadcast(WaveConfig);
	}
}

void UHordeInstance::EndRound()
{
	if(OnRoundCompleted.IsBound())
	{
		OnRoundCompleted.Broadcast(CurrentRoundConfig);
	}
	
	SetSpawningState(ESpawningStatus::ENotSpawning);

	const int NewRoundIndex = CurrentRoundIndex + 1;
	
	if(NewRoundIndex >= WaveConfig->RoundConfigs.Num())
	{
		EndWave();
		return;
	}

	SetupRound(NewRoundIndex);
}

void UHordeInstance::SetupSpawnZoneBindings()
{	
	// Setup bindings to new zones
	TArray<AActor*> OutActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnZone::StaticClass(), OutActors);

	for(AActor* FoundActor : OutActors)
	{
		if(FoundActor->IsA(ASpawnZone::StaticClass()))
		{
			ASpawnZone* Zone = CastChecked<ASpawnZone>(FoundActor);

			if(Zone && Zone->ChannelID == ChannelID)
			{
				Zone->OnActorSpawned.AddUniqueDynamic(this, &UHordeInstance::OnActorSpawned);
				SpawnZones.Add(Zone);
			}
		}
	}
}

void UHordeInstance::SpawnEnemy()
{
	const int Index = FMath::RandRange(0, FMath::Max(0, SpawnZones.Num() - 1));

	// TODO: Determine enemies that can be spawned and the rules
	// example, can setup a Hunter pair from Halo to spawn 2 Hunters only
	// spawn a squad such as 1 elite, 3 grunts, 1 jackal

	if(USpawnGroupConfig* SpawnGroupConfig = GetValidSpawnGroupConfig())
	{
		TArray<AActor*> GroupSpawnedList = SpawnZones[Index]->Spawn(SpawnGroupConfig);

		for(auto& SpawnedActor : SpawnedActors)
		{
			ApplyMutatorsToSpawnedActor(SpawnedActor);
		}

		SpawnedActors.Append(GroupSpawnedList);
	}
}

void UHordeInstance::OnActorSpawned(class AActor* SpawnedActor)
{
	const int TokenValueToAdd = GetTokenValue(SpawnedActor);

	IncreaseTokenPoolValue(TokenValueToAdd);

	if(UHealthComponent* HealthComponent = SpawnedActor->FindComponentByClass<UHealthComponent>())
	{
		HealthComponent->OnDeath.AddUniqueDynamic(this, &UHordeInstance::OnActorDeath);
	}
}

void UHordeInstance::OnActorDeath(AActor* KilledActor)
{
	// We already know its a character because we only bind above
	ACharacterBase* KilledCharacter = CastChecked<ACharacterBase>(KilledActor);

	const int TokenValueToAdd = GetTokenValue(KilledCharacter);

	DecreaseTokenPoolValue(TokenValueToAdd);
	
	// Why is the killed actor not in the list???
	ensure(SpawnedActors.Contains(KilledActor));
	
	if(SpawnedActors.Contains(KilledActor))
	{
		SpawnedActors.Remove(KilledActor);
	}
	
	if(IsRoundOver())
	{
		WaveStatus = EWaveStatus::WS_Ending;
		
		EndRound();
	}
}

int UHordeInstance::GetTokenValue(AActor* InActor) const
{
	// TODO: Define the token value rules in a uobject in developersettings? So it can be used easily across multiple games?
	
	if(const UWaveSystemDevSettings* WaveDevSettings = GetDefault<UWaveSystemDevSettings>())
	{
		if(WaveDevSettings->TokenValueMap.Contains(InActor->GetClass()))
		{
			return WaveDevSettings->TokenValueMap[InActor->GetClass()];
		}
	}

	return 0;
}

void UHordeInstance::IncreaseTokenPoolValue(int InValue)
{
	CurrentInPlayTokenPool += InValue;
	TotalTokenPool += InValue;

	// We've reached our limit
	if(IsCurrentlySpawning() && AllowedToSpawn() == false)
	{
		SetSpawningState(ESpawningStatus::ENotSpawning);
	}
	
	ensure(CurrentInPlayTokenPool <= WaveConfig->MaxInPlayTokenPool);
}

void UHordeInstance::DecreaseTokenPoolValue(int InValue)
{
	CurrentInPlayTokenPool -= InValue;

	if(IsCurrentlySpawning() == false)
	{
		if(AllowedToSpawn() && IsRoundOver() == false)
		{
			SetSpawningState(ESpawningStatus::ESpawning);
		}
	}

	ensure(CurrentInPlayTokenPool >= 0);
}

TStatId UWaveManagerWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UWaveManagerWorldSubsystem, STATGROUP_Tickables);
}

void UWaveManagerWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for(UHordeInstance* Instance : HordeInstances)
	{
		Instance->Tick(DeltaTime);
	}
}

UHordeInstance* UWaveManagerWorldSubsystem::RequestHordeInstance(TSubclassOf<UHordeInstance> ClassType)
{
	UHordeInstance* Instance = NewObject<UHordeInstance>(this, ClassType);
	Instance->WorldWaveManager = this;
	Instance->ID = NextID;

	NextID++;

	HordeInstances.Add(Instance);
	
	return Instance;
}

void UWaveManagerWorldSubsystem::UnregisterHordeInstance(UHordeInstance* Instance)
{
	// Why is this not properly registered?
	ensure(HordeInstances.Contains(Instance));
	
	if(HordeInstances.Contains(Instance))
	{
		HordeInstances.Remove(Instance);
	}

	// Why is this instance not valid?
	ensure(Instance);
	
	if(IsValid(Instance))
	{
		Instance->Cleanup();
		Instance->MarkPendingKill();
	}
}

void UHordeInstance::Tick(float DeltaTime)
{
#if WITH_EDITOR
	if(GEngine && WaveConfig)
	{		
		GEngine->AddOnScreenDebugMessage(7000 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("TotalTokenPool:         %d"), TotalTokenPool));
		GEngine->AddOnScreenDebugMessage(7001 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("TotalTokenPoolForWave:  %d"), WaveConfig->TotalTokenPoolForWave));
		
		GEngine->AddOnScreenDebugMessage(7002 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentInPlayTokenPool: %d"), CurrentInPlayTokenPool));
		GEngine->AddOnScreenDebugMessage(7003 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("MaxInPlayTokenPool:     %d"), WaveConfig->MaxInPlayTokenPool));

		GEngine->AddOnScreenDebugMessage(7004 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentRound:           %d"), (CurrentRoundIndex + 1)));
		GEngine->AddOnScreenDebugMessage(7005 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentRoundIndex:      %d"), CurrentRoundIndex));
		
		GEngine->AddOnScreenDebugMessage(7006 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("IsCurrentlySpawning:    %s"), IsCurrentlySpawning() ? *FString("true") : *FString("false")));

		float TimeTillNextRoundStarts = 0.f;
		if(GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_NextRound))
		{
			TimeTillNextRoundStarts = GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_NextRound);
		}
		
		GEngine->AddOnScreenDebugMessage(7007 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("TimeTillRoundStart:     %f"), TimeTillNextRoundStarts));
		GEngine->AddOnScreenDebugMessage(7008 + 8 * ID, 15.0f, FColor::Yellow, FString::Printf(TEXT("CurrentWave:            %d"), (CurrentWaveCount)));
	}
#endif
}

void UHordeInstance::Cleanup()
{
	for(AActor* Actor : SpawnedActors)
	{
		Actor->Destroy();
	}

	SpawnedActors.Empty();
}

void UHordeInstance::BeginDestroy()
{
	Cleanup();
	
	UObject::BeginDestroy();
}

bool UHordeInstance::AllowedToSpawn() const
{
	return TotalTokenPool < WaveConfig->TotalTokenPoolForWave && CurrentInPlayTokenPool < WaveConfig->MaxInPlayTokenPool;
}

bool UHordeInstance::IsCurrentlySpawning() const
{
	return SpawningStatus == ESpawningStatus::ESpawning;
}

bool UHordeInstance::IsRoundOver() const
{
	const bool bAllTokensConsumed = CurrentInPlayTokenPool == 0 && TotalTokenPool == WaveConfig->TotalTokenPoolForWave;
	
	return bAllTokensConsumed;
}

void UHordeInstance::ApplyMutatorsToSpawnedActor(AActor* InActor)
{
	for(auto& Mutator : Mutators)
	{
		Mutator->ApplyMutation(InActor);
	}
}

void UHordeInstance::SetupMutators(UWaveConfig* InWaveConfig, URoundConfig* RoundConfig)
{
	ClearMutators();

	TArray<TSubclassOf<UMutator>> MutatorClasses;

	// TODO: GUARANTEE WAVEMUTATORCONFIG AND ROUNDMUTATORCONFIG ARE NEVER NULL
	
	if(InWaveConfig->WaveMutatorConfig)
	{
		UMutatorConfig* WaveMutatorConfig = Cast<UMutatorConfig>(InWaveConfig->WaveMutatorConfig->GetDefaultObject());
		if(WaveMutatorConfig)
		{
			WaveMutatorConfig->GetMutatorsToApply(MutatorClasses);
		}
	}
	
	if(RoundConfig->RoundMutatorConfig)
	{
		UMutatorConfig* RoundMutatorConfig = Cast<UMutatorConfig>(RoundConfig->RoundMutatorConfig->GetDefaultObject());
		if(RoundMutatorConfig)
		{
			RoundMutatorConfig->GetMutatorsToApply(MutatorClasses);
		}
	}
	
	for(auto MutatorClass : MutatorClasses)
	{
		// These will be garbage collected when we clear mutators.
		// Future optimization could be to go through the existing mutators and keep the
		// mutators that will be recreated but its not really important right now.
		Mutators.Add(NewObject<UMutator>(GetWorld(), MutatorClass));
	}
}

void UHordeInstance::ClearMutators()
{
	Mutators.Empty();
}

void UHordeInstance::SetChannelID(int NewChannelID)
{
	// TODO: Add initialiation tracking and consolidate spawnzone binding logic
	if(ChannelID == NewChannelID)
	{
		return;
	}

	const int OldChannelID = ChannelID;

	ChannelID = NewChannelID;

	// Clear old spawnzone bindings
	for(ASpawnZone* Zone : SpawnZones)
	{
		if(Zone && Zone->ChannelID == OldChannelID)
		{
			Zone->OnActorSpawned.RemoveDynamic(this, &UHordeInstance::OnActorSpawned);
		}
	}

	SpawnZones.Empty();

	SetupSpawnZoneBindings();
}

USpawnGroupConfig* UHordeInstance::GetValidSpawnGroupConfig() const
{
	if(IsCurrentlySpawning())
	{
		TArray<USpawnGroupConfig*> ValidSpawnGroups;
		
		for(const auto& SpawnGroupClass : CurrentRoundConfig->SpawnGroupConfigs)
		{
			USpawnGroupConfig* GroupSpawnData = Cast<USpawnGroupConfig>(SpawnGroupClass->GetDefaultObject(true));

			const int TokenValue = GroupSpawnData->GetTokenValue();

			const bool bValidInPlayAllocation    = CurrentInPlayTokenPool + TokenValue <= WaveConfig->MaxInPlayTokenPool;
			const bool bValidIMaxTokenAllocation = TotalTokenPool + TokenValue <= WaveConfig->TotalTokenPoolForWave;
			
			// return a valid config as we found one we can spawn
			if(bValidInPlayAllocation && bValidIMaxTokenAllocation)
			{
				ValidSpawnGroups.AddUnique(GroupSpawnData);
			}
		}

		if(ValidSpawnGroups.Num() > 0)
		{
			const int RandIndex = FMath::RandRange(0, ValidSpawnGroups.Num() - 1);
			return ValidSpawnGroups[RandIndex];
		}
	}

	return nullptr;
}
