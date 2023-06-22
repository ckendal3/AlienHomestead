// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_FloatingDamage.h"
#include "Components/TextBlock.h"

void UUI_FloatingDamage::SetText(FText& InText)
{
	NativeSetText(InText);
	OnSetText(InText);
}

void UUI_FloatingDamage::NativeSetText(FText& InText)
{
	DamageText->SetText(InText);
}