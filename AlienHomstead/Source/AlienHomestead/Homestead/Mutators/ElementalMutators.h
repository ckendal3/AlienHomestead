// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AlienHomestead/Homestead/Types/HomesteadTypes.h"
#include "AlienHomestead/WaveSystem/Mutators/Mutator.h"
#include "ElementalMutators.generated.h"


UCLASS()
class ALIENHOMESTEAD_API UMutator_ElementalType : public UMutator
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "Elemental")
	TEnumAsByte<EElementalType> ElementalType = EElementalType::ET_None;

	virtual void ApplyMutation_Implementation(AActor* InActor) override;
};