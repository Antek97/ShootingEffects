#include "ProjectilePoolManager.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectilePoolManager::AProjectilePoolManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AProjectilePoolManager::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectilePoolManager::InitializePool(TSubclassOf<AParadarkProjectile> ProjectileClass)
{
    if (!ProjectileClass) return;

    FProjectilePool& Pool = ProjectilePools.FindOrAdd(ProjectileClass);

    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < PoolSize; i++)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        AParadarkProjectile* Projectile = World->SpawnActor<AParadarkProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if (Projectile)
        {
            Projectile->SetActorHiddenInGame(true);
            Projectile->SetActorEnableCollision(false);
            Pool.Projectiles.Add(Projectile);
            //UE_LOG(LogCombat, Log, TEXT("Hit target"));
        }
    }
}

AParadarkProjectile* AProjectilePoolManager::GetProjectile(TSubclassOf<AParadarkProjectile> ProjectileClass, const FVector& Location, const FRotator& Rotation)
{
    if (!ProjectileClass) return nullptr;

    if (!ProjectilePools.Contains(ProjectileClass))
    {
        InitializePool(ProjectileClass);
    }

    FProjectilePool& Pool = ProjectilePools[ProjectileClass];
    Pool.Projectiles.Remove(nullptr);

    for (AParadarkProjectile* Projectile : Pool.Projectiles)
    {
        if (Projectile && Projectile->IsHidden())
        {
            if (UProjectileMovementComponent* ProjectileMovement = Projectile->ProjectileMovement)
            {
                ProjectileMovement->bRotationFollowsVelocity = true;
                ProjectileMovement->Velocity = Rotation.Vector() * ProjectileMovement->InitialSpeed;
                ProjectileMovement->SetActive(true);
            }

            Projectile->SetActorLocation(Location);
            Projectile->SetActorRotation(Rotation);
            Projectile->SetActorHiddenInGame(false);
            Projectile->SetActorEnableCollision(true);

            Projectile->SetLifeTimeProjectile();
            Projectile->SetActorTickEnabled(true);

            return Projectile;
        }
    }

    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        AParadarkProjectile* NewProjectile = World->SpawnActor<AParadarkProjectile>(ProjectileClass, Location, Rotation, SpawnParams);
        if (NewProjectile)
        {
            Pool.Projectiles.Add(NewProjectile);
            return NewProjectile;
        }
    }
    return nullptr;
}

void AProjectilePoolManager::ReturnProjectileToPool(AParadarkProjectile* Projectile)
{
    if (!Projectile) return;

    Projectile->SetActorTickEnabled(false);

    Projectile->SetActorHiddenInGame(true);
    Projectile->SetActorEnableCollision(false);
    Projectile->SetActorLocation(FVector::ZeroVector);
    Projectile->SetActorRotation(FRotator::ZeroRotator);

    if (UProjectileMovementComponent* ProjectileMovement = Projectile->ProjectileMovement)
    {
        ProjectileMovement->SetActive(false);
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->bRotationFollowsVelocity = false;
        ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
    }

    for (auto& PoolPair : ProjectilePools)
    {
        FProjectilePool& Pool = PoolPair.Value;
        if (Pool.Projectiles.Contains(Projectile))
        {
            return;
        }
    }
}
