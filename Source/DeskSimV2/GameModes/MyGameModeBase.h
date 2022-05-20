// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <DeskSimV2/Editor/UI/EditorHUD.h>
#include "GameFramework/GameModeBase.h"
#include <DeskSimV2/Editor/EditorController.h>
#include "MyGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class DESKSIMV2_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public: 


	UPROPERTY(EditDefaultsOnly, Category = "MainMenuComponent")
		TSubclassOf<AEditorController> MainMenuComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerController")
		TSubclassOf<APlayerController> MainMenuController;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerController")
		TSubclassOf<APlayerController> GameController;



private:
	AMyGameModeBase();

	virtual void BeginPlay() override;
	
 
protected:
	AEditorHUD* EditorHud;
	//UPROPERTY()
	//	class UUserWidget* CurrentWidget;
};
