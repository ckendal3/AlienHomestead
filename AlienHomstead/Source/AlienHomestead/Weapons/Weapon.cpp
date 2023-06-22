// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Component/ActionComponent.h"
#include "AlienHomestead/IsoShooterCharacter.h"

AWeapon::AWeapon()
{
	bReplicates           = true;
	bNetUseOwnerRelevancy = true;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SetCanBeDamaged(false);

	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComp"));
}

void AWeapon::NativeEquipped()
{
	SetActorHiddenInGame(false);
	
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetOwner()))
	{
		USkeletalMeshComponent* Mesh = Char->GetMesh();
		AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

void AWeapon::NativeUnequipped()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	SetActorHiddenInGame(true);

	ActionComponent->StopAllActions(this);
}

void AWeapon::StartAction(int FireMode)
{
	if(FireMode == 0)
	{
		PrimaryAction(true);
	}
	else if(FireMode == 1)
	{
		SecondaryAction(true);
	}
}

void AWeapon::StopAction(int FireMode)
{
	if(FireMode == 0)
	{
		PrimaryAction(false);
	}
	else if(FireMode == 1)
	{
		SecondaryAction(true);
	}
}

void AWeapon::PrimaryAction_Implementation(bool bStart)
{
	if(bStart)
	{
		ActionComponent->StartAction(PrimaryActionName, this);
	}
	else
	{
		ActionComponent->StopAction(PrimaryActionName, this);
	}
}

void AWeapon::SecondaryAction_Implementation(bool bStart)
{
	if(bStart)
	{
		ActionComponent->StartAction(SecondaryActionName, this);
	}
	else
	{
		ActionComponent->StopAction(SecondaryActionName, this);
	}
}

bool AWeapon::IsLocallyControlled() const
{
	const AActor* ActorToCheck = GetNetOwner();
	
	const ENetMode NetMode = ActorToCheck->GetNetMode();

	if (NetMode == NM_Standalone)
	{
		// Not networked.
		return true;
	}
	
	if (NetMode == NM_Client && ActorToCheck->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Networked client in control.
		return true;
	}

	if (ActorToCheck->GetRemoteRole() != ROLE_AutonomousProxy && ActorToCheck->GetLocalRole() == ROLE_Authority)
	{
		// Local authority in control.
		return true;
	}

	return false;
}