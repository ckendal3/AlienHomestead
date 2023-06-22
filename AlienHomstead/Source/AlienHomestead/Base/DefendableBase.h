// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefendableBase.generated.h"

UCLASS()
class ALIENHOMESTEAD_API ADefendableBase : public AActor
{
	GENERATED_BODY()

public:
	
	ADefendableBase();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	/** Returns HealthComponent subobject **/
	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION()
	virtual void OnDeath(AActor* KilledActor);
	
	UFUNCTION()
	virtual void ReceiveHealthChangedEvent(float OldValue, float NewValue);
	
};
