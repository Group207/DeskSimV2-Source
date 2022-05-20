// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SplineActor.h"
#include "Wagon.h"
#include <DeskSimV2/Editor/UI/EditorHUD.h>
#include <DeskSimV2/SaveGame/IsSaveableInterface.h>
#include "Camera/CameraComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Train.generated.h"

UCLASS()
class DESKSIMV2_API ATrain : public APawn, public IIsSaveableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool HasWagonsChanged();
	void AddOrRemoveWagonActors();

	/**
	 * @brief Accessor function for retrieving the train speed.
	 */
	UFUNCTION(BlueprintPure, Category = "Speed")
	float GetSpeed();

	// The EditAnywhere macro lets you modify the element where you want to

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Velocity; ///< The velocity of the train


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TrainComponent; ///< Train when drive mode

	UStaticMeshSocket* TrainSocket;

	UPROPERTY(EditAnywhere)
	ASplineActor* OurSplineActor; ///< The spline track for the train

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UCameraComponent")
	UCameraComponent* OurCamera;


	void TrainControl(float DeltaTime);
	void UpdateTrain(float DeltaTime);
	void UpdateWagons(float DeltaTime);
	void UpdateWagonEditor();

	void SwitchToDrone();

	void AccelerateAxis(float AxisValue);
	void BreakAxis(float AxisValue);

	UFUNCTION()
	void UpdateText();

	FVector GetPosition();
	FVector GetRotation();


	ASplineActor* GetNearestRailwaySplineComponent();

	void SetPause();
	void ShowDMI();

	TArray<AWagon*> WagonActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ID;

	// Train Speed

	FVector LastPosition;
	float TimesinceUpdate;
	float Speed = 0.0f;
	FTimerHandle TextTimerHandle;
	// Train Movement

	float Acceleration;
	float BreakForce;
	
	// Input acceleration and braking from axis bindings

	float AxisAcceleration;
	float AxisBreakForce;

	// Default values for the train

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Train")
	float AccelerationStrength = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Train")
	float BrakingStrength = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Train")
	float MaxAcceleration = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Train")
	float MaxBreakForce = 200.f;


	float DistanceAlongSpline;  ///< The increasing distance of the train along the spline

	// Train emergency stop
	UPROPERTY(VisibleAnywhere)
	bool bEmergencyStop = false;

	// Tick check
	bool bEditorMode;

	int OldWagonCount;

	FVector TotalWagonOffset; ///< The total offset a new wagon will begin at position.

	AEditorHUD* HUD;

};
