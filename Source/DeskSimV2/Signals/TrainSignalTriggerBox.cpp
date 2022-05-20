// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "TrainSignalTriggerBox.h"


/// Sets default values
ATrainSignalTriggerBox::ATrainSignalTriggerBox()
{
	
}

/// Called when the game starts or when spawned
void ATrainSignalTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	// Binds Begin and End Overlap functions to Dynamic Delegates
	this->OnActorBeginOverlap.AddDynamic(this, &ATrainSignalTriggerBox::OnOverlapBegin);
	this->OnActorEndOverlap.AddDynamic(this, &ATrainSignalTriggerBox::OnOverlapEnd);
	
}

/// Called when game stops or when despawned
void ATrainSignalTriggerBox::EndPlay(EEndPlayReason::Type EndPlayReason)
{


	Super::EndPlay(EndPlayReason);
}

/// Called every frame
void ATrainSignalTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Perform any countinous checking functions
	switch (SignalTrigger)
	{
	case ETriggerSignalType::WaitStoppedInZone:
		// Needs a reference to a bool
		WaitStoppedInZone(bDoOnce);
		break;
	default:
		break;
	}
}

/// Runs when overlapping begins
void ATrainSignalTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	Super::OnOverlapBegin(OverlappedActor, OtherActor);
	
	if (OtherActor && (OtherActor != this))
	{
		

		// Enable ticking for countinous checking, or perform a function once
		switch (SignalTrigger)
		{
		case ETriggerSignalType::WaitStoppedInZone:
			// Enable ticking, function is called in Tick
			PrimaryActorTick.SetTickFunctionEnable(true);
			break;
		case ETriggerSignalType::WaitDurationInZone:
			WaitDurationZone();
			break;
		case ETriggerSignalType::InsideZone:
			InsideZone();
			break;
		default:
			break;
		}
	}
}

/// Runs when overlapping ends
void ATrainSignalTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	Super::OnOverlapEnd(OverlappedActor, OtherActor);

	if (OtherActor && (OtherActor != this))
	{
		

		// Perform function once after leaving zone
		switch (SignalTrigger)
		{
		case ETriggerSignalType::NotInsideZone:
			NotInsideZone();
			break;
		default:
			break;
		}


		// Clean up default bools
		bDoOnce = true;
	}
}


/**
 * @brief Sends a new signal status to the Central Controller. Also stops ticking.
 *
 * @param NewSignalStatus - Signal status to send to the Signal Controller
 */
void ATrainSignalTriggerBox::SendSignalToController(ESignalStatus NewSignalStatus)
{
	if (CentralController)
	{
		CentralController->ReceiveInputSignal(SignalID, SignalType, NewSignalStatus);

		// Disable tick when done
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

/**
 * @brief Checks if the Train is stopped inside the zone. Sets a timer at WaitDuration to send new signal status.
 * The timer is reset if the train moves. Call this in Tick, as it countinually checks for train speed.
 *
 * @param bOnce - Stores whether the timer has started, needs a reference to outside variable.
 * @param bTimerDone - If the timer is done, send the signal instead. Used internally.
 */
void ATrainSignalTriggerBox::WaitStoppedInZone(bool& bOnce, bool bTimerDone)
{
	if (Train)
	{
		// Gets the speed of the train, checks if it is standing still
		float Speed = Train->GetSpeed();
		bool bTrainStopped = FMath::IsNearlyEqual(Speed, 0.0f, 0.5f);

		// Sends the signal if the timer is done and train is still stopped
		if (bTrainStopped && bTimerDone)
		{
			SendSignalToController(NewSignal);
		}
		else if (bTrainStopped && bOnce)
		{
			// Only create the timer once when valid
			bOnce = false;

			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("WaitStoppedInZone"), bOnce, true);

			GetWorldTimerManager().SetTimer(WaitTimerHandle, TimerDel, WaitDuration, false);
		}
		else if (!bTrainStopped)
		{
			// If the train is not stopped, clear the timer and try again
			bOnce = true;
			GetWorldTimerManager().ClearTimer(WaitTimerHandle);

		}
	}
}

/**
 * @brief Sends a signal update after WaitDuration.
 *
 */
void ATrainSignalTriggerBox::WaitDurationZone()
{
	if (Train)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("SendSignalToController"), NewSignal);

		GetWorldTimerManager().SetTimer(WaitTimerHandle, TimerDel, WaitDuration, false);
	}
}

/**
 * @brief Sends a signal update once zone is entered.
 *
 */
void ATrainSignalTriggerBox::InsideZone()
{
	if (Train)
	{
		SendSignalToController(NewSignal);
	}
}

/**
 * @brief Sends a signal update after zone is left.
 *
 */
void ATrainSignalTriggerBox::NotInsideZone()
{
	if (Train)
	{
		SendSignalToController(NewSignal);
	}
}
