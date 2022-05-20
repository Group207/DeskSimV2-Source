// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "TopBar.h"
#include "Kismet/GameplayStatics.h"


UTopBar::UTopBar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	WantToQuit = false;
}

/**
 * @brief Leaves editor mode and changes level to main menu.
 * 
 */
void UTopBar::ChangeToMainMenu()
{
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!WantToQuit)
	{
		if (PopupWidgetClass)
		{

			Popup = CreateWidget<UPopUpWidget>(PC, PopupWidgetClass);
			if (Popup)
			{
				Popup->SetPositionInViewport(FVector2D(500, 500));
				Popup->AddToViewport();
				Popup->SetPopUpType(EPopupType::Warning, FString("Have you remembered to save your changes?"));
				Popup->SetVisibility(ESlateVisibility::Visible);
				PC->SetPause(true);

				Popup->PopupOkButton->OnClicked.AddDynamic(this, &UTopBar::OkClicked);
			}
		}
	}
	else { UGameplayStatics::OpenLevel(GetWorld(), "MainMenu_2"); }
	
	WantToQuit = true;
}

/**
 * @brief Removes the popup message.
 * 
 */
void UTopBar::OkClicked()
{
	Popup->PopupOkButton->OnClicked.RemoveDynamic(this, &UTopBar::OkClicked);
	PC->SetPause(false);
	Popup->RemoveFromParent();
	
	
}
