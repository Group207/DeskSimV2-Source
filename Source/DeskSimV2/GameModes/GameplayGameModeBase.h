// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DESKSIMV2_API AGameplayGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameplayGameModeBase();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerController")
		TSubclassOf<APlayerController> GameController;
};
