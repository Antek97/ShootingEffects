#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "Paradark/EnemyTarget.h"
#include "EffectDrivenColorCue.generated.h"

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class PARADARK_API UEffectDrivenColorCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor EffectColor;

	mutable bool bApplied = false;
private:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

};
