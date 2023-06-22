// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseDefenseGameState.h"

#include "AlienHomestead/Base/DefendableBase.h"
#include "AlienHomestead/Component/HealthComponent.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABaseDefenseGameState::ABaseDefenseGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseDefenseGameState::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	if(GetWorld()->IsServer())
	{
		AActor* BaseToDefend = UGameplayStatics::GetActorOfClass(GetWorld(), ADefendableBase::StaticClass());

		if(BaseToDefend)
		{
			UHealthComponent* HealthComponent = Cast<UHealthComponent>(BaseToDefend->GetComponentByClass(UHealthComponent::StaticClass()));
		
			if(HealthComponent)
			{
				HealthComponent->OnDeath.AddDynamic(this, &ABaseDefenseGameState::BaseDestroyed);
			}
		}

		ActorToDefend = BaseToDefend;
		OnRep_ActorToDefend();
	}
}

void ABaseDefenseGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	// TODO: DISPLAY SCOREBOARD

	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	//UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentLevelName));
	if(GetWorld()->IsServer())
	{
		GetWorld()->ServerTravel(CurrentLevelName);
	}
}

void ABaseDefenseGameState::OnRep_ActorToDefend()
{
	if(ActorToDefend)
	{
		UHealthComponent* HealthComponent = Cast<UHealthComponent>(ActorToDefend->GetComponentByClass(UHealthComponent::StaticClass()));
	
		if(HealthComponent)
		{
			HealthComponent->OnHealthChanged.AddDynamic(this, &ABaseDefenseGameState::OnDefendingActorHealthChanged);
		}
	}

	if(OnEventActorToDefendChange.IsBound())
	{
		OnEventActorToDefendChange.Broadcast(ActorToDefend);
	}
}

void ABaseDefenseGameState::OnDefendingActorHealthChanged(float OldValue, float NewValue)
{
	uint32 Index = 0;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController->GetHUD())
		{
			// TODO: SETUP BASE HEALTH CALLBACK?
			// MAYBE HUD JUST HANDLES THE BINDING!?!??!?!?!?
		}
		
		Index++;
	}
}

void ABaseDefenseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseDefenseGameState, ActorToDefend);
}


void ABaseDefenseGameState::BaseDestroyed(AActor* DestroyedBase)
{
	SetMatchState(MatchState::WaitingPostMatch);
}