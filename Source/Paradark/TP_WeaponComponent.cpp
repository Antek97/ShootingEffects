// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_WeaponComponent.h"
#include "ParadarkCharacter.h"
#include "ParadarkProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/MainWeaponWidget.h"
#include "ProjectilePoolManager.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffect.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//PoolManager = GetWorld()->SpawnActor<AProjectilePoolManager>();
}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile

	if (ensure(ProjectileClass))
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		ensure(PoolManager);
		AParadarkProjectile* Projectile = PoolManager->GetProjectile(ProjectileClass, SpawnLocation, SpawnRotation);

		Projectile->PoolManager = PoolManager;
		Projectile->AmmoEffect = WeaponEffect;
				
		//If we want to expand the weapon, we can move the shot to the gameplay ability and enhance it with additional tags.
			
		UE_LOG(LogTemp, Warning, TEXT("Shoot with a gun"));
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::ChangeAmmoType(const UAmmoType& NewAmmoType)
{
	if (WeaponWidgetInstance)
	{
		WeaponWidgetInstance->ChangeAmmo(NewAmmoType.WeaponTypeTag);
	}
	ensure(NewAmmoType.Effect);
	WeaponEffect = NewAmmoType.Effect;

}

bool UTP_WeaponComponent::AttachWeapon(AParadarkCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and- has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	AbilitySystemComponent = Character->FindComponentByClass<UAbilitySystemComponent>();

	if (WeaponWidget)
	{
		UMainWeaponWidget* WidgetInstance = CreateWidget<UMainWeaponWidget>(GetWorld(), WeaponWidget);

		if (WidgetInstance)
		{
			WeaponWidgetInstance = WidgetInstance;
			WidgetInstance->AddToViewport();
		}
	}

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			for (auto const & CurrentAmmoType : AmmoTypes)
			{
				ensure(CurrentAmmoType->Input);

				EnhancedInputComponent->BindActionValueLambda(
					CurrentAmmoType->Input,
					ETriggerEvent::Triggered, 
					[this, CurrentAmmoType] (const FInputActionValue& InputActionValue)
					{ 
						ensure(CurrentAmmoType);
						ChangeAmmoType(*CurrentAmmoType);
					});
			}
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
	ChangeAmmoType(*AmmoTypes[0]);

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}