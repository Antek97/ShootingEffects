#include "ParadarkProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "ProjectilePoolManager.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"

AParadarkProjectile::AParadarkProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AParadarkProjectile::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	PrimaryActorTick.bCanEverTick = true;

}

void AParadarkProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{		
		ensure(AmmoEffect);

		if (IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			if (UAbilitySystemComponent* AbilitySystem = ASInterface->GetAbilitySystemComponent())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 300.0f, GetActorLocation());
				FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(AmmoEffect, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
		PoolManager->ReturnProjectileToPool(this);
	}
}

void AParadarkProjectile::SetLifeTimeProjectile()
{
	LifeTime = DefaultLifeTime;
}

void AParadarkProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LifeTime > 0.0f)
	{
		LifeTime -= DeltaTime;
	}
	else
	{
		if (PoolManager)
		{
			PoolManager->ReturnProjectileToPool(this);
		}
	}
}