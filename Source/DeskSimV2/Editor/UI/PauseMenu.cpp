// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include <DeskSimV2/Editor/UI/PauseMenu.h>
#include "Kismet/GameplayStatics.h"

/**
 * @brief Sets up the dynamic button functionality.
 * 
 */
void UPauseMenu::SetupButtons()
{
	if (ResumeButton) ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumePressed);

	if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UPauseMenu::OnExitPressed);
}

/**
 * @brief Pauses the game.
 * 
 */
void UPauseMenu::PauseGame()
{
	UGameplayStatics::SetGamePaused(this, true);
}

/**
 * @brief Changes the level to main menu.
 * 
 */
void UPauseMenu::OnExitPressed()
{
	// The main menu map reference FName
	FName MapName = FName("MainMenu_2");

	if (!MapName.IsNone())
	{
		// Removes widget
		RemoveFromParent();

		// Opens main menu
		UGameplayStatics::OpenLevel(GetWorld(), MapName);
	}
}

/**
 * @brief Resumes the game, and removes the widget from the viewport.
 * 
 */
void UPauseMenu::OnResumePressed()
{
	UGameplayStatics::SetGamePaused(this, false);
	RemoveFromParent();
}