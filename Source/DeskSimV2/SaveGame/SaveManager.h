// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveManager.generated.h"

UCLASS()
class DESKSIMV2_API ASaveManager : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ASaveManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void SaveGame();
	void LoadGame();

protected:
	FString LevelName; ///< Current name of the .uasset file for level
	AActor* FindActorByName(FName Name);
	uint32 UserIndex; ///< Index of the user that is currently logged in (Used in SaveGame and LoadGame)

};
