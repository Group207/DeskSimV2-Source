// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "GameplayGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include <DeskSimV2/Editor/UI/EditorHUD.h>

AGameplayGameModeBase::AGameplayGameModeBase()
{

}

void AGameplayGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HUDClass = AEditorHUD::StaticClass();

	PlayerControllerClass = GameController->StaticClass();	


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GamePlay Mode Base!"));
}

