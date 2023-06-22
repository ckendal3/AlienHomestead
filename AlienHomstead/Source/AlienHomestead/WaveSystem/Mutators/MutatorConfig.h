// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "UObject/Object.h"
#include "MutatorConfig.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ALIENHOMESTEAD_API UMutatorConfig : public UObject
{
	GENERATED_BODY()

	// Mutators that are guaranteed to be applied
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UMutator>> GuaranteedMutators;

	// Mutators that are have a chance to be applied
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<UMutator>, float> RandomMutatorsMap;

public:
	void GetMutatorsToApply(TArray<TSubclassOf<UMutator>>& InArray);
};
