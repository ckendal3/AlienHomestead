// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartAction.generated.h"


struct FBTStartAction_Memory
{
	bool bInProgress;
	class UActionComponent* ActionComponent;
	FName ActionName;
};


/**
 * 
 */
UCLASS()
class ACTIONSYSTEM_API UBTTask_StartAction : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UActionData> ActionToStart;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
