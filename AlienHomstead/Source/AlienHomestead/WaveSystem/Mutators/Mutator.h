// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Mutator.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ALIENHOMESTEAD_API UMutator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ApplyMutation(AActor* InActor);
};
