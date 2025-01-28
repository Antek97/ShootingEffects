#include "EnemyTarget.h"
#include "Components/WidgetComponent.h"
#include "AttributeSet.h"
#include "EnemyTargetAttribute.h"
#include "UI/EnemyTargetHelthBar.h"
#include "Components/StaticMeshComponent.h"

AEnemyTarget::AEnemyTarget()
{
	Root = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName(TEXT("TargetWidget")));
	WidgetComponent->SetupAttachment(Root);
	WidgetComponent->SetWidgetClass(UEnemyTargetHelthBar::StaticClass());

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("TargetMesh")));
	TargetMesh->SetupAttachment(Root);
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyTarget::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AttributeSet = AbilitySystemComponent->GetSet<UEnemyTargetAttribute>();
		InitializeAttributes();
	}

	if (TargetMesh)
	{
		auto Material = TargetMesh->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
		TargetMesh->SetMaterial(0, DynamicMaterial);
	}
	ResetColorTargetToDefault();
}

void AEnemyTarget::InitializeAttributes()
{
	if (AbilitySystemComponent)
	{
		if (ensure(AttributeSet))
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AttributeSet->GetHealthAttribute())
				.AddUObject(this, &AEnemyTarget::OnHealthChanged);
		}
		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(
			this,
			&AEnemyTarget::OnGameplayEffectApplied
		);
	}
}

void AEnemyTarget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	ensure(AttributeSet);
	
	const float CurrentHealth = Data.NewValue;
	const float MaxHealth = AttributeSet->GetMaxHealth();
	UpdateHealth(CurrentHealth, MaxHealth);
}

void AEnemyTarget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (WidgetComponent)
	{
		if (!WidgetHealthBar)
		{
			WidgetHealthBar = Cast<UEnemyTargetHelthBar>(WidgetComponent->GetUserWidgetObject());
		}

		if (WidgetHealthBar)
		{
			WidgetHealthBar->UpdateHealth(CurrentHealth, MaxHealth);
		}
	}
}

void AEnemyTarget::ChangeColorTarget(FLinearColor NewColor)
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("ColorBase", NewColor);
	}
}

void AEnemyTarget::ResetColorTargetToDefault()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("ColorBase", DefaultColor);
	}
}

void AEnemyTarget::OnGameplayEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle Handle)
{
	UE_LOG(LogTemp, Warning, TEXT("Start effect on target"));
}

