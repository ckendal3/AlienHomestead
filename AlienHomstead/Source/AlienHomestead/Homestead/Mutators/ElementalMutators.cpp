// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementalMutators.h"
#include "AlienHomestead/Homestead/AI/SpiderMinion.h"

void UMutator_ElementalType::ApplyMutation_Implementation(AActor* InActor)
{
	if(ASpiderMinion* SpiderMinion = Cast<ASpiderMinion>(InActor))
	{
		SpiderMinion->SetElementalType(ElementalType);
	}
}