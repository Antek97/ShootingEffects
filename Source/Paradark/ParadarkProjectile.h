// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "ParadarkProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AProjectilePoolManager;

UCLASS(config=Game)
class AParadarkProjectile : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

public:
	AParadarkProjectile();

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> AmmoEffect;

	AProjectilePoolManager* PoolManager;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void SetLifeTimeProjectile();
	void Tick(float DeltaTime);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DefaultLifeTime{ 1 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LifeTime { DefaultLifeTime };

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

};

