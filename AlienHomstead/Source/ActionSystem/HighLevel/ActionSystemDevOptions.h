// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionSystemDevOptions.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "ActionSystem"))
class ACTIONSYSTEM_API UActionSystemDevOptions : public UDeveloperSettings
{
	GENERATED_BODY()
};
