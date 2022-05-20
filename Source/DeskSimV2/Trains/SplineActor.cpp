// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

/*****************************************************************//**
 * @file   SplineActor.cpp
 * @brief  A component for placing static meshes along a spline. The spline conforms to the terrain shape and rotation.
 * 
 * @author Thomas Arinesalingam
 * @date   February 2022
 *********************************************************************/

#include "SplineActor.h"

// Sets default values
ASplineActor::ASplineActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetMobility(EComponentMobility::Movable);

	RootComponent = SplineComponent;

	TerrainTraceParams = FCollisionQueryParams(FName(TEXT("TerrainHeightTrace")), true, this);
	TerrainTraceParams.bTraceComplex = false;
	TerrainTraceParams.bReturnPhysicalMaterial = false;

	DistanceBetweenSplinePoints = 5;
	MaxHeightAllowed = 0.0f;
	MaxAngleAllowed = 0.0f;

	Tags.Add("EditorObject");
}

/**
 * @brief Called on construction of class,
 * generates a mesh for each point in the spline.
 *
 * @param Transform
 */
void ASplineActor::OnConstruction(const FTransform& Transform)
{
	if (RailwayMesh == nullptr) return;

	// Get the length of the bounding box of the mesh along the forward axis
	const float MeshLength = GetMeshAxisLength(RailwayMesh, ForwardAxis);
	const float StartMeshLength = GetMeshAxisLength(BufferStopStartMesh, ForwardAxis);
	const float EndMeshLength = GetMeshAxisLength(BufferStopEndMesh, ForwardAxis);

	const float SplineLength = SplineComponent->GetSplineLength() - (StartMeshLength + EndMeshLength);

	// Set number of spline points to number of meshes fitting inside spline length
	const int SplinePoints = FMath::CeilToInt(SplineLength / MeshLength);

	UpdatedSplinePoints.Empty();

	for (int PointCount = 0; PointCount < SplinePoints; PointCount++)
	{
		// Generate a spline mesh component for each point in the spline
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		// Initialzie mesh properties
		SplineMesh->ComponentTags.Add(FName("EditorSpline"));
		SplineMesh->SetMobility(EComponentMobility::Movable);
		SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SplineMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

		SplineMesh->RegisterComponentWithWorld(GetWorld());
		SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		SplineMesh->SetForwardAxis(ForwardAxis);

		FVector StartPoint;
		FVector EndPoint;

		FVector StartTangent;
		FVector EndTangent;


		/*
			SET START, MIDDLE AND END MESHES
		*/

		// If there is a mesh for the start buffer stop, initialize the first mesh as buffer stop
		if (BufferStopStartMesh != nullptr && PointCount == 0)
		{
			SplineMesh->SetStaticMesh(BufferStopStartMesh);

			StartPoint = SplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local);
			EndPoint = SplineComponent->GetLocationAtDistanceAlongSpline(StartMeshLength, ESplineCoordinateSpace::Local);
			StartTangent = EndTangent = FVector(0);
		}
		// If there is a mesh for the end buffer stop, initialize the end mesh as end buffer stop
		else if (BufferStopEndMesh != nullptr && PointCount == SplinePoints - 1)
		{
			SplineMesh->SetStaticMesh(BufferStopEndMesh);

			StartPoint = SplineComponent->GetLocationAtDistanceAlongSpline(SplineComponent->GetSplineLength() - EndMeshLength, ESplineCoordinateSpace::Local);
			EndPoint = SplineComponent->GetLocationAtDistanceAlongSpline(SplineComponent->GetSplineLength(), ESplineCoordinateSpace::Local);
			StartTangent = EndTangent = FVector(0);
		}
		else // For all meshes in between
		{
			SplineMesh->SetStaticMesh(RailwayMesh);

			const float StartDistance = StartMeshLength + (MeshLength * PointCount);
			StartPoint = SplineComponent->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
			StartTangent = SplineComponent->GetDirectionAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::World);

			const float EndDistance = StartMeshLength + (MeshLength * (PointCount + 1));
			EndPoint = SplineComponent->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
			EndTangent = SplineComponent->GetDirectionAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::World);
		}


		/*
			TERRAIN HEIGHT CONFORM
		*/

		// Cast a line from the mesh start to find the terrain height and normal vector
		const FVector WorldStartPos = SplineComponent->GetWorldLocationAtDistanceAlongSpline(MeshLength * PointCount);
		FHitResult StartHit = GetTerrainHitFromLineTrace(WorldStartPos);

		if (StartHit.bBlockingHit)
		{
			StartPoint.Z = StartHit.ImpactPoint.Z - 100.0f;
			SplineMesh->SetSplineUpDir(StartHit.ImpactNormal, true);
		}

		// Repeat for the mesh end
		const FVector WorldEndPos = SplineComponent->GetWorldLocationAtDistanceAlongSpline(MeshLength * (PointCount + 1));
		FHitResult EndHit = GetTerrainHitFromLineTrace(WorldEndPos);

		if (EndHit.bBlockingHit)
		{
			EndPoint.Z = EndHit.ImpactPoint.Z - 100.0f;
		}

		// Set mesh position and curvature
		SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

		// Add every N meshes to the updated spline points array 
		if(PointCount % DistanceBetweenSplinePoints == 0) UpdatedSplinePoints.Add(StartPoint);
	}
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();

	// Construct the spline
	OnConstruction(this->GetTransform());

	// Set the spline points to the updated spline points
	SplineComponent->SetSplineLocalPoints(UpdatedSplinePoints);

	CheckSplineCurvature();

}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//OnConstruction(this->GetTransform());
}

/**
 * @brief Returns the length of a mesh along a given axis.
 *
 * @param Mesh The mesh to evaluate
 * @param Axis The axis to define length
 * @return Returns the mesh length along axis as a float
 */
float ASplineActor::GetMeshAxisLength(UStaticMesh* Mesh, ESplineMeshAxis::Type Axis) const
{
	if (Mesh == nullptr) return 1.0f;

	// Get the size vector of the mesh
	const FVector MeshSize = Mesh->GetBoundingBox().GetSize();

	// Get the mesh length of the forward axis
	switch (ForwardAxis)
	{
	case ESplineMeshAxis::X:
		return MeshSize.X;
		break;
	case ESplineMeshAxis::Y:
		return MeshSize.Y;
		break;
	case ESplineMeshAxis::Z:
		return MeshSize.Z;
		break;
	default:
		return 1.0f;
		break;
	}

}

/**
 * @brief Casts a ray downwards to find the terrain underneath.
 *
 * @param Position The position of the cast
 * @return Returns the hit result for the cast
 */
FHitResult ASplineActor::GetTerrainHitFromLineTrace(FVector Position) const
{
	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(Hit, FVector(Position.X, Position.Y, 10000.0f), FVector(Position.X, Position.Y, -10000.0f), ECC_WorldStatic, TerrainTraceParams);

	return Hit;
}

/**
 * @brief Check the angle and height change of each spline point,
 * and updates the booleans bTracktooSteep and bTrackTooWinding if
 * the curvature exceeds its threshold.
 * 
 */
void ASplineActor::CheckSplineCurvature()
{
	float MaxHeight = 0.0f;
	float MaxAngle = 0.0f;

	bTrackTooSteep = false;
	bTrackTooWinding = false;

	for (int SplinePoint = 0; SplinePoint < UpdatedSplinePoints.Num() - 1; SplinePoint++)
	{
		FVector FirstPoint = SplineComponent->GetLocationAtSplinePoint(SplinePoint, ESplineCoordinateSpace::World);
		FVector SecondPoint = SplineComponent->GetLocationAtSplinePoint(SplinePoint + 1, ESplineCoordinateSpace::World);
		FVector ThirdPoint = SplineComponent->GetLeaveTangentAtSplinePoint(SplinePoint + 2, ESplineCoordinateSpace::World);

		// Create an offset point where point 3 should be based on the direction between point 1 and 2
		FVector OffsetPoint = SecondPoint + (SecondPoint - FirstPoint);

		float HeightChange = (SecondPoint - FirstPoint).Z;
		float AngleChange = FVector::DistXY(ThirdPoint, OffsetPoint);

		if (HeightChange > MaxHeight) MaxHeight = HeightChange;

		if (AngleChange > MaxAngle) MaxAngle = AngleChange;
	}

	if (MaxHeight > MaxHeightAllowed) bTrackTooSteep = true;
	if (MaxAngle > MaxAngleAllowed) bTrackTooWinding = true;
}
