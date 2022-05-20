// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include <DeskSimV2/Editor/UI/ScenarioWidget.h>
#include "Kismet/GameplayStatics.h"
#include "UObject/NameTypes.h"

/**
 * @brief Setting all the members in the scenario widget and setting a button dynamic.
 * 
 * @param Name - The name of the Scenario
 * @param Description - Description of the scenario.
 * @param MapName - The FName reference of the level
 */
void UScenarioWidget::SetMembers(FString Name, FString Description, FName MapName)
{
	TitleText->SetText(FText::FromString(Name));
	DescriptionText->SetText(FText::FromString(Description));
	MapNameReference = MapName;
	// Main Menu mode as standard
	bMenuMode = true;

	if(ScenarioButton) ScenarioButton->OnClicked.AddDynamic(this, &UScenarioWidget::ChangeToLevel);
}

/**
 * @brief Changes the level based on if the menu is in main menu, or editor mode and sends the gamemode as a parameter.
 * 
 */
void UScenarioWidget::ChangeToLevel()
{
	FString Controller;
	if (bMenuMode) Controller = "?Game=/Game/Blueprints/UI/CDerivedEditor/BP_GameplayGameModeBase.BP_GameplayGameModeBase_C?Controller=Game";
	else Controller = "?Game=/Game/Blueprints/UI/CDerivedEditor/BP_EditorGameMode.BP_EditorGameMode_C?Controller=Editor";
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, MapNameReference.ToString());
	}
	if (!MapNameReference.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), MapNameReference, true, Controller);
	}
}

/**
 * @brief Changes the menu mode.
 * 
 * 
 * @param mode - The menu mode to be set
 */
void UScenarioWidget::SetMenuMode(bool mode)
{
	bMenuMode = mode;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hello"));
}

