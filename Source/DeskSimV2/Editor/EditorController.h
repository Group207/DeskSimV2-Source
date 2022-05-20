// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <DeskSimV2/Trains/SplineActor.h>
#include "GameFramework/DefaultPawn.h"
#include "EditorController.generated.h"

UENUM()
enum class EToolMode : uint8
{
	Translation		UMETA(DisplayName = "Translation"),
	Rotation		UMETA(DisplayName = "Rotation"),
	//Scaling			UMETA(DisplayName = "Scaling"),
};

UCLASS()
class DESKSIMV2_API AEditorController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEditorController();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent; ///< The root of the editor controller

	UPROPERTY(VisibleAnywhere)
	USceneComponent* GizmoComponent; ///< A component holding all gizmo meshes

	UPROPERTY(VisibleAnywhere)
	APlayerController* PC; ///< The current player controller

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* PawnMovement; ///< The floating pawn movement component in charge of drone movement

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera; ///< Camera component

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowX; ///< The mesh of the gizmo arrow for the X-axis (red)

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowY; ///< The mesh of the gizmo arrow for the Y-axis (green)

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowZ; ///< The mesh of the gizmo arrow for the Z-axis (blue)

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlaneXY; ///< The mesh of the gizmo for the XY-plane (red-green)

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DiscZ; ///< The mesh of gizmo disc for the rotation around the Z-axis

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMoveSpeed = 5000.0f; ///< The maximum allowed move speed of the camera

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration = 6000.0f; ///< The acceleration of the camera

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Deceleration = 10000.0f; ///< The deceleration of the camera

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LookSensistivity = 1.0f; ///< The sensitivity scale of the camera mouse movement

	UPROPERTY(VisibleAnywhere)
		EToolMode ToolMode = EToolMode::Translation; ///< The current mode of the transformation tool. Default is translation.
	
	template<const EToolMode NewToolMode>
	void SetToolMode();

	void DeleteSelectedObject();

private:

	void OnMouseLeftPressed();
	void OnMouseLeftReleased();
	void OnMouseRightPressed();
	void OnMouseRightReleased();
	void SaveMouseLocation();

	void MoveForward(float AxisValue);
	void MoveStrafe(float AxisValue);
	void MoveVertical(float AxisValue);
	void ChangeMoveSpeed(float AxisValue);
	void MovePitch(float AxisValue);
	void MoveYaw(float AxisValue);

	UMaterial* GizmoRed;		///< The red gizmo material
	UMaterial* GizmoGreen;		///< The green gizmo material
	UMaterial* GizmoBlue;		///< The blue gizmo material
	UMaterial* GizmoYellow;		///< The yellow gizmo material

	ETraceTypeQuery GizmoTraceTypeQuery; ///< The trace type query for checking collisions in the gizmo channel
	ETraceTypeQuery ObjectTraceTypeQuery; ///< The trace type query for checking collisions of any object

	UPROPERTY(VisibleAnywhere)
	float MoveSpeed = 1800.0f; ///< The current speed of the camera

	// Rotation

	FVector2D EditorRotation; ///< The rotation of the pawn

	EAxisList::Type SelectedAxis = EAxisList::None; ///< The current selected axis for translation tool. This is None when no gizmo arrows are hovered.

	AActor* SelectedObject; ///< A reference to the currently selected object. This is equal to nullptr if no object is selected.
	UPrimitiveComponent* HoveringGizmo; ///< A reference to the currently hovered gizmo mesh component
	FTransform SelectedTransform;

	UPrimitiveComponent* PreviousHoveringGizmo; /// A reference to a gizmo not hit but grabbed.

	bool bGrabbing; ///< True if the player is holding down left mouse button
	bool bRotateCamera; ///< True if the player is holding down the right mouse button
	bool bDoOnce; ///< Used to only apply materials to the gizmo meshes once in Tick

	void UpdateGizmoVisibilityAndCollision();
	void SetMeshEnabled(const std::initializer_list<UStaticMeshComponent*> Meshes, bool Enabled);
	void SetObjectHighlight(AActor* Object, bool Value);
	void SetSelectedObject(AActor* Object);
	void SetGizmoDefaultMaterials();
	USplineComponent* GetNearestRailwaySplineComponent();

	FVector OffsetOnClick; ///< The offset in position between the world position of the impact point on the clicked gizmo and its origin point. Updated when the player clicks left mouse button.

	FVector MouseLocationOnClick; ///< The position of the mouse at the frame of clicking left mouse button.

	float ObjectRotationOnClick; ///< The rotation of the selected object at the frame of clicking left mouse button.

	ASplineActor* SplineActor;
};
