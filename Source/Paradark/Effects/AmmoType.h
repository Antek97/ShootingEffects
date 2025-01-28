#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AmmoType.generated.h"

class UGameplayEffect;

UCLASS()
class PARADARK_API UAmmoType : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	FString EffectName;
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	FGameplayTag WeaponTypeTag;
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	TSubclassOf<UGameplayEffect> Effect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* Input;
};
