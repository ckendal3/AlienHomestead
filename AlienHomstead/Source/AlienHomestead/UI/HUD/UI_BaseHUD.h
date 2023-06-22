// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class ALIENHOMESTEAD_API UUI_BaseHUD : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY()
	AActor* DefendingActor = nullptr;
	
	float MaxHealth = 100.f;
	float CurHealth = 100.f;
	
	UFUNCTION()
	void OnDefensableActorDamaged(float OldValue, float NewValue);

	UFUNCTION()
	void OnDefensableActorChanged(AActor* InActor);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDefensableActorDamaged_BP(float OldValue, float NewValue);
};
