#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "EnemyTargetAttribute.h"
#include "EnemyTarget.generated.h"

class UWidgetComponent;
class UEnemyTargetHelthBar;
class UAbilitySystemComponent;
class UMaterialInstanceDynamic;

UCLASS()
class PARADARK_API AEnemyTarget : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AEnemyTarget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* TargetMesh;

	UEnemyTargetHelthBar* WidgetHealthBar;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<const class UEnemyTargetAttribute> AttributeSet;

	virtual void BeginPlay() override;
	virtual void InitializeAttributes(); 
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void OnGameplayEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle Handle);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor DefaultColor;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeColorTarget(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable)
	void ResetColorTargetToDefault();

private:
	UMaterialInstanceDynamic* DynamicMaterial;
	
};