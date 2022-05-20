// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#include <DeskSimV2/Editor/UI/MainMenuWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <DeskSimV2/Editor/UI/EditorHUD.h>


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

/**
 * @brief Function for setting up native class members.
 * 
 */
void UMainMenuWidget::NativeOnInitialized()
{
	QuitBtn->SetVisibility(ESlateVisibility::Visible);
}

/**
 * @brief.Addign a function dynamic to all buttons
 * 
 */
void UMainMenuWidget::SetButtons()
{

	if (QuitBtn) QuitBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);

	if (LogoutBtn) LogoutBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::LogOut);

	// Setting buttons for main menu
	if (ATCBtn) ATCBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::ATCClicked);
	
	if (ERTMSBtn) ERTMSBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::ERTMSClicked);
	
	if (INFRBtn) INFRBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::INFRClicked);
	
	if (FEILBtn) FEILBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::FEILClicked);

	if (SKIFBtn) ERTMSBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::SKIFClicked);
	
	if (EKSTBtn) EKSTBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::EKSTClicked);
}

/**
 * @brief Quit's the game.
 * 
 */
void UMainMenuWidget::QuitGame()
{
	
	UWorld* World = GetWorld();

	UKismetSystemLibrary::QuitGame(World, UGameplayStatics::GetPlayerController(World, 0), EQuitPreference::Quit, false);
}

/**
 * @brief Logs the user out of the game.
 * 
 */
void UMainMenuWidget::LogOut()
{
	RemoveFromParent();

	UDesksimGameInstance* GameInstance = Cast<UDesksimGameInstance>(GetGameInstance());

	GameInstance->bIsLoggedIn = false;

	UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AEditorHUD>()->SetupLoginMenuWidget();
}

/**
 * @brief Function for changing between Editor menu and Main menu.
 * 
 * @param mode - true = Editormode | false = MainMenuMode
 */
void UMainMenuWidget::ChangeLayout(bool mode)
{
	if (mode)
	{
		MainMenuBtn->SetVisibility(ESlateVisibility::Hidden);
		EditorBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		MainMenuBtn->SetVisibility(ESlateVisibility::Visible);
		EditorBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}


/**
 * @brief Changes the active widget in the menu to ATC.
 * 
 */
void UMainMenuWidget::ATCClicked()
{
	if (ScenarioSwitcher) ScenarioSwitcher->SetActiveWidgetIndex(0);
}

/**
 * @brief Changes the active widget in the menu to ERTMS.
 *
 */
void UMainMenuWidget::ERTMSClicked()
{
	if (ScenarioSwitcher) ScenarioSwitcher->SetActiveWidgetIndex(1);
}

/**
 * @brief Changes the active widget in the menu to INFRASTRUKTUR.
 *
 */
void UMainMenuWidget::INFRClicked()
{
	if (ScenarioSwitcher) ScenarioSwitcher->SetActiveWidgetIndex(2);
}

/**
 * @brief Changes the active widget in the menu to FEIL.
 *
 */
void UMainMenuWidget::FEILClicked()
{
	if (ScenarioSwitcher) ScenarioSwitcher->SetActiveWidgetIndex(3);
}

/**
 * @brief Changes the active widget in the menu to SKIFTING.
 *
 */
void UMainMenuWidget::SKIFClicked()
{
	if (ScenarioSwitcher) ScenarioSwitcher->SetActiveWidgetIndex(4);
}

/**
 * @brief Changes the active widget in the menu to EKSTERN.
 *
 */
void UMainMenuWidget::EKSTClicked()
{
	if (ScenarioSwitcher) ScenarioSwitcher->SetActiveWidgetIndex(5);
}
