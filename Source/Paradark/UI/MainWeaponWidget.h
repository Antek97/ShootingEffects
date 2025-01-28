#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWeaponWidget.generated.h"

UCLASS()
class PARADARK_API UMainWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeAmmo(FGameplayTag GameplayTag);
};
