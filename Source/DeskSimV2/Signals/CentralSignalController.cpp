// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "CentralSignalController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

// Sets default values
ACentralSignalController::ACentralSignalController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACentralSignalController::BeginPlay()
{
	Super::BeginPlay();
	
	//Tags.Add(ControllerTag);

	FindAllSignals();

	FindAllTrains();
}

// Called every frame
void ACentralSignalController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * @brief Finds all signals in the level, based on class.
 * Stores all signals in arrays to be used later.
 * 
 */
void ACentralSignalController::FindAllSignals()
{
	// Finds all actors of signal classes
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), DwarfSignalBP, DwarfSignalActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), MainSignalBP, MainSignalActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ForwardSignalBP, ForwardSignalActors);

	int32 i = 1;

	// Loops through all actors
	for (AActor* DwarfSignalActor : DwarfSignalActors)
	{
		// Do stuff here
		ABasicSignal* DwarfSignal = Cast<ABasicSignal>(DwarfSignalActor);
		if (DwarfSignal)
		{
			FString TagName = FString::Printf(TEXT("Dwarf_%i"), i++);
			
			DwarfSignal->Tags.Add(FName(TagName));

			AllSignalActors.Add(DwarfSignalActor);
		}
	}
	i = 1;
	for (AActor* MainSignalActor : MainSignalActors)
	{
		// Do stuff here
		ABasicSignal* MainSignal = Cast<ABasicSignal>(MainSignalActor);
		if (MainSignal)
		{
			FString TagName = FString::Printf(TEXT("Main_%i"), i++);

			MainSignal->Tags.Add(FName(TagName));

			AllSignalActors.Add(MainSignalActor);
		}
	}
	i = 1;
	for (AActor* ForwardSignalActor : ForwardSignalActors)
	{
		// Do stuff here
		ABasicSignal* ForwardSignal = Cast<ABasicSignal>(ForwardSignalActor);
		if (ForwardSignal)
		{
			FString TagName = FString::Printf(TEXT("Forward_%i"), i++);

			ForwardSignal->Tags.Add(FName(TagName));

			AllSignalActors.Add(ForwardSignalActor);
		}
	}

	// Prints how many signals are found
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Number of Main Signals found: %i"), MainSignalActors.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Number of Forward Signals found: %i"), ForwardSignalActors.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Number of Dwarf Signals found: %i"), DwarfSignalActors.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Total number of  Signals found: %i"), AllSignalActors.Num()));
	}

}

/**
 * @brief Finds all trains in the level, based on class.
 * Stores all trains in an array.
 * 
 */
void ACentralSignalController::FindAllTrains()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TrainClass, AllTrainActors);

	int32 i = 1;
	for (AActor* TrainActor : AllTrainActors)
	{
		ATrain* Train = Cast<ATrain>(TrainActor);
		if (Train)
		{
			FString TagName = FString::Printf(TEXT("Train_%i"), i++);

			Train->Tags.Add(FName(TagName));
			
			//AllTrainActors.Add(Train);
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Number of Trains found: %i"), AllTrainActors.Num()));
	}
}

/**
 * @brief Receives signal updates from inputs and triggers, updates the relevant signals.
 * 
 * @param SignalID - The ID of the signal to update
 * @param SignalType - The type of signal to update
 * @param Status - The new status of the signal
 */
void ACentralSignalController::ReceiveInputSignal(FName SignalID, ESignalType SignalType, ESignalStatus Status)
{
	SendUpdatedSignal(SignalID, SignalType, Status);

	UpdateRelatedSignals(SignalID, SignalType, Status);
}

/**
 * @brief Receives emergency stop command, sends to the relevant trains.
 * 
 * @param TrainID - The ID of the train to stop
 */
void ACentralSignalController::ReceiveInputStatus(FName TrainID)
{
	SendTrainEmergencyStop(TrainID);

	// Do more stuff here?
}

/**
 * @brief Finds and sends signal updates to relevant signals.
 * Searches the list of relevant signals.
 * 
 * @param SignalID - The Id of the signal to update
 * @param SignalType - The type of signal to update
 * @param Status - The new status of the signal
 */
void ACentralSignalController::SendUpdatedSignal(FName SignalID, ESignalType SignalType, ESignalStatus Status)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("New status for %s %s: %s"), *SignalID.ToString(), *UEnum::GetValueAsString(SignalType), *UEnum::GetValueAsString(Status)));
	}

	TArray<AActor*> Actors;

	// Selects relevant actors to search based on signal type
	switch (SignalType)
	{
	case ESignalType::Main:
		Actors = MainSignalActors;
		break;
	case ESignalType::Forward:
		Actors = ForwardSignalActors;
		break;
	case ESignalType::Dwarf:
		Actors = DwarfSignalActors;
		break;
	default:
		checkNoEntry();
		break;
	}

	// Loops through all signals
	for (AActor* SignalActor : Actors)
	{
		ABasicSignal* Signal = Cast<ABasicSignal>(SignalActor);
		if (Signal && (Signal->ID == SignalID))
		{
			// If the signal matches the incoming id, update the status
			Signal->UpdateSignalStatus(Status);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Sent new status for signal: %s"), *UEnum::GetValueAsString(Status)));
			}
		}
	}
}

/**
 * @brief Performs conversion on signal status and type to update related signals.
 * 
 * @see SendUpdatedSignal
 * 
 * @param SignalID - The Id of the signal to update
 * @param SignalType - The type of signal to update
 * @param Status - The new status of the signal
 */
void ACentralSignalController::UpdateRelatedSignals(FName SignalID, ESignalType SignalType, ESignalStatus Status)
{
	ESignalStatus RelatedStatus;

	// Defines the related status, just a conversion between main and forward status of similar effect
	switch (Status)
	{
	case ESignalStatus::MClear:			RelatedStatus = ESignalStatus::FClear;			break;
	case ESignalStatus::MClearReduced:	RelatedStatus = ESignalStatus::FClearReduced;	break;
	case ESignalStatus::MNotClear:		RelatedStatus = ESignalStatus::FStop;			break;
	case ESignalStatus::MUnmanned:		RelatedStatus = ESignalStatus::FUnmanned;		break;
	case ESignalStatus::FClear:			RelatedStatus = ESignalStatus::MClear;			break;
	case ESignalStatus::FClearReduced:	RelatedStatus = ESignalStatus::MClearReduced;	break;
	case ESignalStatus::FStop:			RelatedStatus = ESignalStatus::MNotClear;		break;
	case ESignalStatus::FUnmanned:		RelatedStatus = ESignalStatus::MUnmanned;		break;
	default:
		return;
	}
	
	// Sends signal to related signal type
	switch (SignalType)
	{
	case ESignalType::Main:
		SendUpdatedSignal(SignalID, ESignalType::Forward, RelatedStatus);
		break;
	case ESignalType::Forward:
		SendUpdatedSignal(SignalID, ESignalType::Main, RelatedStatus);
		break;
	default:
		return;
	}
}

/**
 * @brief Sends the emergency stop signal to TrainID.
 * 
 * @param TrainID - The ID of the train to stop
 */
void ACentralSignalController::SendTrainEmergencyStop(FName TrainID)
{
	for (AActor* TrainActor : AllTrainActors)
	{
		ATrain* Train = Cast<ATrain>(TrainActor);
		if (Train && (Train->ID == TrainID))
		{
			// Send the emergency stop signal
			Train->bEmergencyStop = true;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Sent Emergency Stop to train: %s"), *TrainID.ToString()));
			}
		}
	}
}

/**
 * @brief Test function stopping all trains.
 * 
 */
void ACentralSignalController::TestAllEmergencyStop()
{
	for (AActor* TrainActor : AllTrainActors)
	{
		ATrain* Train = Cast<ATrain>(TrainActor);
		if (Train)
		{
			// Send the emergency stop signal
			Train->bEmergencyStop = true;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Sent Emergency Stop to train: %s"), *Train->ID.ToString()));
			}
		}
	}
}
