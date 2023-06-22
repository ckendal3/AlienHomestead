// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Breakable.generated.h"

UCLASS(Abstract)
class ALIENHOMESTEAD_API ABreakable : public AActor
{
	GENERATED_BODY()

public:
	ABreakable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ActorToSpawnOnDeath = nullptr;

	UFUNCTION()
	void OnDeath(AActor* KilledActor);
};
