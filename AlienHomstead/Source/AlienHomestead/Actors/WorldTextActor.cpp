// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldTextActor.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "AlienHomestead/UI/UI_FloatingDamage.h"


AWorldTextActor::AWorldTextActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MainComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainRootComponent"));
	RootComponent = MainComponent;

	TextWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextComponent"));
	TextWidgetComponent->SetupAttachment(RootComponent);
	TextWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWorldTextActor::SetText(FText& InText)
{
	if(UUI_FloatingDamage* UI_FloatingDamage = Cast<UUI_FloatingDamage>(TextWidgetComponent->GetWidget()))
	{
		UI_FloatingDamage->SetText(InText);
	}
}

FText AWorldTextActor::GetText() const
{
	if(UUI_FloatingDamage* UI_FloatingDamage = Cast<UUI_FloatingDamage>(TextWidgetComponent->GetWidget()))
	{
		if(UI_FloatingDamage->DamageText)
		{
			return UI_FloatingDamage->DamageText->GetText();
		}
	}
	
	return FText::GetEmpty();
}