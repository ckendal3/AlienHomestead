// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UActionData;

UCLASS()
class ALIENHOMESTEAD_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UActionComponent* ActionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SocketName = "hand_r";

	virtual void NativeEquipped();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipped();

	virtual void NativeUnequipped();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequipped();

	UFUNCTION(BlueprintCallable)
	void StartAction(int FireMode);

	UFUNCTION(BlueprintCallable)
	void StopAction(int FireMode);

	UFUNCTION(BlueprintNativeEvent)
	void PrimaryAction(bool bStart);

	UFUNCTION(BlueprintNativeEvent)
	void SecondaryAction(bool bStart);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName PrimaryActionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SecondaryActionName;

	UFUNCTION(BlueprintCallable)
	bool IsLocallyControlled() const;
};