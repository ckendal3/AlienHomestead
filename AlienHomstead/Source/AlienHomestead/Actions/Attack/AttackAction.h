// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/ActionData.h"
#include "AttackAction.generated.h"

UENUM(BlueprintType)
enum class ELocationType : uint8 
{
	EOwnerLocation        = 0,
	EOwnerSocketLocation  = 1,
	ECustom               = 2
};

/**
 * 
 */
UCLASS()
class ALIENHOMESTEAD_API UAttackAction : public UActionData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	ELocationType StartLocationType = ELocationType::EOwnerLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Attack", meta=(EditCondition="StartLocationType == ELocationType::EOwnerSocketLocation", EditConditionHides))
	FName StartSocketName;
	
	UFUNCTION(BlueprintPure)
	FVector GetStartLocation() const;

	UFUNCTION(BlueprintNativeEvent)
	FVector GetStartLocation_Custom() const;
};
