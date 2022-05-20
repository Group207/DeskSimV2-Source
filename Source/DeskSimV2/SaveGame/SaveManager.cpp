/*****************************************************************/
/**
 * @file   SaveManager.cpp
 * @brief  Handles and Manages saving and loading Data from the Scene and stored .sav files
 * 
 * @author JohnOle
 * @date   March 2022
 *********************************************************************/
// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "IsSaveableInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include "SaveLevel.h"

/**
 * @brief Sets Default Values.
 * 
 */
ASaveManager::ASaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Default to zero for now
	UserIndex = 0;
}

/**
 * @brief Called when the game starts or when spawned.
 * 
 */
void ASaveManager::BeginPlay()
{
	Super::BeginPlay();

	// Getting the current level (.uasset) name
	//LevelName = GetWorld()->GetMapName();
	
}

/**
 * @brief Called every frame on this Actor.
 * 
 * @PARAM DeltaTime - Game time elapsed during last frame modified by the time dilation
 */
void ASaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// TODO: Implement some sort of auto saving system
}

/**
 * @brief Goes through all actors in the scene and saves relevent data specified in SaveLevel.h.
 * 
 */
void ASaveManager::SaveGame()
{
	// Get LevelName to find what to name the .sav file
	LevelName = GetWorld()->GetMapName();

	// Create and instance of SaveGame
	USaveLevel* SaveGameInstance = Cast<USaveLevel>(UGameplayStatics::CreateSaveGameObject(USaveLevel::StaticClass()));
	check(SaveGameInstance);
	// Loop through ALL Actors in Scene
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		// Checks if the Actor inherits from IsSaveableInterface
		if (It->GetClass()->ImplementsInterface(UIsSaveableInterface::StaticClass())) 
		{
			AActor* Actor = *It;

			FActorData ActorData;
			ActorData.Name = Actor->GetFName();
			ActorData.Transform = Actor->GetActorTransform();
			ActorData.Class = Actor->GetClass();

			// Creates a memorywriter and archive for Actor Data
			FMemoryWriter MemWriter(ActorData.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				
			// Only Saves Variables with UPROPERTY(SaveGame)
			Ar.ArIsSaveGame = true;
				
			// Serializes Actors UPROPERTIES into binary
			Actor->Serialize(Ar);
				

			SaveGameInstance->SavedActors.Add(ActorData);
		}
	}

	// Start async save process.
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, LevelName, UserIndex);

	// Log Save
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Saved"));
}

/**
 * @brief Loads Data from a .sav file and fills it into the correct Actors in the scene
 * 
 */
void ASaveManager::LoadGame()
{
	// Get LevelName to find what .sav file to load
	if (!GetWorld()) return;
	
	LevelName = GetWorld()->GetMapName();

	// Create and instance of SaveGame
	USaveLevel* SaveGameInstance = Cast<USaveLevel>(UGameplayStatics::CreateSaveGameObject(USaveLevel::StaticClass()));
	check(SaveGameInstance);
	
	UE_LOG(LogTemp, Display, TEXT("Savegame Line 119"));

	// Load Data from Save file
	SaveGameInstance = Cast<USaveLevel>(UGameplayStatics::LoadGameFromSlot(LevelName, UserIndex));
	if (SaveGameInstance)
	{
		UE_LOG(LogTemp, Display, TEXT("Savegame Line 125"));
		// Iterate through all actors in save and point to their corresponding actor in scene
		for (FActorData ActorData : SaveGameInstance->SavedActors)
		{
			AActor* Actor = FindActorByName(ActorData.Name);
			if (Actor)
			{
				// Updates Actor found in scene
				Actor->SetActorTransform(ActorData.Transform);
			}
			else
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Name = ActorData.Name;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					
				// Creates a new Actor with data from save
				Actor = GetWorld()->SpawnActor(ActorData.Class, &ActorData.Transform, SpawnParams);
					
				// Creates a memoryreader and FArchive
				FMemoryReader MemReader(ActorData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);

				// Only Filters the Variables with UPROPERTY(SaveGame)
				Ar.ArIsSaveGame = true;

				// Converts serialized binary into Actors UPROPERTIES
				Actor->Serialize(Ar);
					
				// Updates the Actor Transform from save
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Actor: %s"), *ActorData.Name.ToString()));
			}
		}
		// Log Load
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Game Loaded"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Savegame Line 163"));
		// Log Error
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Failed to load from file"));
	}
}

/**
 * @brief Finds Actor in Scene with name and returns the a pointer to the Actor if it is found.
 * Else it will return a NULL
 * 
 * @PARAM name - Name of the Actor you wish to find in scene
 * @return  - A pointer to Actor found with name
 */
AActor* ASaveManager::FindActorByName(FName name)
{
	// Find the corresponding actor in scene with name
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (name == Actor->GetFName())
		{
			return Actor;
		}
	}
	return NULL;
}
