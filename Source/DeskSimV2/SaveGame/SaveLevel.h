// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "SaveLevel.generated.h" // Last Include

/**
 * @brief Struct holding all Data needed from an Actor.
 */
USTRUCT()
struct FActorData
{
    GENERATED_BODY()

public:
    /* Identifier for which Actor this belongs to */
    UPROPERTY()
        FName Name;
	
    /* Class of the Actor */
    UPROPERTY()
        UClass* Class = nullptr;;

    /* For movable Actors, keep location,rotation,scale. */
    UPROPERTY()
        FTransform Transform;

    /* Contains all 'SaveGame' marked variables of the Actor */
    UPROPERTY()
        TArray<uint8> ByteData;
};

/**
 * @brief Handles what data is saved in the .sav files.
 */
UCLASS()
class DESKSIMV2_API USaveLevel : public USaveGame
{
	GENERATED_BODY()

public:
    // Default Values
    UPROPERTY(VisibleAnywhere, Category = Basic)
        uint32 UserIndex;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString SaveSlotName;

    UPROPERTY()
        TArray<FActorData> SavedActors;



    USaveLevel();
};
