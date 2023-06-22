// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldBounds.generated.h"

// Used to determine the world bounds which is used for the UI map generation and calibration. 
UCLASS(BlueprintType, Blueprintable)
class ALIENHOMESTEAD_API AWorldBounds : public AActor
{
	GENERATED_BODY()

public:
	
	AWorldBounds();

	/** Bounding box for the world bounds. */
	UPROPERTY(EditAnywhere, Category = WorldBounds)
	class UBoxComponent* BoxComponent;

	UFUNCTION(BlueprintPure)
	void GetActorBoxAndOrigin(FBox& BoxBounds, FVector& Origin);
};
