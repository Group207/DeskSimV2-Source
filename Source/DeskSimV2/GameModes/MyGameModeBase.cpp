// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"



AMyGameModeBase::AMyGameModeBase()
{
}


void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// Spawning the editor mode component

	FString Controller = UGameplayStatics::ParseOption(OptionsString, FString("Controller"));


	PlayerControllerClass = MainMenuController->StaticClass();
	DefaultPawnClass = MainMenuComponent->StaticClass();

	if (Controller.Equals(FString("Game")))
	{
		PlayerControllerClass = GameController;
	}

	HUDClass = AEditorHUD::StaticClass();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Main menu game mode!"));
}
