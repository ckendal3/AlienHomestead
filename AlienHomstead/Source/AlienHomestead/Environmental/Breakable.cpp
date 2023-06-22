// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"
#include "AlienHomestead/Component/HealthComponent.h"

ABreakable::ABreakable()
{
	bReplicates = true;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComponent->OnDeath.AddUniqueDynamic(this, &ABreakable::OnDeath);
}

void ABreakable::OnDeath(AActor* KilledActor)
{
	if(ActorToSpawnOnDeath && GetWorld()->IsServer())
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		GetWorld()->SpawnActor<AActor>(ActorToSpawnOnDeath, GetActorLocation(), GetActorRotation(), Params);
	}
	
	Destroy();
}