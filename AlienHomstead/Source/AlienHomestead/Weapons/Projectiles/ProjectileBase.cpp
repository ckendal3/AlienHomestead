// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AlienHomestead/AlienHomestead.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
	// Sphere collision for hit detection
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionObjectType(ECC_Projectile);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	// Projectile Movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3500.f;
	ProjectileMovement->MaxSpeed = 3500.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	InitialLifeSpan = 5.0f;
}

void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OverlappedComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, SweepResult.ImpactPoint);
	}

	if(OtherActor->CanBeDamaged())
	{
		FPointDamageEvent PointDamageEvent;
		PointDamageEvent.Damage = 10.f;
		PointDamageEvent.HitInfo = SweepResult;

		AController* OwningController = GetInstigator() ? GetInstigator()->Controller : nullptr;
		
		OtherActor->TakeDamage(Damage, PointDamageEvent, OwningController, this);
	}

	CurrentHits++;

	if(CurrentHits >= MaxHits)
	{
		Destroy();
	}
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComp->IgnoreActorWhenMoving(GetInstigator(), true);

	CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComp->IgnoreActorWhenMoving(GetInstigator(), true);
}