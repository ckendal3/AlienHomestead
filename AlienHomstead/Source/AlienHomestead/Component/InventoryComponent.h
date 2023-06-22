// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALIENHOMESTEAD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AWeapon>> DefaultWeaponArray;
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	TArray<AWeapon*> WeaponArray;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentWeapon)
	AWeapon* CurrentWeapon;

	UFUNCTION(BlueprintPure)
	bool CanAdd(TSubclassOf<AWeapon> WeaponClass) const;

	UFUNCTION(BlueprintCallable)
	bool AddItem(TSubclassOf<AWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(AWeapon* WeaponToEquip);
	
	void SwitchToItem(AWeapon* InWeapon);

	UFUNCTION(BlueprintCallable)
	void SwitchItems(int Direction);

	int32 FindCurrentWeaponIndex();

	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* PreviousWeapon);

	bool CanEquipWeapon(AWeapon* Weapon) const;
	
	AWeapon* CreateWeapon(TSubclassOf<class AWeapon> WeaponClass);

	void UnequipWeapon(AWeapon* WeaponToUnequip);
	void SetWeapon(class AWeapon* WeaponToSet);

	/** Returns Owning Character subobject **/
	FORCEINLINE class ACharacter* GetOwningCharacter() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Server, Reliable)
	virtual void Server_R_SetItem(AWeapon* WeaponToUse);

	UFUNCTION(Client, Reliable)
	virtual void Client_R_SwitchItem_Correction(AWeapon* WeaponToCorrectWith);

	void InitializeDefaultInventory(bool bIsOwner);
};