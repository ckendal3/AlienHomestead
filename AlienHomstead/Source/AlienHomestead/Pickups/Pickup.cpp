// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "AlienHomestead/IsoShooterCharacter.h"
#include "AlienHomestead/Component/InventoryComponent.h"
#include "AlienHomestead/Weapons/Weapon.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;
		
	SetCanBeDamaged(false);

	OverlapSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComponent"));
	OverlapSphereComponent->SetSphereRadius(64.f);

	RootComponent = OverlapSphereComponent;
}

void APickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(WeaponToAdd == nullptr)
	{
		return;
	}
	
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(OtherActor))
	{
		if(UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(Char->GetComponentByClass(UInventoryComponent::StaticClass())))
		{
			if(InventoryComp->AddItem(WeaponToAdd))
			{
				Destroy();
			}
		}
	}
}