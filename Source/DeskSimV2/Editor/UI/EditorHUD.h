// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include <DeskSimV2/Signals/BasicSignal.h>
#include "Engine/Texture2D.h"
#include "TopBar.h"
#include <DeskSimV2/Editor/EditorController.h>
#include "EditorWidget.h"
#include "ObjectWidget.h"
#include "MainMenuWidget.h"
#include "ScenarioWidget.h"
#include "PauseMenu.h"
#include "settings.h"
#include "TrainDMI.h"
#include <DeskSimV2/UI/LoginWidget.h>
#include <DeskSimV2/GameModes/DesksimGameInstance.h>
#include <DeskSimV2/SaveGame/SaveManager.h>
#include "GameFramework/HUD.h"
#include "EditorHUD.generated.h"

UENUM()
enum class ECategoryBC
{ // Content Browser enum
	Train UMETA(DisplayName = "Train"),
	Railway UMETA(DisplayName = "Railway"),
	House UMETA(DisplayName = "House"),
	Signal UMETA(DisplayName = "Signal"),
	Other UMETA(DisplayName = "Other"),
};

UENUM()
enum class ECategoryMM
{ // Main Menu enum
	ATC			  UMETA(DisplayName = "ATC"),
	ERTMS		  UMETA(DisplayName = "ERTMS"),
	INFRASTRUKTUR UMETA(DisplayName = "INFRASTRUKTUR"),
	FEIL		  UMETA(DisplayName = "FEIL"),
	SKIFTING	  UMETA(DisplayName = "SKIFTING"),
	EKSTERN		  UMETA(DisplayName = "EKSTERN"),
};

USTRUCT(BlueprintType)
struct FCBObjectStruct
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	ECategoryBC Category = ECategoryBC::Train;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	UTexture2D* Image = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	TSubclassOf<AActor> ActorReference;
};

USTRUCT(BlueprintType)
struct FMMObjectStruct
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	ECategoryMM Category = ECategoryMM::ATC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	FName MapName;
};


struct FObjects
{
	FString Category;
	FString Name;
	FString Description;
	FString Image;
	TSubclassOf<AActor> ActorReference;

};

/**
 *
 */
UCLASS()
class DESKSIMV2_API AEditorHUD : public AHUD
{
	GENERATED_BODY()

public:
	AEditorHUD();
	virtual void DrawHUD()   override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void SetupEditorWidget();
	void SetupMainMenuWidget();
	void SetupLoginMenuWidget();
	void SetupGameWidget();
	void SetupPauseMenu();


	void CreateAndPopulateBrowserGrid(UEditorWidget* widget);
	void CreateAndPopulateMainMenu(UMainMenuWidget* widget);

	// The config
	TArray<FString> ConfigTrain; ///<Hardcoded Object code
	TArray<FObjects> TrainBrowserContent; // Allocated object information in FObject struct

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	TArray<FCBObjectStruct> CBObjects; ///< All objects

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EditorHUD")
	TArray<FMMObjectStruct> MMObjects; ///< All objects

	// User widgets

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> EditorWidgetClass; ///< The editor widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> ObjectWidgetClass; ///< The object widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> TopBarWidgetClass; ///< The top bar widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass; ///< The main menu widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> LoginMenuWidgetClass; ///< The login menu widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> ScenarioWidgetClass; ///< The scenario widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> SettingsWidgetClass; ///< The settings widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> TrainDMIWidgetClass; ///< The TrainDMI widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass; ///< The TrainDMI widget class

	// PlayerControllers

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Controller")
	TSubclassOf<APlayerController> MainMenuController; ///< The settings widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Controller")
	TSubclassOf<APlayerController> EditGameController; ///< The settings widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Controller")
	TSubclassOf<APlayerController> GameController; ///< The settings widget class

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Actor")
	TSubclassOf<AEditorController> EditorControllerClass;

	// Savemanager

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save Manager")
	ASaveManager* SaveManager;

	// Functions dynamics bound to buttons

	UFUNCTION()
	void UpdateTrainButtonClick();

	UFUNCTION()
	void UpdateRailwayButtonClick();

	UFUNCTION()
	void UpdateHouseButtonClick();

	UFUNCTION()
	void UpdateSignalButtonClick();

	UFUNCTION()
	void UpdateOtherButtonClick();

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void SetToolTranslation();

	UFUNCTION()
	void SetToolRotation();

	UFUNCTION()
	void DeleteObject();


	UFUNCTION()
		void ChangeMenuMode();
	UFUNCTION()
	void OpenSettings();

	// Updating DMI 
	void SetTrainDMI(float Speed);
	void HideTrainDMI();
	void ShowTrainDMI();

private:
	APlayerController* PC; ///< The player controller

	AEditorController* EditorController; ///< The Editor Controller that controlls the player input

	bool bMenuMode = true;

	// WIDGETS
	UTopBar* TopBarWidget;						///< The top bar widget 
	UEditorWidget* EditorWidget;				///< The editor widget (Content Browser)
	ULoginWidget* LoginWidget;					///< The login widget
	UMainMenuWidget* MainMenuWidget;		    ///< The main menu widget
	TArray<UScenarioWidget*> ScenarioWidgets;	///< All widgets of the different scenarios spawned in main menu.
	TArray<UObjectWidget*> ObjectWidgets;		///< All widgets that can be switched between in the content browser
	UTrainDMI* TrainDMIWidget;					///< The train DMI widget
	Usettings* SettingsWidget;					///< The setting widget
	UPauseMenu* PauseMenuWidget;				///< The pause menu widget

	TArray<TSubclassOf<AActor>> ActorSources;	///< List of all actors that can be placed
	TArray<UTexture2D*> ImageSources;			///< List of all images for objects

	EToolMode ControllerToolMode;

};
