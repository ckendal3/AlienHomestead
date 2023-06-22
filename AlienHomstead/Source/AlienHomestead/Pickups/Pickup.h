// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class USphereComponent;

UCLASS()
class ALIENHOMESTEAD_API APickup : public AActor
{
	GENERATED_BODY()

public:

	APickup();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* OverlapSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AWeapon> WeaponToAdd;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
