// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "EditorHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h" // Drag and Drop
#include "Kismet/GameplayStatics.h"
#include "settings.h"
#include "Components/UniformGridPanel.h"
#include <DeskSimV2/GameModes/MyGameModeBase.h>


/**
 * @brief Constructs all images and adds them to an array and initialzes the hard coded config for objects.
 *  
 */
AEditorHUD::AEditorHUD()
{	

}

/**
 * @brief. Function for drawing the HUD
 * 
 */
void AEditorHUD::DrawHUD()
{
	Super::DrawHUD();
}

/**
 * @brief Finds the player controller, finds all objects, created the content browser and adds dynamic functions to
 * the editorwidget's buttons.
 * 
 * @see CreateAndPopulateGrid()
 */
void AEditorHUD::BeginPlay()
{
	Super::BeginPlay();

	// Initializing the save manager and loading the current game
	SaveManager = NewObject<ASaveManager>(this, ASaveManager::StaticClass(), FName("SaveManager"));
	LoadGame();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FInputModeUIOnly InputUIOnly;
		//PC->SetInputMode(InputUIOnly);
		// Set player controller to show cursor and read mouse events
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;

	}
	else
	{
		return;
	}

	UDesksimGameInstance* GameInstance = Cast<UDesksimGameInstance>(GetGameInstance());

	
	if (!GameInstance->bIsLoggedIn)
	{
		SetupLoginMenuWidget();
	}
	else
	{

		FString Options = UGameplayStatics::GetGameMode(GetWorld())->OptionsString;

		FString Controller = UGameplayStatics::ParseOption(Options, FString("Controller"));

		if (Controller.IsEmpty()) Controller = FString("Menu");

		if (Controller.Equals(FString("Editor")))
		{


			FVector SpawnPosition = FVector(0.0f, 0.0f, 0.0f);
			FRotator StartRotation = FRotator(1, 0, 0);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			EditorController = GetWorld()->SpawnActor<AEditorController>(AEditorController::StaticClass(), SpawnPosition, StartRotation, SpawnParams);

			SetupEditorWidget();

			if (EditorController)
			{
				PC->Possess(EditorController);
			}
		}
		else if (Controller.Equals(FString("Menu")))
		{
			SetupMainMenuWidget();
		}
		else if (Controller.Equals(FString("Game")))
		{
			SetupGameWidget();
		}
	}
	
}

// Tick function
void AEditorHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

/**
 * @brief Setting up the widget for the editor mode.
 * 
 */
void AEditorHUD::SetupEditorWidget()
{
	if (TopBarWidgetClass)
	{
		TopBarWidget = CreateWidget<UTopBar>(PC, TopBarWidgetClass);
		FVector2D TopBarPosition = FVector2D(0, 0); // should be on the top
		TopBarWidget->SetPositionInViewport(TopBarPosition);
		if (TopBarWidget)
		{
			TopBarWidget->AddToViewport();
			if (TopBarWidget->SaveButton)
			{
				TopBarWidget->SaveButton->OnClicked.AddDynamic(this, &AEditorHUD::SaveGame);
			}
			if (TopBarWidget->MainMenuButton)
			{
				TopBarWidget->MainMenuButton->OnClicked.AddDynamic(TopBarWidget, &UTopBar::ChangeToMainMenu);
			}
			// Setting up the editorcontroller functionality
			if (TopBarWidget->ScaleButton)
			{
				// Future Scaling
			}
			if (TopBarWidget->TranslateButton)
			{
				TopBarWidget->TranslateButton->OnClicked.AddDynamic(this, &AEditorHUD::SetToolTranslation);

			}
			if (TopBarWidget->RotateButton)
			{
				TopBarWidget->RotateButton->OnClicked.AddDynamic(this, &AEditorHUD::SetToolRotation);
			}
			if (TopBarWidget->TrashButton)
			{
				TopBarWidget->TrashButton->OnClicked.AddDynamic(this, &AEditorHUD::DeleteObject);
			}

		}
	}
	// Creating the Editor widget
	if (EditorWidgetClass)
	{
		EditorWidget = CreateWidget<UEditorWidget>(PC, EditorWidgetClass); // Creating the Widget
		//FVector2D EditorPosition = FVector2D(35, 60);
		//EditorWidget->SetPositionInViewport(EditorPosition); // Placing the widget in the viewport

		if (EditorWidget)
		{
			EditorWidget->AddToViewport(); // Adding the content browser in the viewport

			// Setting the dynamics for the switch between each category view.
			if (EditorWidget->TrainButton)
			{
				EditorWidget->TrainButton->OnClicked.AddDynamic(this, &AEditorHUD::UpdateTrainButtonClick);
			}
			if (EditorWidget->RailwayButton)
			{
				EditorWidget->RailwayButton->OnClicked.AddDynamic(this, &AEditorHUD::UpdateRailwayButtonClick);
			}
			if (EditorWidget->HouseButton)
			{
				EditorWidget->HouseButton->OnClicked.AddDynamic(this, &AEditorHUD::UpdateHouseButtonClick);
			}
			if (EditorWidget->SignalButton)
			{
				EditorWidget->SignalButton->OnClicked.AddDynamic(this, &AEditorHUD::UpdateSignalButtonClick);
			}
			if (EditorWidget->OtherButton)
			{
				EditorWidget->OtherButton->OnClicked.AddDynamic(this, &AEditorHUD::UpdateOtherButtonClick);
			}

			CreateAndPopulateBrowserGrid(EditorWidget);

		}
	}
}

/**
 * @brief Setting up the Main Menu widget.
 * 
 */
void AEditorHUD::SetupMainMenuWidget()
{
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass); // Creating the Widget

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport(); // Adding the content browser in the viewport

			MainMenuWidget->SetButtons();
			// Setting the dynamics for the switch between each category view.
			MainMenuWidget->EditorBtn->OnClicked.AddDynamic(this, &AEditorHUD::ChangeMenuMode);
			MainMenuWidget->MainMenuBtn->OnClicked.AddDynamic(this, &AEditorHUD::ChangeMenuMode);
			MainMenuWidget->SettingsBtn->OnClicked.AddDynamic(this, &AEditorHUD::OpenSettings);

			CreateAndPopulateMainMenu(MainMenuWidget);
		}
	}
}

void AEditorHUD::SetupLoginMenuWidget()
{
	if (LoginMenuWidgetClass)
	{
		LoginWidget = CreateWidget<ULoginWidget>(PC, LoginMenuWidgetClass);

		if (LoginWidget)
		{
			LoginWidget->AddToViewport();
		}
	}
}

void AEditorHUD::SetupGameWidget()
{
	if (TrainDMIWidgetClass)
	{
		TrainDMIWidget = CreateWidget<UTrainDMI>(PC, TrainDMIWidgetClass);

		if (TrainDMIWidget)
		{
			TrainDMIWidget->SetPositionInViewport(FVector2D(0, 30));
			TrainDMIWidget->AddToViewport();
		}
	}
}

/**
 * @brief Settign up the Pause Menu widget.
 * 
 */
void AEditorHUD::SetupPauseMenu()
{

	// If there is no pause menu created
	if (PauseMenuWidgetClass && !PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UPauseMenu>(PC, PauseMenuWidgetClass);
		if (PauseMenuWidget)
		{
			PauseMenuWidget->PauseGame();
			PauseMenuWidget->AddToViewport();
			PauseMenuWidget->SetupButtons();

			// Settings Button called from this class
			if (PauseMenuWidget->SettingsButton) PauseMenuWidget->SettingsButton->OnClicked.AddDynamic(this, &AEditorHUD::OpenSettings);
			
			return;
		}
	}

	// If the pause menu is already in the viewport
	if (PauseMenuWidget)
	{
		if (PauseMenuWidget->IsInViewport())
		{
			PauseMenuWidget->OnResumePressed();
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, (TEXT("Resuming game")));
			}
		}
		else
		{
			PauseMenuWidget->PauseGame();
			PauseMenuWidget->AddToViewport();
		}
		
		
		
	}

}

/**
 * @brief Changes the menu mode between Main Menu and Editor Menu.
 * 
 */
void AEditorHUD::ChangeMenuMode()
{
	bMenuMode = !bMenuMode;

	// Changing the options for the Menu scenario
	for (int i = 0; i < ScenarioWidgets.Num(); i++)
	{
		ScenarioWidgets[i]->SetMenuMode(bMenuMode);
	}
	// Changes layout for the menu
	MainMenuWidget->ChangeLayout(bMenuMode);
}


/**
 * @brief Creating and displaying the Settings widget.
 * 
 */
void AEditorHUD::OpenSettings()
{
	// If a setting widget is present
	if (SettingsWidget != nullptr) {
		SettingsWidget->AddToViewport();
	 }else 
		if (SettingsWidgetClass)
	    {
			SettingsWidget = CreateWidget<Usettings>(PC, SettingsWidgetClass);
		
			if (SettingsWidget)
			{
				SettingsWidget->AddToViewport();
				SettingsWidget->SetupSettings();

			}
		}
}

/**
 * @brief Updates the train DMI.
 * 
 * @param Speed - Current speed of the train.
 */
void AEditorHUD::SetTrainDMI(float Speed)
{
	if (TrainDMIWidget)
	{
		TrainDMIWidget->Update(Speed);
	}
}

/**
 * @brief Calls the Hide() function in TrainDMI.cpp to remove the DMI from the viewport.
 * 
 * @see TrainDMI::Hide()
 */
void AEditorHUD::HideTrainDMI()
{
	if (TrainDMIWidget)
	{
		TrainDMIWidget->Hide();
	}
}

/**
 * @brief Adds the Train DMI to the viewport.
 * 
 */
void AEditorHUD::ShowTrainDMI()
{
	if (TrainDMIWidget)
	{
		TrainDMIWidget->AddToViewport();
	}
}


/**
 * @brief Creates each of the object widgets which is part of the uniform grid that fills the content browser.
 * 
 * @param widget - The editor widget.
 */
void AEditorHUD::CreateAndPopulateBrowserGrid(UEditorWidget* widget)
{
	TArray<UUniformGridPanel*> Panels;

	TArray<UWidget*> Widgets = widget->ObjectSwitcher->GetAllChildren();

	// Casting to Uniformgidpanel
	for (int i = 0; i < Widgets.Num(); i++)
	{
		Panels.Add(Cast<UUniformGridPanel>(Widgets[i]));
	}

	int PanelCount = 0;
	uint8 ObjectCount[5] = {0};

	for (int i = 0; i < CBObjects.Num(); i++)
	{
		uint8 CurrentCategory = (uint8)CBObjects[i].Category;

		// Assigning each of the widget information
		UObjectWidget* CreatedWidget = CreateWidget<UObjectWidget>(PC, ObjectWidgetClass);
		CreatedWidget->SetTitleText(CBObjects[i].Name);
		CreatedWidget->SetDescriptionText(CBObjects[i].Description);
		CreatedWidget->SetImageResource(CBObjects[i].Image);
		CreatedWidget->SetActorResource(CBObjects[i].ActorReference);

		uint16 GridColoumns = 3;
		
		Panels[CurrentCategory]->AddChildToUniformGrid(CreatedWidget, ObjectCount[CurrentCategory] / 3, ObjectCount[CurrentCategory] % 3);

		if (i != 0 && i + 1 % 3 == 0)
		{
			widget->ForceLayoutPrepass();
		}
		ObjectCount[CurrentCategory] += 1;
	}
}

/**
 * @brief Fills the main menu widget with scenario widgets.
 * 
 * @param widget - The main menu widget
 */
void AEditorHUD::CreateAndPopulateMainMenu(UMainMenuWidget* widget)
{
	TArray<UUniformGridPanel*> Panels;

	TArray<UWidget*> Widgets = widget->ScenarioSwitcher->GetAllChildren();

	// Casting to Uniformgidpanel
	for (int i = 0; i < Widgets.Num(); i++)
	{
		Panels.Add(Cast<UUniformGridPanel>(Widgets[i]));
	}

	int PanelCount = 0;
	//uint16 ObjectCount = 0;
	uint8 ObjectCount[6] = { 0 };

	for (int i = 0; i < MMObjects.Num(); i++)
	{
		uint8 CurrentCategory = (uint8)MMObjects[i].Category;

		UScenarioWidget* CreatedWidget = CreateWidget<UScenarioWidget>(PC, ScenarioWidgetClass);

		FName temp = MMObjects[i].MapName;

		// MMObjects are the user defined struct of levels.
		CreatedWidget->SetMembers(MMObjects[i].Name, MMObjects[i].Description, MMObjects[i].MapName);


		uint16 GridColoumns = 3;

		Panels[CurrentCategory]->AddChildToUniformGrid(CreatedWidget, ObjectCount[CurrentCategory] / 3, ObjectCount[CurrentCategory] % 3);

		if (i != 0 && i + 1 % 3 == 0)
		{
			widget->ForceLayoutPrepass();
		}
		ObjectCount[CurrentCategory] += 1;
		ScenarioWidgets.Add(CreatedWidget);
	}
}

// Dynamic button functions
void AEditorHUD::UpdateTrainButtonClick()
{
	EditorWidget->TrainBtnOnClicked();
	TopBarWidget->WantToQuit = false;
}
void AEditorHUD::UpdateRailwayButtonClick()
{
	EditorWidget->RailwayBtnOnClicked();
	TopBarWidget->WantToQuit = false;
}
void AEditorHUD::UpdateHouseButtonClick()
{
	EditorWidget->HouseBtnOnClicked();
	TopBarWidget->WantToQuit = false;
}
void AEditorHUD::UpdateSignalButtonClick()
{
	EditorWidget->SignalBtnOnClicked();
	TopBarWidget->WantToQuit = false;
}
void AEditorHUD::UpdateOtherButtonClick()
{
	EditorWidget->OtherBtnOnClicked();
	TopBarWidget->WantToQuit = false;
}

/**
 * @brief Calls on editorController to delete an object.
 * 
 * @see AEditorcontroller::DeleteSelectedObjects()
 */
void AEditorHUD::DeleteObject()
{
	if (EditorController != nullptr)
	{
		EditorController->DeleteSelectedObject();
	}
}

/**
 * @brief Saves the current game changes.
 * 
 * @see ASaveManager::SaveGame()
 */
void AEditorHUD::SaveGame()
{
	if (SaveManager)
	{
		SaveManager->SaveGame();
	}
}

/**
 * @brief Loads the game.
 * @see ASaveManager::LoadGame()
 */
void AEditorHUD::LoadGame()
{
	if (SaveManager)
	{
		SaveManager->LoadGame();
	}
}


/**
 * @brief Changes the tool mode in the Editor to translation.
 * @see EditorController::SetToolMode()
 */
void AEditorHUD::SetToolTranslation()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Trans!"));
	EditorController->SetToolMode<EToolMode::Translation>();
}

/**
 * @brief Changes the tool mode in the Editor to rotation.
 * 
 * @see EditorController::SetToolMode()
 */
void AEditorHUD::SetToolRotation()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Rot!"));
	EditorController->SetToolMode<EToolMode::Rotation>();
}


