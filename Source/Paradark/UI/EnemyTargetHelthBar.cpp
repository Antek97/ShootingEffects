#include "EnemyTargetHelthBar.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

void UEnemyTargetHelthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (CurrentHealth > 0)
	{
		float HealthPercentage = CurrentHealth / MaxHealth;
		UpdateHealthBar(HealthPercentage);
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthPercentage);
	}
	else
	{
		UpdateHealthBar(0.0f);
		UE_LOG(LogTemp, Warning, TEXT("Health: 0"));
	}
}