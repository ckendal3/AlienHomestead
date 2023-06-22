// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "UI_FloatingDamage.generated.h"

/**
 * 
 */
UCLASS()
class ALIENHOMESTEAD_API UUI_FloatingDamage : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DamageText;
	
	void SetText(FText& InText);

	void NativeSetText(FText& InText);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetText(const FText& InText);
};
