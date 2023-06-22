// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienHomestead/Homestead/World/WorldBounds.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "UI_Map.generated.h"

class UOverlay;
class UButton;

/**
 * 
 */
UCLASS()
class ALIENHOMESTEAD_API UUI_Map : public UUserWidget
{
	GENERATED_BODY()

public:

	void ClearMarkers();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY()
	FTimerHandle TimerHandle_UpdateMap;
	FTimerDelegate TimerDelegate_UpdateMap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Action1Class;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Action2Class;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> ActorToSpawnOnClick;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> ActorsToDisplay;

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AActor>, TSubclassOf<UUserWidget>> MarkerMap;

	UPROPERTY(BlueprintReadOnly)
	AWorldBounds* WorldBounds;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* BoundsWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button_Map;

	UPROPERTY()
	TArray<UUserWidget*> Markers;
	
	UFUNCTION(BlueprintCallable)
	void DisplayActorsOnMap();

	UFUNCTION(BlueprintImplementableEvent)
	void PushLocationToScreen(UUserWidget* InMarker, FVector2D InPosPercentage);
	
	FVector2D GetDisplayLocation(const AActor* InActor) const;

	UPROPERTY(BlueprintReadOnly)
	FVector TopLeftWorldBoundsCoordinates;

	UPROPERTY(BlueprintReadOnly)
	FVector BottomRightWorldBoundsCoordinates;

	UPROPERTY(BlueprintReadOnly)
	FVector2D MaxDistances;

	UFUNCTION()
	void MapClicked();

	UFUNCTION()
	void UpdateMap();

	UFUNCTION(BlueprintCallable)
	void CloseMap();

	void CalculateReferenceCoordinates();

	UUserWidget* CreateMarker(const AActor* InActor);
	
	FVector2D GetScreenLocationFromPercentage(FVector2D InPercentages) const;

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	FVector GetNormalizeValueToWorldSpace(FVector2D NormalizedValue) const;

	bool GetNormalisedMousePositionInGeometry( UObject *WorldContextObject, FGeometry Geometry, FVector2D &Position );
};
