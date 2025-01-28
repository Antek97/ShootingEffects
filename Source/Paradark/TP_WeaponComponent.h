#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Effects/AmmoType.h"
#include "TP_WeaponComponent.generated.h"

class AParadarkCharacter;
class AProjectilePoolManager;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PARADARK_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AParadarkProjectile> AmmoType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FGameplayTag WeaponTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class UGameplayEffect> WeaponEffect;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere)
	AProjectilePoolManager* PoolManager;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class UMainWeaponWidget> WeaponWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UMainWeaponWidget* WeaponWidgetInstance;
	
public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AParadarkProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<UAmmoType*> AmmoTypes;
	
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(AParadarkCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	void ChangeAmmoType(const UAmmoType& AmmoType);

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AParadarkCharacter* Character;
};
