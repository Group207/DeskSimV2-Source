/****************************************************************/
/***
 * @file   TrainTriggerBox.h
 * @brief  Base class for trigger box containing some common functionality and enums.
 * 
 * @author Endre Heksum
 * @date   March 2022
 *********************************************************************/

// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "CentralSignalController.h"
#include "BasicSignal.h"
#include <DeskSimV2/Trains/Train.h>

#include "TrainTriggerBox.generated.h"

/**
 * @brief Enum used to select function type for signal trigger box.
 */
UENUM(BlueprintType)
enum class ETriggerSignalType : uint8
{
	WaitStoppedInZone,
	WaitDurationInZone,
	InsideZone,
	NotInsideZone
};

/**
 * @brief Enum used to select function type for status trigger box.
 */
UENUM(BlueprintType)
enum class ETriggerStatusType : uint8
{
	EmergencyIf,
	EmergencyIfNot
};

/**
 * @brief Base class containing some common functionality to be extended.
 * 
 * Class needs to be extended with additional functionality to be used.
 * Current functionality is just storing some common variables, such as
 * the central controller and overlapping train.
 */
UCLASS(Blueprintable, abstract)
class DESKSIMV2_API ATrainTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:

	ATrainTriggerBox();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay();

	// Called before the actor is destroyed
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason);

public:

	// Called every frame
	virtual void Tick(float DeltaTime);
	
	UFUNCTION()
	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	virtual void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
	


	/// Contains the class of the controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TSubclassOf<ACentralSignalController> CentralControllerClass;

	/// Reference to the controller
	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	ACentralSignalController* CentralController;

	/// Reference to the overlapping train
	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	ATrain* Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	ESignalType SignalType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	FName SignalID;

	


};
