/****************************************************************/
/***
 * @file   TrainStatusTriggerBox.h
 * @brief  Contains the status trigger class.
 * 
 * @author Endre Heksum
 * @date   March 2022
 *********************************************************************/

// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TrainTriggerBox.h"
#include "TrainStatusTriggerBox.generated.h"

/**
 * @brief The status trigger box is used to send status updates to the controller.
 * 
 * Currently the only status which is sent is emergency stop. The status is
 * sent after a certain condition, such as entering the zone and a signal is
 * of a certain signal status. 
 */
UCLASS()
class DESKSIMV2_API ATrainStatusTriggerBox : public ATrainTriggerBox
{
	GENERATED_BODY()
	
public:

	ATrainStatusTriggerBox();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Called before the actor is destroyed
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;


public:
	// Called every frame
	void Tick(float DeltaTime) override;

	//UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;

	//UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor) override;

	UFUNCTION()
	void SendStatusToController();

	UFUNCTION()
	void EmergencyIf(bool bEqual);

	UFUNCTION()
	void EmergencyIfNot();

	/// Which trigger type to use for this box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	ETriggerStatusType StatusTrigger;

	/// The signal status to compare against
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	ESignalStatus SignalCompare;


};
