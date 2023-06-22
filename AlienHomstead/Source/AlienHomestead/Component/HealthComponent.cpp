// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Engine/TextRenderActor.h"
#include "AlienHomestead/Actors/WorldTextActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bCanEverAffectNavigation = false;
	bWantsInitializeComponent = true;

	TextActorClass = AWorldTextActor::StaticClass();
	
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::OnDamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = DamageType->GetClass();
	
	if(ShouldTakeDamage(Damage, DamageEvent, InstigatedBy, DamageCauser))
	{
		TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	}
}

bool UHealthComponent::ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	const bool bCanBeDamaged = Damage > 0 && CurrentHealth > 0;
	const bool bCanBeHealed  = Damage < 0 && CurrentHealth < MaxHealth;
	
	return IsAlive() && (bCanBeDamaged || bCanBeHealed);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser )
{
	if(IsAlive())
	{
		const float PreDamageHealth = CurrentHealth;
		
		CurrentHealth = CurrentHealth - Damage;

		if(GetWorld()->IsServer())
		{
			OnRep_CurrentHealth(PreDamageHealth);
		}

		if(IsDead() && OnDeath.IsBound())
		{
			OnDeath.Broadcast(GetOwner());
		}
	}
}

void UHealthComponent::OnRep_CurrentHealth(float OldValue)
{
	UE_LOG(LogTemp, Warning, TEXT("OldValue %f, NewValue %f"), OldValue, CurrentHealth);
	HealthValueChanged(OldValue - CurrentHealth);

	if(OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(OldValue, CurrentHealth);
	}
}

void UHealthComponent::HealthValueChanged_Implementation(float Difference)
{	
	// Random offset lets the text actors not stack on top of eachother
	const FVector RandomOffsetLoc = UKismetMathLibrary::RandomPointInBoundingBox(GetOwner()->GetActorLocation(), FVector(35.f));

	const FTransform SpawnTransform(GetOwner()->GetActorForwardVector().Rotation(), RandomOffsetLoc, FVector::OneVector);

	if(IsValid(TextActor) && TextType == ETextType::ETT_Stack)
	{
		const float PreviousValue = FCString::Atof(*TextActor->GetText().ToString());
		const float NewValue      = PreviousValue + Difference;

		FText Value = FText::AsNumber(NewValue);
		
		TextActor->SetText(Value);
		TextActor->SetActorTransform(SpawnTransform);
	}
	else
	{
		FActorSpawnParameters Params;
		
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Owner = GetOwner();	
		
		TextActor = GetWorld()->SpawnActor<AWorldTextActor>(TextActorClass, SpawnTransform, Params);

		ensure(TextActor);
		
		FText Value = FText::AsNumber(Difference);
		TextActor->SetText(Value);
	}
	
	TextActor->SetLifeSpan(2.f);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner() && GetOwner()->CanBeDamaged())
	{
		GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UHealthComponent::OnDamageReceived);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

#if WITH_EDITOR
void UHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.GetPropertyName() : NAME_None;

	if(PropName == GET_MEMBER_NAME_CHECKED(UHealthComponent, MaxHealth))
	{
		CurrentHealth = MaxHealth;
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif WITH_EDITOR