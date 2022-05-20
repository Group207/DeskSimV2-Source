// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#include "Train.h"
#include "Engine/Engine.h"
#include "Math/Quat.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include <DeskSimV2/Editor/EditorController.h>
#include <Kismet/GameplayStatics.h>
#include "Drone.h"
#include "GameFramework/GameModeBase.h"
#include "Components/PointLightComponent.h"

#include <DeskSimV2/SaveGame/SaveManager.h>
#include "Kismet/GameplayStatics.h"


/**
 * @brief Constructor for AMyPawn class.
 *
 * @return
 */
ATrain::ATrain()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Creating the the train components in the scene
	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));

	SetRootComponent(RootSceneComponent);
	

	TrainComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Driver Mesh"));
	TrainComponent->SetupAttachment(RootComponent);
	/*
	Offsetting the origin of the Train
	FVector OriginOffset = FVector(0, 1500, 190);
	TrainComponent->SetRelativeLocation(OriginOffset);


	*/

	// Camera for train driving.
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Our Camera"));
	OurCamera->SetupAttachment(RootComponent);
	//OurCamera->SetRelativeLocation(FVector(0.0f, -224.0f, 261.0f));
	//OurCamera->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	

	Tags.Add("EditorObject");

}

/**
 * @brief Starts the Scene.
 *
 */
void ATrain::BeginPlay()
{
	Super::BeginPlay();

	HUD = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AEditorHUD>();

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("UpdateText"));

	GetWorldTimerManager().SetTimer(TextTimerHandle, TimerDel, 0.5f, true);

	OurSplineActor = GetNearestRailwaySplineComponent();

	if (OurSplineActor)
	{
		FTransform ClosestSplineTransform = OurSplineActor->SplineComponent->FindTransformClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);

		FVector Rotation = OurSplineActor->SplineComponent->FindRightVectorClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
		SetActorRotation(Rotation.Rotation(), ETeleportType::None);
		

		float key = OurSplineActor->SplineComponent->FindInputKeyClosestToWorldLocation(GetActorLocation());
		float val = OurSplineActor->SplineComponent->GetDistanceAlongSplineAtSplineInputKey(key);

		DistanceAlongSpline = val;
	}
	else
	{
		PrimaryActorTick.SetTickFunctionEnable(false);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Failed to find spline actor"));
		}
	}
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	FString Options = *GameMode->OptionsString;

	FString ControllString = UGameplayStatics::ParseOption(Options, FString("Controller"));

	if (PC)
	{
		PC->bAutoManageActiveCameraTarget = true;
		if (PC->GetPawn())
		{

			// TODO this fails in packaged builds
			if (ControllString.Equals(FString("Editor")))
			{
				bEditorMode = true;
				PrimaryActorTick.SetTickFunctionEnable(true);
			}
			else
			{
				bEditorMode = false;
				PC->Possess(this);
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("First train possess"));
				}
			}
		}
		else
		{
			bEditorMode = false;
			PC->Possess(this);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Second train possess"));
			}
		}
	}
	else return;
	AddOrRemoveWagonActors();
}

/**
 * @brief Called every frame.
 *
 * @param DeltaTime
 */
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bEditorMode) {
		TrainControl(DeltaTime);

		UpdateTrain(DeltaTime);

		UpdateWagons(DeltaTime);
	}
	else
	{
		if (HasWagonsChanged())
		{
			AddOrRemoveWagonActors();
		}
		else
		{
			// TODO - only update when train is moving.
			UpdateWagonEditor();
		}

	}
}

/**
 * @brief.Updates the train wagons in realtime during a simulation
 * 
 * @param DeltaTime - fps controll
 */
void ATrain::UpdateWagons(float DeltaTime)
{
	if (OurSplineActor != nullptr) {
		for (int32 i = 0; i < WagonActors.Num(); i++)
		{
			float Distance = DistanceAlongSpline;
			
			float Pos = Distance - (TotalWagonOffset.Size() / (WagonActors.Num() - i));

			FVector DesiredDirection = OurSplineActor->SplineComponent->GetDirectionAtDistanceAlongSpline(Pos, ESplineCoordinateSpace::World);
			FRotator RotationOffset(0, 90, 0);
			FVector SplineRotation = RotationOffset.RotateVector(DesiredDirection);
			WagonActors[i]->SetRotation(SplineRotation);

			// Updating the train position based on the distance along the spline.

			FVector NewPosition = OurSplineActor->SplineComponent->GetLocationAtDistanceAlongSpline(Pos, ESplineCoordinateSpace::World);
			WagonActors[i]->SetPosition(NewPosition);
		}
	}
}

/**
 * @brief Adds a new, or alredy existing wagon objets to a list .
 * 
 */
void ATrain::AddOrRemoveWagonActors()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWagon::StaticClass(), Actors);

	if (Actors.Num() > WagonActors.Num())
	{
		for (int32 i = WagonActors.Num(); i < Actors.Num(); i++)
		{
			WagonActors.Add(Cast<AWagon>(Actors[i]));
		}
	}
	else
	{
		for (int32 i = WagonActors.Num(); i > Actors.Num(); i--)
		{
			WagonActors.RemoveAt(i - 1);
		}
	}

	UpdateWagonEditor();

}

/**
 * @brief Checks if the number of wagons is different than the last timem the function was run..
 * 
 * @return 
 */
bool ATrain::HasWagonsChanged()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWagon::StaticClass(), Actors);

	if (Actors.Num() != WagonActors.Num())
	{
		return true;
	}

	return false;
}

/**
 * @brief Handles the updating of the wagons in editor mode, including snapping to back of train.
 * 
 */
void ATrain::UpdateWagonEditor()
{
	FName TSocket = FName("train-socket");
	TrainSocket = TrainComponent->GetStaticMesh()->FindSocket(TSocket);
	FVector TrainOffset;

	if (TrainSocket)
	{
		TrainOffset = TrainSocket->RelativeLocation;
	}
	else return;

	for (int32 i = 0; i < WagonActors.Num(); i++)
	{
		UStaticMesh* Mesh = WagonActors[i]->VisibleMesh->GetStaticMesh();

		FName FSocket = FName("front-socket");
		FName BSocket = FName("back-socket");

		UStaticMeshSocket* FrontSocket = Mesh->FindSocket(FSocket);
		UStaticMeshSocket* BackSocket = Mesh->FindSocket(BSocket);
		if (FrontSocket)
		{
			FVector WagonOffset = FrontSocket->RelativeLocation;

			if (i == 0)
			{
				TotalWagonOffset = FVector(TrainOffset.X, TrainOffset.Y, 0.0f) + FVector(WagonOffset.X, WagonOffset.Y, 0.0f);
				WagonActors[i]->SetPosition(GetActorLocation() - TotalWagonOffset);

				FVector BackOffset = BackSocket->RelativeLocation;
				
			}
			else
			{
				FrontSocket = Mesh->FindSocket(FSocket);
				BackSocket = Mesh->FindSocket(BSocket);

				WagonOffset = BackSocket->RelativeLocation;
				FVector FrontOffset = FrontSocket->RelativeLocation;
				TotalWagonOffset += FVector(FrontOffset.X - WagonOffset.X, FrontOffset.Y - WagonOffset.Y, 0.0f);

				WagonActors[i]->SetPosition(GetActorLocation() - TotalWagonOffset);}

		}
	}
}

/**
 * @brief Switches to drone mode.
 * 
 */
void ATrain::SwitchToDrone()
{
	TArray<AActor*> Drones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADrone::StaticClass(), Drones);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (Drones.Num() >= 1)
	{
		ADrone* Drone = Cast<ADrone>(Drones[0]);
		if (Drone)
		{
			GetWorldTimerManager().PauseTimer(TextTimerHandle);
			HUD->HideTrainDMI();
			PC->Possess(Drone);
		}
	}
}

/**
 * @brief Updates train speed from input.
 * 
 * @param DeltaTime
 */
void ATrain::TrainControl(float DeltaTime)
{
	if (bEmergencyStop)
	{
		AccelerateAxis(0);
		BreakAxis(1);

		// Train is stopped
		if (FMath::IsNearlyEqual(Speed, 0.0f, 0.1f))
		{
			// Reset the EmergencyStop command
			bEmergencyStop = false;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Reset emergency stop for train: %s"), *ID.ToString()));
			}
		}
	}

	Acceleration = FMath::Clamp(AxisAcceleration, 0.0f, MaxAcceleration);
	BreakForce = FMath::Clamp(AxisBreakForce, 0.0f, MaxBreakForce);

	// Physics for train driving
	Velocity += Acceleration * DeltaTime;

	if (Velocity > 0)
	{
		Velocity -= BreakForce * DeltaTime;
	}

	DistanceAlongSpline += Velocity * DeltaTime;
}

/**
 * @brief Updates train position based on speed.
 * 
 * @param DeltaTime
 */
void ATrain::UpdateTrain(float DeltaTime)
{

	// Finding and setting the correct train orientation according to the spline.

	if (OurSplineActor != nullptr) {
		FVector DesiredDirection = OurSplineActor->SplineComponent->GetDirectionAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator RotationOffset(0, 90, 0);
		FVector SplineRotation = RotationOffset.RotateVector(DesiredDirection);
		SetActorRotation(SplineRotation.Rotation(), ETeleportType::None);

		// Updating the train position based on the distance along the spline.
		FVector NewPosition = OurSplineActor->SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		SetActorLocation(NewPosition);


		// Calculating speed
		Speed = ((((NewPosition - LastPosition)).Size()) / DeltaTime * 0.036f);

		LastPosition = NewPosition;
	}

}


/**
 * @brief Called to bind functionality to input.
 *
 * @param PlayerInputComponent - Used to bind functionality to input
 */
void ATrain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("AxisAcceleration", this, &ATrain::AccelerateAxis);
	InputComponent->BindAxis("AxisBreakForce", this, &ATrain::BreakAxis);

	InputComponent->BindAction(FName("Pause"), EInputEvent::IE_Pressed, this,  &ATrain::SetPause);
	InputComponent->BindAction(FName("ToggleDrone"), EInputEvent::IE_Pressed, this, &ATrain::SwitchToDrone);
}

/**
 * @brief Return the speed of the train.
 * 
 * @return Speed - Stored speed of the train updated every Tick.
 */
float ATrain::GetSpeed()
{
	return Speed;
}

void ATrain::SetPause()
{
	if (HUD)
	{
		HUD->SetupPauseMenu();
	}
}

void ATrain::ShowDMI()
{
	if (HUD)
	{
		HUD->ShowTrainDMI();
	}
}

/**
 * @brief Updates the train acceleration based on raw input.
 *
 * @param AxisValue - Raw input from flightstick
 */
void ATrain::AccelerateAxis(float AxisValue)
{
	AxisAcceleration = FMath::Clamp(AxisValue, 0.0f, 1.0f) * AccelerationStrength;
}

/**
 * @brief Updates the train break force based on wasd input.
 *
 * \param AxisValue - Raw input from flightstick
 */
void ATrain::BreakAxis(float AxisValue)
{
	AxisBreakForce = FMath::Clamp(AxisValue, 0.0f, 1.0f) * BrakingStrength;
}


FVector ATrain::GetPosition()
{
	return RootComponent->GetComponentLocation();
}

FVector ATrain::GetRotation()
{
	FVector DesiredDirection = OurSplineActor->SplineComponent->GetDirectionAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	
	return DesiredDirection;
}


/**
* @brief Locates the USplineComponent object nearest the
* location of a given vector in world space
*
* @return Returns a pointer to the relevant USplineComponent if any
*/
ASplineActor* ATrain::GetNearestRailwaySplineComponent()
{
	TSubclassOf<ASplineActor> ClassToFind = ASplineActor::StaticClass();
	TArray<AActor*> AllRailways;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, AllRailways);

	// Return null if no railways were found
	if (AllRailways.Num() == 0)
	{
		return nullptr;
	}

	// Loop through all found railways and compare distance to get the nearest
	AActor* ClosestActor = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (AActor* Railway : AllRailways)
	{
		float Distance = FVector::DistSquared(GetActorLocation(), Railway->GetActorLocation());

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Railway;
		}
	}

	// Cast nearest actor to ASplineActor and return its SplineComponent
	ASplineActor* ClosestRailway = Cast<ASplineActor>(ClosestActor);
	return (ClosestRailway == nullptr) ? nullptr : ClosestRailway;
}

void ATrain::UpdateText()
{
	if (HUD)
	{
		HUD->SetTrainDMI(Speed);
	}
}
