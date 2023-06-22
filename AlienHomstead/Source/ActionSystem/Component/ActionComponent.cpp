// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Actions/ActionData.h"


UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(const auto ActionClass : ActionClassArray)
	{
		if(ActionClass)
		{
			UActionData* NewActionObject = NewObject<UActionData>(GetWorld(), ActionClass);
			NewActionObject->Owner = this->GetOwner();
				
			ActionObjectArray.Add(NewActionObject);
		}
	}	
}

void UActionComponent::StartAction(FName ActionName, AActor* Instigator)
{
	for(const auto ActionObject : ActionObjectArray)
	{
		if(ActionName == ActionObject->GetActionName() && !ActionObject->IsRunning())
		{
			ActionObject->StartAction(Instigator);
		}
	}	
}

void UActionComponent::StopAction(FName ActionName, AActor* Instigator)
{
	for(const auto ActionObject : ActionObjectArray)
	{
		if(ActionName == ActionObject->GetActionName() && ActionObject->IsRunning())
		{
			ActionObject->EndAction(Instigator);
		}
	}	
}

void UActionComponent::StopAllActions(AActor* Instigator)
{
	for(const auto ActionObject : ActionObjectArray)
	{
		if(ActionObject->IsRunning())
		{
			ActionObject->EndAction(Instigator);
		}
	}
}

bool UActionComponent::IsActionRunning(FName ActionName) const
{
	for(const auto ActionObject : ActionObjectArray)
	{
		if(ActionName == ActionObject->GetActionName())
		{
			return ActionObject->IsRunning();
		}
	}

	return false;
}

bool UActionComponent::HasRunningAction() const
{
	for(const auto ActionObject : ActionObjectArray)
	{
		if(ActionObject->IsRunning())
		{
			return true;
		}
	}

	return false;
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(const auto ActionObject : ActionObjectArray)
	{
		if(ActionObject->IsRunning() && ActionObject->IsAllowedToTick())
		{
			ActionObject->NativeTick(DeltaTime);
			ActionObject->OnTick(DeltaTime);
		}
	}	
}
