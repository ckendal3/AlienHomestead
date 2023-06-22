// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Component/ActionComponent.h"
#include "AlienHomestead/IsoShooterCharacter.h"
#include "AlienHomestead/Actors/Ragdoll.h"
#include "AlienHomestead/Component/HealthComponent.h"
#include "Kismet/GameplayStatics.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComponent->OnDeath.AddUniqueDynamic(this, &ACharacterBase::OnDeath);
	HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &ACharacterBase::ReceiveHealthChangedEvent);

	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComp"));
}

void ACharacterBase::OnDeath(AActor* KilledActor)
{
	Multicast_R_CharacterDied();
}

void ACharacterBase::Multicast_R_CharacterDied_Implementation()
{
	CreateRagdoll();

	GetActionComponent()->StopAllActions(this);
	
	if(GetWorld()->IsServer())
	{
		Destroy();
	}
}

void ACharacterBase::ReceiveHealthChangedEvent(float OldValue, float NewValue) { }

void ACharacterBase::CreateRagdoll()
{
	const FTransform SpawnTransform = GetMesh()->GetComponentTransform();

	ARagdoll* Ragdoll = GetWorld()->SpawnActorDeferred<ARagdoll>(ARagdoll::StaticClass(), SpawnTransform, this, this);
	
	Ragdoll->SetMesh(GetMesh()->SkeletalMesh, GetMesh()->GetMaterial(0));

	UGameplayStatics::FinishSpawningActor(Ragdoll, SpawnTransform);
}