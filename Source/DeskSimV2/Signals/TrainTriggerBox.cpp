// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "TrainTriggerBox.h"
#include <DeskSimV2/Editor/EditorController.h>
#include <Kismet/GameplayStatics.h>

/// Sets default values
ATrainTriggerBox::ATrainTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	


	// Sets up default Central Controller Class
	CentralControllerClass = ACentralSignalController::StaticClass();

	Tags.Add("EditorObject");
}

/// Called when the game starts or when spawned
void ATrainTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	// Binds Begin and End Overlap functions to Dynamic Delegates
	//this->OnActorBeginOverlap.AddDynamic(this, &ATrainTriggerBox::OnOverlapBegin);
	//this->OnActorEndOverlap.AddDynamic(this, &ATrainTriggerBox::OnOverlapEnd);

	//PrimaryActorTick.SetTickFunctionEnable(false);

	// Finds the Central Signal Controller
	TArray<AActor*> CentralControllerActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CentralControllerClass, CentralControllerActors);

	// Gets the first valid controller
	for (AActor* CentralControllerActor : CentralControllerActors)
	{
		ACentralSignalController* Controller = Cast<ACentralSignalController>(CentralControllerActor);
		if (Controller)
		{
			CentralController = Controller;
			break;
		}
	}
	// Error message if no controller was found
	if (!CentralController)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Failed to find Central Controller"));
		}
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC->GetPawn())
	{
		if (PC->GetPawn()->GetClass()->IsChildOf(AEditorController::StaticClass()))
		{
			SetActorHiddenInGame(false);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Failed to set hiddenty"));
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Failed to set hiddenty"));
			}
			SetActorHiddenInGame(true);
		}
	}
	else 
	{

	}
}

/// Called when game stops or when despawned
void ATrainTriggerBox::EndPlay(EEndPlayReason::Type EndPlayReason)
{


	Super::EndPlay(EndPlayReason);
}

/// Called every frame
void ATrainTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

/// Runs when overlapping begins
void ATrainTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		// Print debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Begin Overlap"));
		}
		
		// Stores the train which is overlapping
		ATrain* OtherTrain = Cast<ATrain>(OtherActor);
		if (OtherTrain)
		{
			Train = OtherTrain;
		}

	}
}

/// Runs when overlapping ends
void ATrainTriggerBox::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		// Print debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("End Overlap"));
		}
		
		
		
		// Disable tick
		PrimaryActorTick.SetTickFunctionEnable(false);

		// Removes the stored train
		Train = nullptr;

		
	}
}

