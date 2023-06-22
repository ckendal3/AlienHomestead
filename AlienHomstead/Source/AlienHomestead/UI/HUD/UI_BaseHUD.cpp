// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_BaseHUD.h"
#include "AlienHomestead/Component/HealthComponent.h"
#include "AlienHomestead/Game/BaseDefenseGameState.h"

// TODO: CLEANUP THIS SHENANIGANS

void UUI_BaseHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if(ABaseDefenseGameState* GS = Cast<ABaseDefenseGameState>(GetWorld()->GetGameState()))
	{
		if(GS && GS->ActorToDefend)
		{
			GS->OnEventActorToDefendChange.AddUniqueDynamic(this, &UUI_BaseHUD::OnDefensableActorChanged);

			if(GS->ActorToDefend)
			{
				OnDefensableActorChanged(GS->ActorToDefend);
			}
		}
	}
}

void UUI_BaseHUD::NativeDestruct()
{
	Super::NativeDestruct();
	
	if(DefendingActor)
	{
		UHealthComponent* HealthComponent = Cast<UHealthComponent>(DefendingActor->GetComponentByClass(UHealthComponent::StaticClass()));

		if(HealthComponent)
		{
			HealthComponent->OnHealthChanged.RemoveDynamic(this, &UUI_BaseHUD::OnDefensableActorDamaged);
		}
	}
}

void UUI_BaseHUD::OnDefensableActorDamaged(float OldValue, float NewValue)
{
	CurHealth = NewValue;
	OnDefensableActorDamaged_BP(MaxHealth, CurHealth);
}

void UUI_BaseHUD::OnDefensableActorChanged(AActor* InActor)
{
	if(InActor)
	{
		if(DefendingActor != InActor)
		{
			if(DefendingActor)
			{
				// Remove bind to to old defendable health component
				UHealthComponent* OldHealthComponent = Cast<UHealthComponent>(DefendingActor->GetComponentByClass(UHealthComponent::StaticClass()));
	
				if(OldHealthComponent)
				{
					OldHealthComponent->OnHealthChanged.RemoveDynamic(this, &UUI_BaseHUD::OnDefensableActorDamaged);
				}
			}

			// Bind to new defendable health component
			UHealthComponent* HealthComponent = Cast<UHealthComponent>(InActor->GetComponentByClass(UHealthComponent::StaticClass()));
	
			if(HealthComponent)
			{
				MaxHealth = HealthComponent->MaxHealth;
				CurHealth = HealthComponent->CurrentHealth;
				
				HealthComponent->OnHealthChanged.AddDynamic(this, &UUI_BaseHUD::OnDefensableActorDamaged);

				OnDefensableActorDamaged(MaxHealth, CurHealth);
			}
		}
	}

	DefendingActor = InActor;
}