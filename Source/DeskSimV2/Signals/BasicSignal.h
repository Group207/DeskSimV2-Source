/****************************************************************/
/***
 * @file   BasicSignal.h
 * @brief  Contains basic signal class along with enums and structs used by signals.
 * 
 * @author Endre Heksum
 * @date   March 2022
 *********************************************************************/

// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DeskSimV2/Trains/SplineActor.h>
#include <DeskSimV2/SaveGame/IsSaveableInterface.h>
#include "BasicSignal.generated.h"

/**
 * @brief Contains the different statuses of signals.
 */
UENUM(BlueprintType)
enum class ESignalStatus : uint8
{
	MClear,
	MClearReduced,
	MNotClear,
	MUnmanned,
	FClear,
	FClearReduced,
	FStop,
	FUnmanned,
	DAllowed,
	DForbidden,
	DCareful,
	DFree
};

/**
 * @brief Contains the different types of signals.
 */
UENUM(BlueprintType)
enum class ESignalType : uint8
{
	Main,
	Forward,
	Dwarf
};

/**
 * @brief Contains components and dynamic material of one light.
 */
USTRUCT()
struct FSignalLight
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LightMesh;

	UPROPERTY(VisibleAnywhere, Transient)
	UMaterialInstanceDynamic* DynMaterial;
	
	float BlinkingTimer = 1.0f;

	FTimerHandle LightTimerHandle;

	// Needs a constructor with no params to be used with arrays
	FSignalLight() :
		LightMesh(nullptr),
		DynMaterial(nullptr)
	{}

	// Constructor for the struct, takes in the light mesh
	FSignalLight(UStaticMeshComponent* InLightMesh) :
		LightMesh(InLightMesh),
		DynMaterial(nullptr)
	{}
};

/**
 * @brief Basic signal class, contains functionality to setup and use signal lights.
 * 
 * Needs to be extended into a blueprint class in order to be used. Switching light
 * statuses depends on the type of signal, and each signal uses a custom amount of
 * lights. The model should also be applied in the blueprint.
 */
UCLASS(Blueprintable)
class DESKSIMV2_API ABasicSignal : public AActor, public IIsSaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicSignal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called before the actor is destroyed
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Sets up the lights, with meshes in sockets and dynamic materials
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Signal Light")
	void SetupLight();

protected:
	
	// Removes the lights, by unregistering created components
	UFUNCTION()
	void RemoveLights();

public:

	UFUNCTION(BlueprintCallable, Category = "Signal Light")
	void SetLightOnOff(int32 InLightIndex, bool bInLightOn);
	
	UFUNCTION(BlueprintCallable, Category = "Signal Light")
	void ToggleLight(int32 InLightIndex);

	UFUNCTION(BlueprintCallable, Category = "Signal Light")
	void SetLightColor(int32 InLightIndex, FLinearColor InColor);

	UFUNCTION(BlueprintCallable, Category = "Signal Light")
	void StartLightBlink(int32 InLightIndex, float InSeconds);

	UFUNCTION(BlueprintCallable, Category = "Signal Light")
	void StopLightBlink(int32 InLightIndex);

	/// Function needs to be implemented in blueprints
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Signal Light")
	void UpdateSignalStatus(ESignalStatus NewSignalStatus);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signal Light", SaveGame)
	FName ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Signal Light")
	ESignalType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Signal Light")
	ESignalStatus SignalStatus;

protected:

	/// Array of light components
	UPROPERTY()
	TArray<FSignalLight> Lights;

public:

	/// The visual component for the 3D model
	UPROPERTY(EditDefaultsOnly, Category = "Signal Light")
	UStaticMeshComponent* VisualComponent;

	/// Base mesh for the lights
	UPROPERTY(EditDefaultsOnly, Category = "Signal Light")
	UStaticMesh* BaseLightMesh;

	/// Base material for the dynamic lights
	UPROPERTY(EditDefaultsOnly, Category = "Signal Light")
	UMaterialInterface* BaseLightMaterial;
	
	/// The number of lights to generate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Signal Light")
	int32 NumLights = 2;

	/// The max emissive light level.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Signal Light")
	float MaxLightLevel = 10.0f;

	/// The period of blinking lights.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Signal Light")
	float LightBlinkPeriod = 2.0f;
	
	// ---------- RAILWAY INTEGRATION ------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Railway Integration")
	bool bSnapToNearestRailway; ///< If true, the signal will snap to the nearest railway

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Railway Integration")
	float OffsetFromRailway = 400.0f; ///< The sideways distance to add from the railway in centimeters

protected:

	USplineComponent* GetNearestRailwaySplineComponent();

	void SnapToNearestRailway();
};
