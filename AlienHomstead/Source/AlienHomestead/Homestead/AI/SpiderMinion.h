// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlienHomestead/AI/BaseAICharacter.h"
#include "AlienHomestead/Homestead/Types/HomesteadTypes.h"
#include "SpiderMinion.generated.h"

USTRUCT(BlueprintType)
struct FElementalData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorSpawnedOnDeath;
};


UCLASS()
class ALIENHOMESTEAD_API ASpiderMinion : public ABaseAICharacter
{
	GENERATED_BODY()

public:
	
	ASpiderMinion();

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_ElementalType, Category = "Elemental")
	TEnumAsByte<EElementalType> ElementalType = EElementalType::ET_None;

	UPROPERTY(EditAnywhere, Category = "Elemental")
	TMap<EElementalType, FElementalData> ElementalDataMap;

	// Set from Elemental Data
	UPROPERTY()
	TSubclassOf<AActor> ActorSpawnedOnDeath;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetElementalType(EElementalType NewElementalType);
	
protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnDeath(AActor* KilledActor) override;
	
	UFUNCTION()
	void OnRep_ElementalType();
};
