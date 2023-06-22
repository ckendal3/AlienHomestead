// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

UCLASS()
class ALIENHOMESTEAD_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:	

	ABaseAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UActionComponent* PlayerActionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* BehaviorTreeAsset;

protected:

	virtual void BeginPlay() override;
	
};