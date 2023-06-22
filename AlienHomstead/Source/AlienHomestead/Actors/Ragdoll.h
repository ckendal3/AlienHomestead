// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ragdoll.generated.h"

UCLASS()
class ALIENHOMESTEAD_API ARagdoll : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARagdoll();

	/** The main skeletal mesh associated with this Ragdoll. */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	UFUNCTION(BlueprintCallable)
	void SetMesh(USkeletalMesh* InMesh, UMaterialInterface* Material);
};