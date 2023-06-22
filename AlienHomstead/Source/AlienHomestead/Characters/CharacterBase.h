// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class ALIENHOMESTEAD_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UActionComponent* ActionComponent;

protected:

	UFUNCTION()
	virtual void OnDeath(AActor* KilledActor);
	
	UFUNCTION(Reliable, NetMulticast)
	void Multicast_R_CharacterDied();

	UFUNCTION()
	virtual void ReceiveHealthChangedEvent(float OldValue, float NewValue);

	virtual void CreateRagdoll();
	
public:
	
	ACharacterBase();

	/** Returns HealthComponent subobject **/
	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	/** Returns ActionComponent subobject **/
	FORCEINLINE class UActionComponent* GetActionComponent() const { return ActionComponent; }
};
