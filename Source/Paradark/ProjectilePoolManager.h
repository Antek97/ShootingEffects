#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParadarkProjectile.h"
#include "Containers/Map.h"
#include "UObject/Class.h"
#include "ProjectilePoolManager.generated.h"

USTRUCT(BlueprintType)
struct FProjectilePool
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<AParadarkProjectile*> Projectiles;
};

UCLASS()
class PARADARK_API AProjectilePoolManager : public AActor
{
    GENERATED_BODY()

public:
    AProjectilePoolManager();

    UFUNCTION(BlueprintCallable)
    AParadarkProjectile* GetProjectile(TSubclassOf<AParadarkProjectile> ProjectileClass, const FVector& Location, const FRotator& Rotation);

    UFUNCTION(BlueprintCallable)
    void ReturnProjectileToPool(AParadarkProjectile* Projectile);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Pool Settings")
    int32 PoolSize = 5;

    UPROPERTY(EditAnywhere)
    TMap<TSubclassOf<AParadarkProjectile>, FProjectilePool> ProjectilePools;


private:
    void InitializePool(TSubclassOf<AParadarkProjectile> ProjectileClass);
};
