// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UActionableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONSYSTEM_API IActionableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PerformAction(int ActionID = 0);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopAction(int ActionID = 0);
};
