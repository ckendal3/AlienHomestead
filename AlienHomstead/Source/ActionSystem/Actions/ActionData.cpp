// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionData.h"

void UActionData::StartAction(AActor* Instigator)
{
	bIsRunning = true;
	
	NativeActionStart(Instigator);
	OnActionStart(Instigator);

	if(bTickAction == false)
	{
		EndAction(Instigator);
	}
}

void UActionData::EndAction(AActor* Instigator)
{
	NativeActionEnd(Instigator);	
	OnActionEnd(Instigator);

	bIsRunning = false;
}

void UActionData::NativeActionStart(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("NativeActionStart"));
}

void UActionData::NativeActionEnd(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("NativeActionEnd"));
}

void UActionData::NativeTick(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("Tick"));
}

bool UActionData::IsAllowedToTick() const
{
	return bTickAction;
}

UWorld* UActionData::GetWorld() const
{
	if ( HasAllFlags(RF_ClassDefaultObject) )
	{
		return nullptr;
	}

	if(GetOuter()->GetWorld())
	{
		return GetOuter()->GetWorld();
	}

	if(Owner)
	{
		return Owner->GetWorld();
	}

	return nullptr;
}