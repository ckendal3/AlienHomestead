// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldTextActor.generated.h"

UCLASS()
class ALIENHOMESTEAD_API AWorldTextActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MainComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* TextWidgetComponent;
	
public:

	AWorldTextActor();
	
	UFUNCTION(BlueprintCallable)
	void SetText(FText& InText);

	UFUNCTION(BlueprintPure)
	FText GetText() const;
};
