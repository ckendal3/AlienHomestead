// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IsoPlayerController.generated.h"

class UUI_Map;

UCLASS()
class ALIENHOMESTEAD_API AIsoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUI_Map> MapWidget_Class;

	UPROPERTY(VisibleInstanceOnly)
	UUI_Map* MapWidget;

	AIsoPlayerController();

	virtual void SetupInputComponent() override;
	
	void Jump();
	void StopJumping();
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void SwitchItem();

	void StartSprint();
	void StopSprinting();

	void PrimaryItemActionStart();
	void PrimaryItemActionStop();

	void SecondaryItemActionStart();
	void SecondaryItemActionStop();
	
	virtual void StartFire(uint8 FireModeNum) override;

	/** Stop firing the player's currently selected weapon with the optional firemode. */
	UFUNCTION(exec)
	virtual void StopFire(uint8 FireModeNum = 0);

	void PrimaryAbilityActionStart();
	void PrimaryAbilityActionStop();

	void SecondaryAbilityActionStart();
	void SecondaryAbilityActionStop();

	void ToggleMap();

	UFUNCTION(BlueprintPure)
	class UUI_BaseHUD* GetUMGHUD() const;
};
