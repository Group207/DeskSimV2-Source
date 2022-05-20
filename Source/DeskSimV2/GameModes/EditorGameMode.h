// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <DeskSimV2/Editor/EditorController.h>
#include "GameFramework/GameModeBase.h"
#include "EditorGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DESKSIMV2_API AEditorGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditDefaultsOnly, Category = "EditModeComponent")
			TSubclassOf<AEditorController> EditModeComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerController")
			TSubclassOf<APlayerController> EditorControllerClass;


	AEditorGameMode();

	virtual void BeginPlay() override;
};
