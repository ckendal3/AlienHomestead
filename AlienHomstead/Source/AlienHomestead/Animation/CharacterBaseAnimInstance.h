// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALIENHOMESTEAD_API UCharacterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UCharacterBaseAnimInstance();

public:

	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	class ACharacterBase* GetOwningCharacter() const;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	FRotator GetLookDirection() const { return LookDirection; }

	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	FVector GetLookDirectionAsVector() const { return LookDirection.Vector(); }

	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	float GetLookToMovementDirectionalFloat() const;

	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	FVector GetVelocity() const { return Velocity; }
	
	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	float GetSpeed() const { return Velocity.Size(); }
	
	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	float GetSpeed2D() const { return Velocity.Size2D(); }
	
	UFUNCTION(BlueprintPure, Meta = (BlueprintThreadSafe))
	float GetSpeedFalling() const { return Velocity.Z; }

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Core")
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, Category = "Core")
	FRotator LookDirection;
};