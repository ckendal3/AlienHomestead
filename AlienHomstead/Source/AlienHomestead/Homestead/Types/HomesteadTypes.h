// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HomesteadTypes.generated.h"

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	ET_Corrosive,
	ET_Electrical,
	ET_Fire,
	ET_None
};