// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UUI_BaseHUD;

UCLASS()
class ALIENHOMESTEAD_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:

	ABaseHUD();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUI_BaseHUD> HUDUMGWidgetClass;

	UPROPERTY()
	UUI_BaseHUD* UMGWidget;

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	UUI_BaseHUD* GetUMGHUD() const;
	
};
