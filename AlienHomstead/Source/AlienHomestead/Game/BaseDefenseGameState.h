// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseDefenseGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventActorToDefendChange, AActor*, InActor);

UCLASS()
class ALIENHOMESTEAD_API ABaseDefenseGameState : public AGameState
{
	GENERATED_BODY()

public:

	ABaseDefenseGameState();

	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasEnded() override;

	UPROPERTY(ReplicatedUsing=OnRep_ActorToDefend)
	AActor* ActorToDefend = nullptr;

	UPROPERTY(BlueprintAssignable)
	FEventActorToDefendChange OnEventActorToDefendChange;

	UFUNCTION()
	void OnRep_ActorToDefend();

	UFUNCTION()
	void OnDefendingActorHealthChanged(float OldValue, float NewValue);

	UFUNCTION()
	void BaseDestroyed(AActor* DestroyedBase);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
