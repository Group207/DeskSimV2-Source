/****************************************************************/
/***
 * @file   CentralSignalController.h
 * @brief  Contains the central controller class
 * 
 * @author Endre Heksum
 * @date   March 2022
 *********************************************************************/

// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BasicSignal.h"
#include <DeskSimV2/Trains/Train.h>

#include "CentralSignalController.generated.h"

/**
 * @brief The central signal controller used for communication.
 * 
 * The controller is used to communicate between signals, trigger boxes, and trains.
 * The controller mostly receives signals then forwards them to the correct actors.
 * Only one is needed in the level at a time.
 */
UCLASS()
class DESKSIMV2_API ACentralSignalController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACentralSignalController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void FindAllSignals();

	UFUNCTION()
	void FindAllTrains();

	UFUNCTION(BlueprintCallable, Category = "Controller")
	void ReceiveInputSignal(FName SignalID, ESignalType SignalType, ESignalStatus Status);

	UFUNCTION(BlueprintCallable, Category = "Controller")
	void ReceiveInputStatus(FName TrainID);

	UFUNCTION()
	void SendUpdatedSignal(FName SignalID, ESignalType SignalType, ESignalStatus Status);

	UFUNCTION()
	void UpdateRelatedSignals(FName SignalID, ESignalType SignalType, ESignalStatus Status);

	UFUNCTION()
	void SendTrainEmergencyStop(FName TrainID);

	UFUNCTION(CallInEditor, Category = "Controller")
	void TestAllEmergencyStop();

	/// Class reference of dwarf signal
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TSubclassOf<ABasicSignal> DwarfSignalBP;

	/// Class reference of main signal
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TSubclassOf<ABasicSignal> MainSignalBP;

	/// Class reference of forward signal
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TSubclassOf<ABasicSignal> ForwardSignalBP;

	/// Base class reference of trains
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	TSubclassOf<ATrain> TrainClass;

	/// Array storing all dwarf signal actors
	TArray<AActor*> DwarfSignalActors;

	/// Array storing all main signal actors
	TArray<AActor*> MainSignalActors;

	/// Array storing all forward signal actors
	TArray<AActor*> ForwardSignalActors;

	/// Array storing all signal actors
	TArray<AActor*> AllSignalActors;

	/// Array storing all train actors
	TArray<AActor*> AllTrainActors;
};
