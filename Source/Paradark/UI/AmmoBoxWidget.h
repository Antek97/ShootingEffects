#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoBoxWidget.generated.h"

UCLASS()
class PARADARK_API UAmmoBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString KeyBindName;

	UFUNCTION(BlueprintImplementableEvent)
	void ActiveBox();
	UFUNCTION(BlueprintImplementableEvent)
	void DeactiveBox();
};
