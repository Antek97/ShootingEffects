#include "EffectDrivenColorCue.h"

bool UEffectDrivenColorCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	bool bResult = Super::OnExecute_Implementation(MyTarget, Parameters);

	AEnemyTarget* Target = Cast<AEnemyTarget>(MyTarget);
	Target->ChangeColorTarget(EffectColor);

	return bResult;
}

bool UEffectDrivenColorCue::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	bool bResult = Super::OnRemove_Implementation(MyTarget, Parameters);

	UE_LOG(LogTemp, Warning, TEXT("Remove visual effect"));

	AEnemyTarget* Target = Cast<AEnemyTarget>(MyTarget);
	Target->ResetColorTargetToDefault();

	return bResult;
}