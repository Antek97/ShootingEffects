#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidgetPanel.generated.h"

class UAmmoBoxWidget;

UCLASS()
class PARADARK_API UAmmoWidgetPanel : public UUserWidget
{
	GENERATED_BODY()

private:
	UAmmoBoxWidget* CurrentAmmoBoxWidgets;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TArray<UAmmoBoxWidget*> AmmoBoxWidgets;
	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBoxState(UAmmoBoxWidget* AmmoBox);
};
