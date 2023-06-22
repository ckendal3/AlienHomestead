// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "AlienHomestead/UI/HUD/UI_BaseHUD.h"

ABaseHUD::ABaseHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseHUD::BeginPlay()
{
	UMGWidget = CreateWidget<UUI_BaseHUD>(GetWorld(), HUDUMGWidgetClass);

	if(UMGWidget)
	{
		UMGWidget->AddToPlayerScreen();
	}
	
	Super::BeginPlay();
}

UUI_BaseHUD* ABaseHUD::GetUMGHUD() const
{
	return UMGWidget;
}
