// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseAnimInstance.h"
#include "AlienHomestead/Characters/CharacterBase.h"

UCharacterBaseAnimInstance::UCharacterBaseAnimInstance() { }

ACharacterBase* UCharacterBaseAnimInstance::GetOwningCharacter() const
{
	return Cast<ACharacterBase>(GetOwningActor());
}

void UCharacterBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(GetOwningCharacter())
	{
		Velocity      = GetOwningCharacter()->GetVelocity();
		LookDirection = GetOwningCharacter()->GetBaseAimRotation();

		/** Used to debug Velocity vs Look Direction
		const FVector ActorLoc = GetOwningActor()->GetActorLocation();
	
		DrawDebugDirectionalArrow(GetOwningCharacter()->GetWorld(), ActorLoc, ActorLoc + (GetLookDirection().Vector() * 50.f), 32.f, FColor::Blue);
		DrawDebugDirectionalArrow(GetOwningCharacter()->GetWorld(), ActorLoc, ActorLoc + (Velocity.GetSafeNormal() * 50.f), 32.f, FColor::Green);
		*/
	}
}

float UCharacterBaseAnimInstance::GetLookToMovementDirectionalFloat() const
{
	return CalculateDirection(Velocity, GetLookDirection());
}