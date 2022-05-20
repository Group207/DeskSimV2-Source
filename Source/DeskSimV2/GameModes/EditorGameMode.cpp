// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "EditorGameMode.h"
#include "Kismet/GameplayStatics.h"
#include <DeskSimV2/Editor/UI/EditorHUD.h>
#include "Containers/UnrealString.h"



AEditorGameMode::AEditorGameMode()
{

}


void AEditorGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Spawning the editor mode component


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Editor game mode!"));
	}
}