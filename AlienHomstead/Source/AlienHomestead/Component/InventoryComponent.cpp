// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "AlienHomestead/IsoShooterCharacter.h"
#include "AlienHomestead/Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

bool UInventoryComponent::CanAdd(TSubclassOf<AWeapon> WeaponClass) const
{
	if(WeaponArray.Num() > 0)
	{
		for(const auto Weapon : WeaponArray)
		{
			if(Weapon && Weapon->GetClass() == WeaponClass)
			{
				return false;
			}
		}
	}
	
	return true;
}

bool UInventoryComponent::AddItem(TSubclassOf<AWeapon> WeaponClass)
{
	if(CanAdd(WeaponClass))
	{
		if(AWeapon* NewWeapon = CreateWeapon(WeaponClass))
		{
			WeaponArray.Add(NewWeapon);

			return true;
		}
	}

	return false;
}

AWeapon* UInventoryComponent::CreateWeapon(TSubclassOf<AWeapon> WeaponClass)
{	
	AWeapon* CreatedWeapon = nullptr;
	
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetOwner()))
	{		
		FActorSpawnParameters Params;
		Params.Owner      = Char;
		Params.Instigator = Char;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		CreatedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, Char->GetActorLocation(), Char->GetActorForwardVector().Rotation(), Params);

		CreatedWeapon->SetActorHiddenInGame(true);
	}

	return CreatedWeapon;
}

void UInventoryComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	UnequipWeapon(CurrentWeapon);
	
	SetWeapon(WeaponToEquip);

	if(GetOwningCharacter()->IsLocallyControlled() && GetWorld()->IsServer() == false)
	{
		Server_R_SetItem(WeaponToEquip);
	}
}

void UInventoryComponent::UnequipWeapon(AWeapon* WeaponToUnequip)
{
	if(WeaponToUnequip)
	{
		WeaponToUnequip->NativeUnequipped();
		WeaponToUnequip->OnUnequipped();
	}
}

void UInventoryComponent::SetWeapon(AWeapon* WeaponToSet)
{
	ensure(WeaponToSet);
	
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetOwner()))
	{
		Char->SetWeapon(WeaponToSet);
		
		CurrentWeapon = WeaponToSet;
		CurrentWeapon->NativeEquipped();
		CurrentWeapon->OnEquipped();

		CurrentWeaponIndex = FindCurrentWeaponIndex();
	}
}

void UInventoryComponent::SwitchToItem(AWeapon* InWeapon)
{
	if(CanEquipWeapon(InWeapon))
	{
		EquipWeapon(InWeapon);
	}
}

void UInventoryComponent::SwitchItems(int Direction)
{
	if(WeaponArray.Num() > 0)
	{
		int NextIndex = CurrentWeaponIndex + Direction;
		
		if(NextIndex < 0)
		{
			NextIndex = WeaponArray.Num() -1;
		}
		else if(NextIndex >= WeaponArray.Num())
		{
			NextIndex = 0;
		}
		
		if(CanEquipWeapon(WeaponArray[NextIndex]))
		{
			EquipWeapon(WeaponArray[NextIndex]);
		}
	}
}

int32 UInventoryComponent::FindCurrentWeaponIndex()
{
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetOwner()))
	{
		if(WeaponArray.Num() > 0 && Char->GetWeapon())
		{
			int Index = 0;
			while(Index < WeaponArray.Num())
			{
				if(WeaponArray[Index] == Char->GetWeapon())
				{
					return Index;
				}
				
				Index++;
			}
		}
	}

	return 0;
}

void UInventoryComponent::OnRep_CurrentWeapon(AWeapon* PreviousWeapon)
{	
	// if we don't already have weapon equipped
	if(CurrentWeapon && PreviousWeapon != CurrentWeapon)
	{
		UnequipWeapon(PreviousWeapon);
		
		SetWeapon(CurrentWeapon);
	}
}

bool UInventoryComponent::CanEquipWeapon(AWeapon* Weapon) const
{
	return Weapon && (CurrentWeapon == nullptr || CurrentWeapon->GetClass() != Weapon->GetClass());
}

ACharacter* UInventoryComponent::GetOwningCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, CurrentWeapon, COND_SkipOwner);
	DOREPLIFETIME(UInventoryComponent, WeaponArray);
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(GetWorld()->IsServer())
	{
		for(auto Weapon : WeaponArray)
		{
			if(Weapon)
			{
				Weapon->Destroy();
			}
		}
	}
}

void UInventoryComponent::InitializeDefaultInventory(bool bIsOwner)
{
	if(DefaultWeaponArray.Num() > 0)
	{
		for(auto WeaponClass : DefaultWeaponArray)
		{
			AddItem(WeaponClass);
		}
		
		AWeapon* FirstWeapon = WeaponArray[0];
		if(CurrentWeapon == nullptr && CanEquipWeapon(FirstWeapon))
		{
			EquipWeapon(FirstWeapon);
		}
	}
}

void UInventoryComponent::Server_R_SetItem_Implementation(AWeapon* WeaponToUse)
{
	ensure(WeaponToUse);
	
	if(WeaponArray.Contains(WeaponToUse))
	{
		// if we don't already have weapon equipped
		if(CurrentWeapon && WeaponToUse != CurrentWeapon)
		{
			EquipWeapon(WeaponToUse);
		}
	}
	else
	{
		Client_R_SwitchItem_Correction(CurrentWeapon);
	}
}

void UInventoryComponent::Client_R_SwitchItem_Correction_Implementation(AWeapon* WeaponToCorrectWith)
{
	EquipWeapon(WeaponToCorrectWith);
}