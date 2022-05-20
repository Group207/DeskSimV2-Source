// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

/*****************************************************************//**
 * @file   SplineActor.h
 * @brief  A component for placing static meshes along a spline. The spline conforms to the terrain shape and rotation.
 *
 * @author Thomas Arinesalingam
 * @date   February 2022
 *********************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include <DeskSimV2/SaveGame/IsSaveableInterface.h>
#include "DrawDebugHelpers.h"
#include "SplineActor.generated.h"

UCLASS()
class DESKSIMV2_API ASplineActor : public AActor, public IIsSaveableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASplineActor();

	void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* RailwayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* BufferStopStartMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* BufferStopEndMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	int DistanceBetweenSplinePoints; ///< The number of meshes between each generated splinepoint

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	float MaxHeightAllowed; ///< The maximum change of height allowed along the spline

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	float MaxAngleAllowed; ///< The maximum change of angle allowed along the spline

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	bool bTrackTooSteep; ///< True if the track changes in height more than allowed

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	bool bTrackTooWinding; ///< True if the track changes in angle more than allowed

private:
	float GetMeshAxisLength(UStaticMesh* Mesh, ESplineMeshAxis::Type Axis) const;

	FHitResult GetTerrainHitFromLineTrace(FVector Position) const;

	void CheckSplineCurvature();

	FCollisionQueryParams TerrainTraceParams; ///< Parameters for physical terrain line cast

	TArray<FVector> UpdatedSplinePoints; ///< The number of meshes between each generated spline point
};