// Fill out your copyright notice in the Description page of Project Settings.

#include "SpiderMinion.h"
#include "Net/UnrealNetwork.h"

ASpiderMinion::ASpiderMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: Move map to a single spot (dev options?) as not every enemy needs this stored
	ElementalDataMap.Add(EElementalType::ET_None);
	ElementalDataMap.Add(EElementalType::ET_Corrosive);
	ElementalDataMap.Add(EElementalType::ET_Electrical);
	ElementalDataMap.Add(EElementalType::ET_Fire);
}

void ASpiderMinion::BeginPlay()
{
	Super::BeginPlay();

	SetElementalType(ElementalType);
}

void ASpiderMinion::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpiderMinion, ElementalType);
}

void ASpiderMinion::OnDeath(AActor* KilledActor)
{
	Super::OnDeath(KilledActor);

	if(GetWorld()->IsServer() && ActorSpawnedOnDeath != nullptr)
	{		
		GetWorld()->SpawnActor<AActor>(ActorSpawnedOnDeath, GetActorLocation(), FRotator::ZeroRotator);
	}
}

void ASpiderMinion::OnRep_ElementalType()
{
	SetElementalType(ElementalType);
}

void ASpiderMinion::SetElementalType(EElementalType NewElementalType)
{
	ElementalType = NewElementalType;
	
	const FElementalData& ElementalData = ElementalDataMap[NewElementalType];

	GetMesh()->SetMaterial(0, ElementalData.Material);

	ActorSpawnedOnDeath = ElementalData.ActorSpawnedOnDeath;
}