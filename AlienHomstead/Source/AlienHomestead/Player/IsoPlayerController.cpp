// Fill out your copyright notice in the Description page of Project Settings.

#include "IsoPlayerController.h"

#include "BaseHUD.h"
#include "GameFramework/Character.h"
#include "AlienHomestead/IsoShooterCharacter.h"
#include "AlienHomestead/Homestead/UI/UI_Map.h"
#include "Blueprint/UserWidget.h"

AIsoPlayerController::AIsoPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AIsoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AIsoPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AIsoPlayerController::MoveRight);
	
	InputComponent->BindAction("Jump", IE_Pressed, this, &AIsoPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AIsoPlayerController::StopJumping);

	InputComponent->BindAction("PrimaryItemAction", IE_Pressed, this, &AIsoPlayerController::PrimaryItemActionStart);
	InputComponent->BindAction("PrimaryItemAction", IE_Released, this, &AIsoPlayerController::PrimaryItemActionStop);

	InputComponent->BindAction("SecondaryItemAction", IE_Pressed, this, &AIsoPlayerController::SecondaryItemActionStart);
	InputComponent->BindAction("SecondaryItemAction", IE_Released, this, &AIsoPlayerController::SecondaryItemActionStop);

	InputComponent->BindAction("SwitchWeapons", IE_Pressed, this, &AIsoPlayerController::SwitchItem);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AIsoPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AIsoPlayerController::StopSprinting);

	InputComponent->BindAction("PrimaryAbility", IE_Pressed, this, &AIsoPlayerController::PrimaryAbilityActionStart);
	InputComponent->BindAction("PrimaryAbility", IE_Released, this, &AIsoPlayerController::PrimaryAbilityActionStop);

	InputComponent->BindAction("SecondaryAbility", IE_Pressed, this, &AIsoPlayerController::SecondaryAbilityActionStart);
	InputComponent->BindAction("SecondaryAbility", IE_Released, this, &AIsoPlayerController::SecondaryAbilityActionStop);

	InputComponent->BindAction("Map", IE_Pressed, this, &AIsoPlayerController::ToggleMap);
}

void AIsoPlayerController::Jump()
{
	if(GetCharacter())
	{
		GetCharacter()->Jump();
	}
}

void AIsoPlayerController::StopJumping()
{
	if(GetCharacter())
	{
		GetCharacter()->StopJumping();
	}
}

void AIsoPlayerController::MoveForward(float Value)
{
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter()))
	{
		Char->MoveForward(Value);
	}
}

void AIsoPlayerController::MoveRight(float Value)
{
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter()))
	{
		Char->MoveRight(Value);
	}
}

void AIsoPlayerController::SwitchItem()
{
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter()))
	{
		Char->SwitchItem(1.f);
	}
}

void AIsoPlayerController::StartSprint()
{
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter()))
	{
		Char->Sprint(true);
	}
}

void AIsoPlayerController::StopSprinting()
{
	if(AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter()))
	{
		Char->Sprint(false);
	}
}

void AIsoPlayerController::PrimaryItemActionStart()
{
	StartFire(0);
}

void AIsoPlayerController::PrimaryItemActionStop()
{
	StopFire(0);
}

void AIsoPlayerController::SecondaryItemActionStart()
{
	StartFire(1);
}

void AIsoPlayerController::SecondaryItemActionStop()
{
	StopFire(1);
}

void AIsoPlayerController::StartFire(uint8 FireModeNum)
{
	Super::StartFire(FireModeNum);
	// This calls PawnStartFire already - this is purely for visibility
}

void AIsoPlayerController::StopFire(uint8 FireModeNum)
{
	AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter());
	
	if ( Char && !GetWorld()->bPlayersOnly )
	{
		Char->PawnStopFire( FireModeNum );
	}
}

void AIsoPlayerController::PrimaryAbilityActionStart()
{
	AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter());
	
	if (Char)
	{
		Char->ActionAbility( 0, false);
	}
}

void AIsoPlayerController::PrimaryAbilityActionStop()
{
	AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter());
	
	if(Char)
	{
		Char->ActionAbility( 0, true);
	}
}

void AIsoPlayerController::SecondaryAbilityActionStart()
{
	AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter());
	
	if (Char)
	{
		Char->ActionAbility( 1, false);
	}
}

void AIsoPlayerController::SecondaryAbilityActionStop()
{
	AIsoShooterCharacter* Char = Cast<AIsoShooterCharacter>(GetCharacter());
	
	if(Char)
	{
		Char->ActionAbility( 1, true);
	}
}

void AIsoPlayerController::ToggleMap()
{
	if(MapWidget == nullptr || MapWidget->IsInViewport() == false)
	{
		MapWidget = nullptr;
		
		MapWidget = CreateWidget<UUI_Map>(this, MapWidget_Class);
		MapWidget->AddToPlayerScreen();
	}
	else
	{
		MapWidget->RemoveFromViewport();
		MapWidget = nullptr;
	}
}

UUI_BaseHUD* AIsoPlayerController::GetUMGHUD() const
{
	if(ABaseHUD* BaseHUD = GetHUD<ABaseHUD>())
	{
		return BaseHUD->UMGWidget;
	}
	
	return nullptr;
}
