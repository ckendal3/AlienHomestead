// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionData.generated.h"

/**
 * 
 */
UCLASS(abstract, Blueprintable, BlueprintType)
class ACTIONSYSTEM_API UActionData : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTickAction = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsRunning = false;

	/** The actor the owns this data. Typically spawned it.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	AActor* Owner;

	UFUNCTION(BlueprintCallable)
	virtual void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	virtual void EndAction(AActor* Instigator);
	
	virtual void NativeActionStart(AActor* Instigator);
	virtual void NativeActionEnd(AActor* Instigator);
	virtual void NativeTick(float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionStart(AActor* Instigator);

	UFUNCTION(BlueprintImplementableEvent)
	void OnActionEnd(AActor* Instigator);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTick(float DeltaTime);

	UFUNCTION(BlueprintPure)
	bool IsAllowedToTick() const;

	UFUNCTION(BlueprintPure)
	bool IsRunning() const { return bIsRunning; }

	UFUNCTION(BlueprintPure)
	FName GetActionName() const { return ActionName; }

	virtual UWorld* GetWorld() const override;
};
