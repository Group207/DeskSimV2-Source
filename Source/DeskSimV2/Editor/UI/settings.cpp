// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include <DeskSimV2/Editor/UI/settings.h>
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetStringLibrary.h"


/**
 * @brief Changes the resolution of the screen.
 * 
 * @param sItem - the resolution string in the form (num1 x num2)
 * @param seltype
 */
void Usettings::ChangeResolution(FString sItem, ESelectInfo::Type seltype)
{
	FIntPoint ChosedRes;
	FString Left, Right;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Settingsssssss!!!!"));
	}

	// Splitting the resolution string
	sItem.Split(TEXT("x"), &Left, &Right);
	
	ChosedRes.X = UKismetStringLibrary::Conv_StringToInt(Left);
	ChosedRes.Y = UKismetStringLibrary::Conv_StringToInt(Right);
	// Setting the rasolution if GEngine is found
	if (GEngine) 
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d, %d"), ChosedRes.X, ChosedRes.Y));
		}
		GEngine->GameUserSettings->SetScreenResolution(ChosedRes);
		GEngine->GameUserSettings->ApplyResolutionSettings(false);
	}
}

/**
 * @brief Changes the simulator's screen mode.
 * 
 * @param sItem - The string with the screen mode
 * @param seltype - info of the selection
 */
void Usettings::ChangeScreenMode(FString sItem, ESelectInfo::Type seltype)
{
	EWindowMode::Type Mode;

	Mode = EWindowMode::Fullscreen;

	if (sItem == FString("Fullscreen"))
	{
		Mode = EWindowMode::Fullscreen;
	}
	else if (sItem == FString("WindowedFullscreen"))
	{
		Mode = EWindowMode::WindowedFullscreen;
	}
	else if (sItem == FString("Windowed"))
	{
		Mode = EWindowMode::Windowed;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Updating fullscreen mode to: %s"), *UEnum::GetValueAsString(Mode)));
	}
	GEngine->GameUserSettings->SetFullscreenMode(Mode);
	GEngine->GameUserSettings->ApplyResolutionSettings(false);
	
}

/**
 * @brief Removes the settings window.
 */
void Usettings::CloseSettings()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Settingsssssss!!!!"));
	}
	RemoveFromParent();

}

/**
 * @brief Sets up and fills the information seen in the settings widget in the main menu.
 * The function also binds the functionality for when a different resolution or screen 
 * mode is selected in the widget. 
 * 
 * @see ChangeScreenMode()
 * @see ChangeResolution()
 * 
 */
void Usettings::SetupSettings()
{
	// Set up resolution options
	TArray<FString> Resolutions;

	Resolutions.Add(FString("1080x720"));
	Resolutions.Add(FString("1920x1080"));
	Resolutions.Add(FString("2560x1440"));

	for (int i = 0; i < Resolutions.Num(); i++)
	{
		ResolutionDropdown->AddOption(Resolutions[i]);
	}

	// Adding a dynimic function to the drop down selections
	ResolutionDropdown->OnSelectionChanged.AddDynamic(this, &Usettings::ChangeResolution);
	WindowModeDropdown->OnSelectionChanged.AddDynamic(this, &Usettings::ChangeScreenMode);

	// Set up window options
	TArray<FString> WindowMode;
	WindowMode.Add(FString("Fullscreen"));
	WindowMode.Add(FString("WindowedFullscreen"));
	WindowMode.Add(FString("Windowed"));

	for (int j = 0; j < WindowMode.Num(); j++)
	{
		WindowModeDropdown->AddOption(WindowMode[j]);
	}

	// Set button functionality 

	QuitButton->OnClicked.AddDynamic(this, &Usettings::CloseSettings);
}
