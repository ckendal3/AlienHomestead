// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnZone.h"
#include "SpawnGroupConfig.h"
#include "AlienHomestead/Characters/CharacterBase.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASpawnZone::ASpawnZone()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpawnZoneComponent = CreateDefaultSubobject<UBoxComponent>("SpawnZone");
	RootComponent = SpawnZoneComponent;

#if WITH_EDITOR
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>("DirectionalArrow");
	ArrowComponent->SetupAttachment(SpawnZoneComponent);
#endif
}

TArray<AActor*> ASpawnZone::Spawn(USpawnGroupConfig* InConfig)
{
	TArray<AActor*> SpawnedActors;
	for(auto ActorSpawnData : InConfig->ActorSpawnMap)
	{
		for(int i = 0; i < ActorSpawnData.Value; i++)
		{
			const FVector  SpawnLoc = UKismetMathLibrary::RandomPointInBoundingBox(SpawnZoneComponent->GetComponentLocation(), SpawnZoneComponent->Bounds.BoxExtent);
			const FRotator SpawnRot = GetActorRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorSpawnData.Key, SpawnLoc, SpawnRot, SpawnParams);

			if(OnActorSpawned.IsBound())
			{
				OnActorSpawned.Broadcast(SpawnedActor);
			}

			SpawnedActors.Add(SpawnedActor);
		}
	}

	return SpawnedActors;
}