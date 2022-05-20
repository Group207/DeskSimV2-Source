// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "Drone.h"
#include "Math/Quat.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Camera"));

	DroneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Our Camera"));
	
	DroneCamera->SetupAttachment(RootComponent);
	DroneCamera->SetRelativeLocation(FVector(0.0f, 0.0, 0.0f));
	DroneCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	bIsJustPossessed = false;

}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Trains;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrain::StaticClass(), Trains);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (Trains.Num() >= 1)
	{
		TrainActor = Cast<ATrain>(Trains[0]);
		if (!TrainActor)
		{
			PrimaryActorTick.bCanEverTick = false;
		}
	}

}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPlayerControlled())
	{
		bStartPositionSet = false;
	}

	if (bStartPositionSet == false && TrainActor != nullptr)
	{
		FRotator rot = FRotator(0, 0, 0);

		FVector LookAt = TrainActor->GetPosition();

		FVector RotationTrain = TrainActor->GetRotation();
		FVector StartPosition = LookAt - (RotationTrain * 1000);
		SetActorLocation(StartPosition + FVector(0, 0, 1000));

		FRotator Rotation = LookAt.Rotation();

		FQuat StartRotation = Rotation.Quaternion();

		SetActorRotation(RotationTrain.Rotation());

		bStartPositionSet = true;

	}

	FVector CurrentVelocity = CurrentForward + CurrentStrafe; 

	FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime * MAXSPEED;
	SetActorLocation(NewLocation);

	// Calculating mouse rotation.
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CurrentRotation.X;
	NewRotation.Pitch += CurrentRotation.Y;

	// Setting drone velocity and rotation
	SetActorRotation(NewRotation);

}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MouseY", this, &ADrone::MoveYaw);
	InputComponent->BindAxis("MouseX", this, &ADrone::MovePitch);
	InputComponent->BindAxis("forward", this, &ADrone::MoveForward);
	InputComponent->BindAxis("strafe", this, &ADrone::MoveStrafe);

	InputComponent->BindAction(FName("ToggleDrone"), EInputEvent::IE_Pressed, this, &ADrone::SwitchToTrain);
	if (TrainActor)
	{
		InputComponent->BindAction(FName("Pause"), EInputEvent::IE_Pressed, TrainActor, &ATrain::SetPause);
	}
}

void ADrone::MoveForward(float AxisValue)
{
	CurrentForward = this->GetActorForwardVector() * FMath::Clamp(AxisValue, -1.f, 1.f);
}
/**
 * @brief Updates velocity based on keyboard input.
 *
 * @param AxisValue - float from keypress
 */
void ADrone::MoveStrafe(float AxisValue)
{
	CurrentStrafe = this->GetActorRightVector() * FMath::Clamp(AxisValue, -1.f, 1.f);
}

/**
 * @brief Updates rotation based on mouse input.
 *
 * @param AxisValue - float from mouse movement
 */
void ADrone::MovePitch(float AxisValue)
{
	CurrentRotation.X = AxisValue;
}

/**
 * @brief Updates velocity based on mouse input.
 *
 * @param AxisValue - float from mouse movement
 */
void ADrone::MoveYaw(float AxisValue) 
{ 
	CurrentRotation.Y = AxisValue; 
}

/**
 * @brief Switches to train driving mode.
 * 
 */
void ADrone::SwitchToTrain()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC && TrainActor) 
	{
		TrainActor->ShowDMI();
		GetWorldTimerManager().UnPauseTimer(TrainActor->TextTimerHandle);
		PC->Possess(TrainActor);
	}

}
