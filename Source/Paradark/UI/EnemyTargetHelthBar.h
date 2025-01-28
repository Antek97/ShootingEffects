#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyTargetHelthBar.generated.h"

UCLASS()
class PARADARK_API UEnemyTargetHelthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	FORCEINLINE void UpdateHealthBar(float Percent);

	UFUNCTION()
	FORCEINLINE void UpdateHealth(float CurrentHealth, float MaxHealth);
};
