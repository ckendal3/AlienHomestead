// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnZone.generated.h"

class UArrowComponent;
class USpawnGroupConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventOnActorSpawned, AActor*, SpawnedActor);

UCLASS()
class ALIENHOMESTEAD_API ASpawnZone : public AActor
{
	GENERATED_BODY()

public:

	ASpawnZone();

	UPROPERTY(EditAnywhere)
	int ChannelID = 0;

	UPROPERTY(BlueprintAssignable)
	FEventOnActorSpawned OnActorSpawned;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UShapeComponent* SpawnZoneComponent;

#if WITH_EDITOR
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UArrowComponent* ArrowComponent;
#endif
	
	TArray<AActor*> Spawn(USpawnGroupConfig* InConfig);
};
