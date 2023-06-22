// Copyright Epic Games, Inc. All Rights Reserved.

#include "IsoShooterCharacter.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Component/ActionComponent.h"
#include "Component/HealthComponent.h"
#include "Component/InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UI_PlayerHealthWidget.h"
#include "Weapons/Weapon.h"

AIsoShooterCharacter::AIsoShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	CameraBoom->TargetArmLength = 750.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch    = false;
	CameraBoom->bInheritYaw      = false;
	CameraBoom->bInheritRoll     = false;
	CameraBoom->bDoCollisionTest = false;
	
	// Create a follow camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PlayerInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	HealthWidgetComponent->SetupAttachment(GetCapsuleComponent());
	HealthWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	HealthWidgetComponent->SetUsingAbsoluteRotation(true);
}

void AIsoShooterCharacter::ReceiveHealthChangedEvent(float OldValue, float NewValue)
{
	if(UUI_PlayerHealthWidget* HealthWidget = Cast<UUI_PlayerHealthWidget>(HealthWidgetComponent->GetWidget()))
	{
		const float MaxHealth = GetHealthComponent()->MaxHealth;
		HealthWidget->OnHealthChanged(NewValue, MaxHealth);
	}
}

void AIsoShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AIsoShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup widget to have correct value from start
	const float MaxHealth = GetHealthComponent()->MaxHealth;
	ReceiveHealthChangedEvent(MaxHealth, MaxHealth);
}

void AIsoShooterCharacter::Restart()
{
	Super::Restart();

	PlayerInventoryComponent->InitializeDefaultInventory(false);
}

FRotator AIsoShooterCharacter::GetBaseAimRotation() const
{
	return AimRotator;
}

void AIsoShooterCharacter::PawnStartFire(uint8 FireModeNum)
{
	if(GetWorld()->IsClient())
	{
		Server_R_PawnStartFire(FireModeNum);
	}
	else
	{
		Multicast_R_PawnStartFire(FireModeNum);
	}
}

void AIsoShooterCharacter::PawnStopFire(uint8 FireModeNum)
{
	if(GetWorld()->IsClient())
	{
		Server_R_PawnStopFire(FireModeNum);
	}
	else
	{
		Multicast_R_PawnStopFire(FireModeNum);
	}
}

void AIsoShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController && IsLocallyControlled())
	{
		FVector MouseWorldPos, WorldDir;
		const bool bValidProjection = PlayerController->DeprojectMousePositionToWorld(MouseWorldPos, WorldDir);

		/**
		FVector2D CharScreenPos;
		FVector2D MouseScreenPos;
		
		PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), CharScreenPos);
		PlayerController->GetMousePosition(MouseScreenPos.X, MouseScreenPos.Y);

		FVector2D LookLoc2D = (CharScreenPos - MouseScreenPos);
		
		//Invert X-Axis
		const FVector LookLoc = FVector(LookLoc2D.Y, -LookLoc2D.X, 0.f);
		const FVector LookDir = LookLoc.GetSafeNormal();
		

		LookAt(LookDir.Rotation());
		*/

		
		if(bValidProjection)
		{			
			FCollisionResponseParams ResponseParams = FCollisionResponseParams::DefaultResponseParam;
			
			FCollisionQueryParams QueryParams       = FCollisionQueryParams::DefaultQueryParam;
			QueryParams.bTraceComplex = true;
			QueryParams.AddIgnoredActor(this);
			
			FVector EndLoc = MouseWorldPos + (WorldDir * 100000.f);

			FHitResult OutHitResult;

			// TODO: Accommodate if we aim outside world
			bool bHadHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, MouseWorldPos, EndLoc, ECollisionChannel::ECC_Visibility, QueryParams, ResponseParams);

			FVector LookLocation = OutHitResult.Location + (OutHitResult.ImpactNormal * 10);
			
			// Find an enemy to target
			if(bHadHit)
			{
				FCollisionShape CollisionShape = FCollisionShape::MakeSphere(196.f);

				FCollisionQueryParams FindTargetQueryParams;
				FindTargetQueryParams.AddIgnoredActor(this);
				FindTargetQueryParams.AddIgnoredActor(Weapon);

				FCollisionResponseParams FindTargetResponseParams;
				FindTargetResponseParams = FCollisionResponseParams::DefaultResponseParam;

				TArray<FOverlapResult> OverlapResults;
				
				if(GetWorld()->OverlapMultiByChannel(OverlapResults, OutHitResult.Location, FQuat::Identity, ECollisionChannel::ECC_Pawn, CollisionShape, FindTargetQueryParams, FindTargetResponseParams))
				{
					FOverlapResult FoundResult = OverlapResults[0];
					float          Distance    = FVector::Dist(FoundResult.Actor->GetActorLocation(), OutHitResult.Location);
					
					for(FOverlapResult& Result : OverlapResults)
					{
						float NewDist = FVector::Dist(Result.Actor->GetActorLocation(), OutHitResult.Location);
						if(NewDist < Distance)
						{
							Distance = NewDist;
							FoundResult = Result;
						}
					}

					if(FoundResult.Actor->IsA(ACharacterBase::StaticClass()))
					{
						LookLocation = FoundResult.Actor->GetActorLocation();
						DrawDebugSphere(GetWorld(), LookLocation, 196.f, 32, FColor::Blue, false, .016f, 32, 8.f);
					}
				}
			}			
			
			FRotator FinalLookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookLocation);
			FinalLookRot.Roll  = 0.f;
			FinalLookRot.Pitch = 0.f;
			
			LookAt(FinalLookRot, LookLocation);

			if(Weapon)
			{
				const FRotator WeaponLookAtRot = UKismetMathLibrary::FindLookAtRotation(Weapon->GetActorLocation(), LookLocation);
				
				FRotator NewWeaponRot = Weapon->GetActorRotation();
				NewWeaponRot.Yaw      = WeaponLookAtRot.Yaw + -90.f;

				Weapon->SetActorRotation(NewWeaponRot);
			}
			
			// CrosshairMesh->SetHiddenInGame(!bHadHit);
			// CrosshairMesh->SetWorldLocation(LookLocation);
			// CrosshairMesh->SetWorldRotation(FRotator::ZeroRotator);
		}
	}

	/**
	/* Can be uncommented to debug the aiming inconsistency
	if(Weapon)
	{
		DrawDebugLine(GetWorld(), Weapon->GetActorLocation(), Weapon->GetActorLocation() + (3000.f * GetBaseAimRotation().Vector()), FColor::Purple, false, .016f);
	}	

	DrawDebugLine(GetWorld(), GetCapsuleComponent()->GetComponentLocation(), GetCapsuleComponent()->GetComponentLocation() + (15000.f * GetBaseAimRotation().Vector()), FColor::Red, false, .016f);
	*/
}

void AIsoShooterCharacter::SwitchItem(float Value)
{
	PlayerInventoryComponent->SwitchItems(Value);
}

void AIsoShooterCharacter::Sprint(bool bStart)
{
	if(GetWorld()->IsClient())
	{
		Server_R_Sprint(bStart);
	}
	else
	{
		Multicast_R_Sprint(bStart);
	}
}

void AIsoShooterCharacter::LookAt(FRotator LookRotation, FVector LookLocation)
{
	if(IsLocallyControlled())
	{
		AimRotator  = LookRotation;
		AimLocation = LookLocation;
		
		SetActorRotation(LookRotation);

		if(GetWorld()->IsClient())
		{
			Server_LookAt(LookRotation, LookLocation);
		}
		else
		{
			Multicast_LookAt(LookRotation, LookLocation);
		}
	}
}

void AIsoShooterCharacter::Server_LookAt_Implementation(FRotator LookRotation, FVector LookLocation)
{
	Multicast_LookAt(LookRotation, LookLocation);
}

void AIsoShooterCharacter::Multicast_LookAt_Implementation(FRotator LookRotation, FVector LookLocation)
{
	if(IsLocallyControlled() == false)
	{
		AimRotator  = LookRotation;
		AimLocation = LookLocation;

		SetActorRotation(LookRotation);
	}
}

void AIsoShooterCharacter::Server_R_PawnStartFire_Implementation(uint8 FireModeNum)
{
	Multicast_R_PawnStartFire(FireModeNum);
}

void AIsoShooterCharacter::Multicast_R_PawnStartFire_Implementation(uint8 FireModeNum)
{
	if(Weapon != nullptr)
	{
		Weapon->StartAction(FireModeNum);
	}
}

void AIsoShooterCharacter::Server_R_PawnStopFire_Implementation(uint8 FireModeNum)
{
	Multicast_R_PawnStopFire(FireModeNum);
}

void AIsoShooterCharacter::Multicast_R_PawnStopFire_Implementation(uint8 FireModeNum)
{
	if(Weapon != nullptr)
	{
		Weapon->StopAction(FireModeNum);
	}
}

void AIsoShooterCharacter::Server_R_Sprint_Implementation(bool bStart)
{
	Multicast_R_Sprint(bStart);
}

void AIsoShooterCharacter::Multicast_R_Sprint_Implementation(bool bStart)
{
	if(bStart)
	{
		GetActionComponent()->StartAction("Sprint", this);
	}
	else
	{
		GetActionComponent()->StopAction("Sprint", this);
	}
}

void AIsoShooterCharacter::ActionAbility(uint8 ActionSlotNum, bool bEnd)
{
	const FName ActionName = ActionSlotNum > 0 ? SecondaryAbilityName : PrimaryAbilityName;
	
	if(GetWorld()->IsClient())
	{
		Server_R_ActionAbility(ActionName, bEnd);
	}
	else
	{
		Multicast_R_ActionAbility(ActionName, bEnd);
	}
}

void AIsoShooterCharacter::Server_R_ActionAbility_Implementation(FName ActionName, bool bEnd)
{
	Multicast_R_ActionAbility(ActionName, bEnd);
}

void AIsoShooterCharacter::Multicast_R_ActionAbility_Implementation(FName ActionName, bool bEnd)
{
	if(bEnd)
	{
		GetActionComponent()->StartAction(FName(ActionName), this);
	}
	else
	{
		GetActionComponent()->StopAction(FName(ActionName), this);
	}
}

void AIsoShooterCharacter::MoveForward(float Value)
{
	if((Value != 0.0f) )
	{
		if ( GetFollowCamera() )
		{
			// find out which way is forward
			const FRotator CamRotation = GetFollowCamera()->GetComponentRotation();
			const FRotator CorrectedRot(0.f, 0.f, CamRotation.Yaw);

			const FVector FinalDir = UKismetMathLibrary::GetForwardVector(CorrectedRot);

			AddMovementInput(FinalDir, Value);
		}
	}
}

void AIsoShooterCharacter::MoveRight(float Value)
{
	if((Value != 0.0f))
	{
		if ( GetFollowCamera())
		{
			// find out which way is forward
			const FRotator CamRotation = GetFollowCamera()->GetComponentRotation();
			const FRotator CorrectedRot(0.f, 0.f, CamRotation.Yaw);

			const FVector FinalDir = UKismetMathLibrary::GetRightVector(CorrectedRot);

			AddMovementInput(FinalDir, Value);
		}
	}
}

void AIsoShooterCharacter::SetWeapon(AWeapon* InWeapon)
{
	Weapon = InWeapon;
}