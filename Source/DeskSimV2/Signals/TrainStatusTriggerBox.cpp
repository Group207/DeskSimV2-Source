// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "TrainStatusTriggerBox.h"


/// Sets default values
ATrainStatusTriggerBox::ATrainStatusTriggerBox()
{

}

/// Called when the game starts or when spawned
void ATrainStatusTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	// Binds Begin and End Overlap functions to Dynamic Delegates
	this->OnActorBeginOverlap.AddDynamic(this, &ATrainStatusTriggerBox::OnOverlapBegin);
	this->OnActorEndOverlap.AddDynamic(this, &ATrainStatusTriggerBox::OnOverlapEnd);


}

/// Called when game stops or when despawned
void ATrainStatusTriggerBox::EndPlay(EEndPlayReason::Type EndPlayReason)
{


	Super::EndPlay(EndPlayReason);
}

/// Called every frame
void ATrainStatusTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Perform any countinous checking functions
	
}

/// Runs when overlapping begins
void ATrainStatusTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	Super::OnOverlapBegin(OverlappedActor, OtherActor);

	if (OtherActor && (OtherActor != this))
	{
		// Enable ticking for countinous checking, or perform a function once
		switch (StatusTrigger)
		{
		case ETriggerStatusType::EmergencyIf:
			//something
			EmergencyIf(true);
			break;
		case ETriggerStatusType::EmergencyIfNot:
			//something
			EmergencyIfNot();
			break;
		default:
			break;
		}

		
	}
}

/// Runs when overlapping ends
void ATrainStatusTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	Super::OnOverlapEnd(OverlappedActor, OtherActor);

	if (OtherActor && (OtherActor != this))
	{


		// Perform function once after leaving zone
		


		// Clean up default bools
		
	}
}

/**
 * @brief Sends the emergency status to the controller.
 * 
 */
void ATrainStatusTriggerBox::SendStatusToController()
{
	if (CentralController)
	{
		CentralController->ReceiveInputStatus(Train->ID);

		// Disable tick when done
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

/**
 * @brief Sends emergency stop to the train if signal status match or not.
 * 
 * @param bEqual - Used to switch between == and !=
 */
void ATrainStatusTriggerBox::EmergencyIf(bool bEqual)
{
	if (Train && CentralController)
	{
		// Loops through all signals
		for (AActor* SignalActor : CentralController->AllSignalActors)
		{
			ABasicSignal* Signal = Cast<ABasicSignal>(SignalActor);
			
			// Checks if signal id matches
			if (Signal && (Signal->ID == SignalID))
			{
				if (bEqual && Signal->SignalStatus == SignalCompare)
				{
					SendStatusToController();
				}
				else if (!bEqual && Signal->SignalStatus != SignalCompare)
				{
					SendStatusToController();	
				}
				

				if (GEngine)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Sent new status for signal: %s"), *UEnum::GetValueAsString(Status)));
				}
			}
		}
	}
}

/**
 * @brief Sends emergency stop to the train if signal status does not match.
 * 
 */
void ATrainStatusTriggerBox::EmergencyIfNot()
{
	EmergencyIf(false);
}
