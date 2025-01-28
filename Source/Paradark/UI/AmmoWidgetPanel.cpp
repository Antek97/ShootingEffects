#include "AmmoWidgetPanel.h"
#include "AmmoBoxWidget.h"

//This function is used in widgets because I wanted to make debugging easier, it includes a switch statement based on tags.

void UAmmoWidgetPanel::SetBoxState(UAmmoBoxWidget* AmmoBoxWidget)
{
	if (CurrentAmmoBoxWidgets != nullptr)
	{
		CurrentAmmoBoxWidgets->DeactiveBox();
		UE_LOG(LogTemp, Warning, TEXT("Deactive animation UI"));
	}

	for (auto const AmmoBox : AmmoBoxWidgets)
	{
		if (AmmoBox == AmmoBoxWidget)
		{
			CurrentAmmoBoxWidgets = AmmoBox;
			CurrentAmmoBoxWidgets->ActiveBox();
			UE_LOG(LogTemp, Warning, TEXT("Active animation UI"));
			return;
		}
	}
}
