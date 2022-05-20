/****************************************************************/
/***
 * @file   TrainSignalTriggerBox.h
 * @brief  Contains the signal trigger box class
 * 
 * @author Endre Heksum
 * @date   March 2022
 *********************************************************************/

// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TrainTriggerBox.h"
#include "TrainSignalTriggerBox.generated.h"

/**
 * @brief Signal trigger box class which is used to send signal updates.
 * 
 * Signal updates are sent to central controller when the condition is met,
 * such as waiting in a zone for a duration or exiting the zone. Which
 * condition is selected on each instance of the object. 
 */
UCLASS()
class DESKSIMV2_API ATrainSignalTriggerBox : public ATrainTriggerBox
{
	GENERATED_BODY()
	
public:

	ATrainSignalTriggerBox();

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
	void SendSignalToController(ESignalStatus NewSignalStatus);

	UFUNCTION()
	void WaitStoppedInZone(bool& bValid, bool bTimerDone = false);

	UFUNCTION()
	void WaitDurationZone();

	UFUNCTION()
	void InsideZone();

	UFUNCTION()
	void NotInsideZone();


protected:

	bool bDoOnce = true;

	/// Timer handle used to manage timers
	FTimerHandle WaitTimerHandle;

public:

	/// Which condition to use for this box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	ETriggerSignalType SignalTrigger;

	/// The duration to wait for a condition, if applicable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	float WaitDuration;

	/// The new signal status to send to the controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	ESignalStatus NewSignal;
};
