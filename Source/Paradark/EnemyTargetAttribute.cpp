#include "EnemyTargetAttribute.h"

void UEnemyTargetAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
		const float CurrentHewalthValue = Health.GetBaseValue();

		//NewValue = FMath::Clamp<float>(NewValue, 0, 100);
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.0f);

		if (!FMath::IsNearlyEqual(CurrentHewalthValue, NewValue))
		{
			AbilityComp->ApplyModToAttributeUnsafe(Attribute, EGameplayModOp::Override, NewValue);
		}
	}
}