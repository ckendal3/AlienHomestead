// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldBounds.h"
#include "Components/BoxComponent.h"

AWorldBounds::AWorldBounds()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent0"));
	RootComponent = BoxComponent;
	BoxComponent->Mobility = EComponentMobility::Movable;
	BoxComponent->SetCanEverAffectNavigation(false);

	BoxComponent->bDrawOnlyIfSelected = true;
	BoxComponent->bUseAttachParentBound = false;
	BoxComponent->bUseEditorCompositing = true;
	BoxComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	BoxComponent->InitBoxExtent(FVector(100.f, 100.f, 100.f));
}

void AWorldBounds::GetActorBoxAndOrigin(FBox& BoxBounds, FVector& Origin)
{
	BoxBounds = BoxComponent->Bounds.GetBox();
	Origin    = BoxComponent->Bounds.Origin;
}
