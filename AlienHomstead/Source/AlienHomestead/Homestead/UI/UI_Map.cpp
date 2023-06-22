// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Map.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"


void UUI_Map::NativeConstruct()
{
	Super::NativeConstruct();
	
	bIsFocusable = true;
	SetFocus();
	SetKeyboardFocus();

	CalculateReferenceCoordinates();

	TimerDelegate_UpdateMap.BindUObject(this, &UUI_Map::UpdateMap);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateMap, TimerDelegate_UpdateMap, .016f, true);

	Button_Map->OnPressed.AddUniqueDynamic(this, &UUI_Map::MapClicked);
}

void UUI_Map::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_UpdateMap);
}

FReply UUI_Map::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == EKeys::One)
	{
		ActorToSpawnOnClick = Action1Class;
	}
	else if(InKeyEvent.GetKey() == EKeys::Two)
	{
		ActorToSpawnOnClick = Action2Class;
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUI_Map::MapClicked()
{
	FVector2D NormalizedValue = FVector2D::ZeroVector;
	
	if(GetNormalisedMousePositionInGeometry( this, BoundsWidget->GetCachedGeometry(), NormalizedValue ))
	{
		// WE HAVE TO COMPENSATE FOR Y-axis in 3D coordinates not being the same orientation as the Y-Axis in 2D coordinate system
		// so we flip flop the X, Y values
		FVector2D WorldSpaceNormalized(NormalizedValue.Y, NormalizedValue.X);
		WorldSpaceNormalized.X = 1 - WorldSpaceNormalized.X;
		
		FVector WorldPos = GetNormalizeValueToWorldSpace(WorldSpaceNormalized);
		WorldPos.Z = 5000.f;

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, WorldPos, WorldPos + FVector(0.f, 0.f, -6000.f), ECollisionChannel::ECC_Visibility);

		DrawDebugSphere(GetWorld(), HitResult.Location, 64.f, 16, FColor::Green, false, 30.f, 8, 16.f);

		if(ActorToSpawnOnClick)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = GetOwningPlayer()->AcknowledgedPawn;
			SpawnParams.Owner = GetOwningPlayer();
			
			GetWorld()->SpawnActor<AActor>(ActorToSpawnOnClick, HitResult.Location, FRotator::ZeroRotator, SpawnParams);
		}
	}
}

void UUI_Map::UpdateMap()
{
	ClearMarkers();

	DisplayActorsOnMap();
}

void UUI_Map::CloseMap()
{
	RemoveFromViewport();

	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	GetOwningPlayer()->bShowMouseCursor = false;
}

void UUI_Map::ClearMarkers()
{
	for(auto ExistingMarker : Markers)
	{
		ExistingMarker->RemoveFromParent();
	}

	Markers.Empty();
}

void UUI_Map::DisplayActorsOnMap()
{
	for(const auto ClassType : ActorsToDisplay)
	{
		TArray<AActor*> ActorsFound;
		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassType, ActorsFound);

		for(const AActor* ActorFound : ActorsFound)
		{
			if(UUserWidget* NewMarker = CreateMarker(ActorFound))
			{
				Markers.Add(NewMarker);
				BoundsWidget->AddChildToOverlay(NewMarker);
			
				const FVector2D ScreenPosition = GetDisplayLocation(ActorFound);

				PushLocationToScreen(NewMarker, ScreenPosition);
			}
		}
	}
}

FVector2D UUI_Map::GetDisplayLocation(const AActor* InActor) const
{
	FVector2D ScreenPos = FVector2D::ZeroVector;
	if(WorldBounds)
	{
		// Get distance between (left bound to actor location) and (top bound to actor location)
		const float ActorHorizDist = FMath::Abs(TopLeftWorldBoundsCoordinates.X + InActor->GetActorLocation().X);
		const float ActorVertDist  = FMath::Abs(TopLeftWorldBoundsCoordinates.Y - InActor->GetActorLocation().Y);

		// Find how far along the actor is in the X axis - (0.f - 1.f)
		// Find how far along the actor is in the Y axis - (0.f - 1.f)
		const float XPercentage = ActorHorizDist / MaxDistances.X;
		const float YPercentage =  ActorVertDist / MaxDistances.Y;
		
		const FVector2D ScreenPosPercentages = FVector2D(XPercentage, YPercentage);
		
		ScreenPos = GetScreenLocationFromPercentage(ScreenPosPercentages);
	}

	return ScreenPos;
}

void UUI_Map::CalculateReferenceCoordinates()
{
	WorldBounds = Cast<AWorldBounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AWorldBounds::StaticClass()));
	if(WorldBounds)
	{
		FBox BoxBounds;
		FVector Origin;
		
		WorldBounds->GetActorBoxAndOrigin(BoxBounds, Origin);
		
		TopLeftWorldBoundsCoordinates.X = Origin.X - BoxBounds.GetExtent().X;
		TopLeftWorldBoundsCoordinates.Y = Origin.Y - BoxBounds.GetExtent().Y;
		TopLeftWorldBoundsCoordinates.Z = Origin.Z;

		BottomRightWorldBoundsCoordinates.X = Origin.X + BoxBounds.GetExtent().X;
		BottomRightWorldBoundsCoordinates.Y = Origin.Y + BoxBounds.GetExtent().Y;
		BottomRightWorldBoundsCoordinates.Z = Origin.Z;

		// Get max distance between (left right) and (top bottom) of world bounds
		MaxDistances.X = FMath::Abs(BottomRightWorldBoundsCoordinates.X - TopLeftWorldBoundsCoordinates.X);
		MaxDistances.Y = FMath::Abs(BottomRightWorldBoundsCoordinates.Y - TopLeftWorldBoundsCoordinates.Y);
	}
}

UUserWidget* UUI_Map::CreateMarker(const AActor* InActor)
{	
	for(auto KeyPair : MarkerMap)
	{
		if(InActor->IsA(KeyPair.Key))
		{
			UUserWidget* NewMarker = CreateWidget<UUserWidget>(this, KeyPair.Value);
			NewMarker->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			
			return NewMarker;
		}
	}

	return nullptr;
}

FVector2D UUI_Map::GetScreenLocationFromPercentage(FVector2D InPercentages) const
{
	const FVector2D StartPos = FVector2D::ZeroVector;

	//GEngine->AddOnScreenDebugMessage(8004, 15.0f, FColor::Blue, FString::Printf(TEXT("OverlayAsSlot XPos Location:         %f"), StartPos.X));
	//GEngine->AddOnScreenDebugMessage(8005, 15.0f, FColor::Blue, FString::Printf(TEXT("OverlayAsSlot YPos Location:         %f"), StartPos.Y));

	// calculate furthest bounds of widget
	const float FurthestXValue = StartPos.X + BoundsWidget->GetCachedGeometry().GetLocalSize().X;
	const float FurthestYValue = StartPos.Y + BoundsWidget->GetCachedGeometry().GetLocalSize().Y;

	//GEngine->AddOnScreenDebugMessage(8006, 15.0f, FColor::Blue, FString::Printf(TEXT("OverlayAsSlot X Size:         %f"), FurthestXValue));
	//GEngine->AddOnScreenDebugMessage(8007, 15.0f, FColor::Blue, FString::Printf(TEXT("OverlayAsSlot Y Size:         %f"), FurthestYValue));

	// lerp to find our actual screen pos
	const float FinalPosX = FMath::Lerp(StartPos.X, FurthestXValue, InPercentages.X);
	const float FinalPosY = FMath::Lerp(StartPos.Y, FurthestYValue, InPercentages.Y);

	//GEngine->AddOnScreenDebugMessage(8002, 15.0f, FColor::Blue, FString::Printf(TEXT("XPos Location:         %f"), FinalPosX));
	//GEngine->AddOnScreenDebugMessage(8003, 15.0f, FColor::Blue, FString::Printf(TEXT("YPos Location:         %f"), FinalPosY));

	// WE HAVE TO COMPENSATE FOR Y-axis in 3D coordinates not being the same orientation as the Y-Axis in 2D coordinate system
	// so we flip flop the X, Y values
	return FVector2D(FinalPosY, FinalPosX);
}

void UUI_Map::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	if(InFocusEvent.GetCause() != EFocusCause::OtherWidgetLostFocus)
	{
		CloseMap();
	}
}

FVector UUI_Map::GetNormalizeValueToWorldSpace(FVector2D NormalizedValue) const
{
	// lerp to find our actual world pos based on world bounds
	const float FinalPosX = FMath::Lerp(TopLeftWorldBoundsCoordinates.X, BottomRightWorldBoundsCoordinates.X, NormalizedValue.X);
	const float FinalPosY = FMath::Lerp(TopLeftWorldBoundsCoordinates.Y, BottomRightWorldBoundsCoordinates.Y, NormalizedValue.Y);
	
	return FVector(FinalPosX, FinalPosY,  0.f);
}

bool UUI_Map::GetNormalisedMousePositionInGeometry( UObject *WorldContextObject, FGeometry Geometry, FVector2D &Position )
{
	FVector2D PixelPosition;
	FVector2D ViewportMinPosition;
	FVector2D ViewportMaxPosition;

	// Get the top left and bottom right viewport positions
	USlateBlueprintLibrary::LocalToViewport( WorldContextObject, Geometry, FVector2D( 0, 0 ), PixelPosition, ViewportMinPosition );
	USlateBlueprintLibrary::LocalToViewport( WorldContextObject, Geometry, Geometry.GetLocalSize(), PixelPosition, ViewportMaxPosition );

	// Get the mouse's current position in the viewport
	FVector2D mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport( WorldContextObject );

	// Determine where the mouse is relative to the widget geometry
	Position = ( mousePosition - ViewportMinPosition ) / ( ViewportMaxPosition - ViewportMinPosition );

	// Calculate if the mouse is inside the geometry or not
	return FMath::Min( Position.X, Position.Y ) >= 0.f && FMath::Max( Position.X, Position.Y ) <= 1.f;
}