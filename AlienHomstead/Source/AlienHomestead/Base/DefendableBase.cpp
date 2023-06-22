// Fill out your copyright notice in the Description page of Project Settings.


#include "DefendableBase.h"
#include "AlienHomestead/Component/HealthComponent.h"

ADefendableBase::ADefendableBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComponent->OnDeath.AddUniqueDynamic(this, &ADefendableBase::OnDeath);
	HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &ADefendableBase::ReceiveHealthChangedEvent);
}

void ADefendableBase::OnDeath(AActor* KilledActor)
{
	if(GetWorld()->IsServer())
	{
		Destroy();
	}
}

void ADefendableBase::ReceiveHealthChangedEvent(float OldValue, float NewValue)
{
	
}