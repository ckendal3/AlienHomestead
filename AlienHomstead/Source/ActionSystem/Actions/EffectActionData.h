// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionData.h"
#include "EffectActionData.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONSYSTEM_API UEffectActionData : public UActionData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Period = 1.f;

	FTimerHandle TimerHandle_Duration;
	FTimerHandle TimerHandle_Period;
	
	FTimerDelegate Delegate_Duration;
	FTimerDelegate Delegate_Period;
	
	virtual void StartAction(AActor* Instigator) override;
	virtual void EndAction(AActor* Instigator) override;

	UFUNCTION(BlueprintNativeEvent)
	void PeriodicEvent(AActor* Instigator);
};