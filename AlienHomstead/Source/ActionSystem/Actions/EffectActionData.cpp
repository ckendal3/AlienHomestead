// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActionData.h"

#include "Component/ActionComponent.h"


void UEffectActionData::StartAction(AActor* Instigator)
{
	Delegate_Duration.BindUObject(this, &UEffectActionData::EndAction, Owner);
	Delegate_Period.BindUObject(this, &UEffectActionData::PeriodicEvent, Instigator);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, Delegate_Duration, Duration, false,Duration);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, Delegate_Period, Period, true, Period);
	
	Super::StartAction(Instigator);
}

void UEffectActionData::EndAction(AActor* Instigator)
{
	if(FMath::IsNearlyZero(GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period)))
	{
		PeriodicEvent(Owner);
	}
	
	Super::EndAction(Instigator);
}

void UEffectActionData::PeriodicEvent_Implementation(AActor* Instigator) { }
