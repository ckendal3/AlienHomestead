// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SpawnGroupConfig.generated.h"

UENUM(BlueprintType)
enum class ESpawnTokenCalculation : uint8
{
	ECalculateFromGroup,
	ECalculateUsingOverride
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ALIENHOMESTEAD_API USpawnGroupConfig : public UObject
{
	GENERATED_BODY()

public:
	// <ActorClassToSpawn, AmountToSpawn>
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AActor>, int> ActorSpawnMap;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESpawnTokenCalculation> TokenCalculation = ESpawnTokenCalculation::ECalculateFromGroup;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "TokenCalculation == ESpawnTokenCalculation::ECalculateUsingOverride"))
	int GroupTokenValue = 0;

	UFUNCTION(BlueprintPure)
	int GetTokenValue() const;

	void CalculateTokenValue();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
