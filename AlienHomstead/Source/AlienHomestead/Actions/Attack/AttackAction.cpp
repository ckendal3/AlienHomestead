// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAction.h"

FVector UAttackAction::GetStartLocation() const
{
	if(StartLocationType == ELocationType::ECustom)
	{
		return GetStartLocation_Custom();
	}
	
	FVector StartLocation = Owner ? Owner->GetActorLocation() : FVector::ZeroVector;

	if(StartLocationType == ELocationType::EOwnerSocketLocation)
	{
		const USkeletalMeshComponent* SKMeshComp = Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

		if(SKMeshComp && SKMeshComp->DoesSocketExist(StartSocketName))
		{
			StartLocation = SKMeshComp->GetSocketLocation(StartSocketName);
		}
	}

	return StartLocation;
}

FVector UAttackAction::GetStartLocation_Custom_Implementation() const
{
	unimplemented()
	return Owner->GetActorLocation();
}