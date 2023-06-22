// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class UActionData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONSYSTEM_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActionComponent();

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UActionData>> ActionClassArray;

	UPROPERTY()
	TArray<UActionData*> ActionObjectArray;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void StartAction(FName ActionName, AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	void StopAction(FName ActionName, AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	void StopAllActions(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	bool IsActionRunning(FName ActionName) const;

	UFUNCTION(BlueprintCallable)
	bool HasRunningAction() const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
