// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/Character.h"
#include "IsoShooterCharacter.generated.h"

// TODO: LOOK INTO OFFLOADING ALL WEAPON SHOOTING TO THE ITEM/WEAPON
// THAT SHOULD ALSO PLAY THE ANIMATION ON THE CHARACTER, ETC

// TODO: Should make an enum that is `EStart, EEnd` for actions?

// TODO: Sprint could utilize packing with bits for movement data ?
// TODO: Sprint could also just call Start/Stop Action ability RPC and pass in the enum?

// TODO: Weapon aiming and crosshair are slightly off due to socket location/animations

UCLASS(config=Game)
class AIsoShooterCharacter : public ACharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* PlayerInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthWidgetComponent;
	
public:
	AIsoShooterCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadWrite)
	class AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Char|Ability")
	FName PrimaryAbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Char|Ability")
	FName SecondaryAbilityName;

	FRotator AimRotator;
	FVector  AimLocation;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	virtual void LookAt(FRotator LookRotation, FVector LookLocation);
	
	UFUNCTION(BlueprintCallable)
	virtual void SwitchItem(float Value);

	UFUNCTION(BlueprintCallable)
	virtual void Sprint(bool bStart);

	virtual void PawnStartFire(uint8 FireModeNum = 0) override;
	
	/** Handle StopFire() passed from PlayerController */
	virtual void PawnStopFire(uint8 FireModeNum = 0);

	UFUNCTION(BlueprintCallable)
	virtual void ActionAbility(uint8 ActionSlotNum, bool bEnd);

	UFUNCTION(BlueprintCallable)
	void SetWeapon(AWeapon* InWeapon);

	UFUNCTION(BlueprintPure)
	AWeapon* GetWeapon() const { return Weapon; }

protected:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(Unreliable, Server)
	virtual void Server_LookAt(FRotator LookRotation, FVector LookLocation);

	UFUNCTION(NetMulticast, Unreliable)
	virtual void Multicast_LookAt(FRotator LookRotation, FVector LookLocation);

	UFUNCTION(Server, Reliable)
	virtual void Server_R_PawnStartFire(uint8 FireModeNum = 0);

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_R_PawnStartFire(uint8 FireModeNum = 0);

	UFUNCTION(Server, Reliable)
	virtual void Server_R_PawnStopFire(uint8 FireModeNum = 0);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_R_PawnStopFire(uint8 FireModeNum = 0);

	UFUNCTION(Server, Reliable)
	virtual void Server_R_Sprint(bool bStart);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_R_Sprint(bool bStart);

	UFUNCTION(Server, Reliable)
	virtual void Server_R_ActionAbility(FName ActionName, bool bEnd);

	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_R_ActionAbility(FName ActionName, bool bEnd);
	
protected:

	virtual void ReceiveHealthChangedEvent(float OldValue, float NewValue) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay();

	virtual void Restart() override;

	virtual FRotator GetBaseAimRotation() const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return PlayerCamera; }

	/** Returns the location the player is looking at. **/
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetAimLocation() const { return AimLocation; }
};