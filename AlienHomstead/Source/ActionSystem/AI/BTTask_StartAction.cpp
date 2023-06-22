// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_StartAction.h"
#include "AIController.h"
#include "Actions/ActionData.h"
#include "Component/ActionComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTask_StartAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ensure(ActionToStart);

	FBTStartAction_Memory* ActionNodeMemory = reinterpret_cast<FBTStartAction_Memory*>(NodeMemory);
	if(ActionNodeMemory->bInProgress)
	{
		return EBTNodeResult::InProgress;
	}

	ACharacter*       Char       = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UActionComponent* ActionComp = Cast<UActionComponent>(Char->GetComponentByClass(UActionComponent::StaticClass()));

	if(ActionComp)
	{
		const UActionData* ActionDataCDO = Cast<UActionData>(ActionToStart->GetDefaultObject());

		ActionComp->StartAction(ActionNodeMemory->ActionName, Char);
		
		ActionNodeMemory->ActionName      = ActionDataCDO->GetActionName();
		ActionNodeMemory->ActionComponent = ActionComp;
		ActionNodeMemory->bInProgress     = ActionComp->IsActionRunning(ActionNodeMemory->ActionName);
		
		if(ActionNodeMemory->bInProgress == false)
		{
			// There is the chance that this is a single frame action and will be completed immediately
			return EBTNodeResult::Succeeded;
		}

		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Aborted;
}

void UBTTask_StartAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTStartAction_Memory* ActionNodeMemory = reinterpret_cast<FBTStartAction_Memory*>(NodeMemory);

	if(ActionNodeMemory->ActionComponent == nullptr)
	{
		FinishLatentAbort(OwnerComp);
	}

	if(	ActionNodeMemory->bInProgress == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	ActionNodeMemory->bInProgress = ActionNodeMemory->ActionComponent->IsActionRunning(ActionNodeMemory->ActionName);

	if(ActionNodeMemory->bInProgress == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_StartAction::GetInstanceMemorySize() const
{
	return sizeof(FBTStartAction_Memory);
}