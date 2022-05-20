// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Train.h"
#include <DeskSimV2/SaveGame/IsSaveableInterface.h>
#include "Drone.generated.h"


UCLASS()
class DESKSIMV2_API ADrone : public APawn, public IIsSaveableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UCameraComponent)
		UCameraComponent* DroneCamera; ///< Camera component for the Pawn.

	bool bIsJustPossessed;
	bool bStartPositionSet;

	// Called to controll the drone movement.
	void MoveForward(float AxisValue);
	void MoveStrafe(float AxisValue);
	void MovePitch(float AxisValue);
	void MoveYaw(float AxisValue);

	void SwitchToTrain();

private:
	ATrain* TrainActor;
	FVector CurrentForward; ///< The current velocity of the drone
	FVector CurrentStrafe;
	FVector2D CurrentRotation; ///< The current rotation of the drone

	const float MAXSPEED = 10000.0f;

};

