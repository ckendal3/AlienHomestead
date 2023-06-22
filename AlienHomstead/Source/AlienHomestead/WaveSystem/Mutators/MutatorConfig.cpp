// Fill out your copyright notice in the Description page of Project Settings.

#include "MutatorConfig.h"
#include "Kismet/KismetMathLibrary.h"

void UMutatorConfig::GetMutatorsToApply(TArray<TSubclassOf<UMutator>>& InArray)
{
	// Normally we would append but this makes sure that we only add unique mutators
	for(auto GuaranteedMutator: GuaranteedMutators)
	{
		InArray.AddUnique(GuaranteedMutator);
	}

	for(auto MutatorElement: RandomMutatorsMap)
	{
		const bool bShouldApply = UKismetMathLibrary::RandomBoolWithWeight(MutatorElement.Value);

		if(bShouldApply)
		{
			InArray.AddUnique(MutatorElement.Key);
		}
	}
}