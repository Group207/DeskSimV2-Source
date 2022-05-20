// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "EditorController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEditorController::AEditorController()
{
 	// Set this pawn to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	// Add a floating pawn movement for the drone view
	
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovementComponent"));

	PawnMovement->Acceleration = Acceleration;
	PawnMovement->Deceleration = Deceleration;

	// Add a camera inside the drone
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->bUsePawnControlRotation = true;
	Camera->bUseAttachParentBound = true;
	//Camera->AttachToComponent(RootSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Camera->SetupAttachment(RootSceneComponent); // NEW

	// Define a gizmo component to hold gizmo meshes
	GizmoComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Gizmo Component"));
	GizmoComponent->bUseAttachParentBound = false;

	// Define trace channel queries for different collision channels
	GizmoTraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);
	ObjectTraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera);

	// Get references to gizmo materials from files
	static ConstructorHelpers::FObjectFinder<UMaterial>RedGizmoMaterial(TEXT("Material'/Game/Materials/TransformGizmoRed.TransformGizmoRed'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>GreenGizmoMaterial(TEXT("Material'/Game/Materials/TransformGizmoGreen.TransformGizmoGreen'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>BlueGizmoMaterial(TEXT("Material'/Game/Materials/TransformGizmoBlue.TransformGizmoBlue'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>YellowGizmoMaterial(TEXT("Material'/Game/Materials/TransformGizmoYellow.TransformGizmoYellow'"));

	// Set gizmo materials
	GizmoRed = RedGizmoMaterial.Object;
	GizmoGreen = GreenGizmoMaterial.Object;
	GizmoBlue = BlueGizmoMaterial.Object;
	GizmoYellow = YellowGizmoMaterial.Object;

	// Get reference to arrow mesh for gizmo
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ArrowMeshAsset(TEXT("StaticMesh'/Game/Models/Editor/TransformArrow.TransformArrow'"));
	UStaticMesh* ArrowMesh = ArrowMeshAsset.Object;

	// Create arrows for X, Y and Z, rotating Y and Z to point in the relative directions
	ArrowX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Transform Arrow X"));
	ArrowX->SetStaticMesh(ArrowMesh);

	ArrowY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Transform Arrow Y"));
	ArrowY->SetStaticMesh(ArrowMesh);
	ArrowY->AddRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	ArrowZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Transform Arrow Z"));
	ArrowZ->SetStaticMesh(ArrowMesh);
	ArrowZ->AddRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	// Get a reference to the mesh of the gizmo for the XY-plane and initialize
	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlaneMeshAsset(TEXT("StaticMesh'/Game/Models/Editor/TransformArrowXYSquare.TransformArrowXYSquare'"));
	UStaticMesh* PlaneMesh = PlaneMeshAsset.Object;
	PlaneXY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Transform Plane XY"));
	PlaneXY->SetStaticMesh(PlaneMesh);

	// Get a reference to the mesh of the rotation disc gizmo and initialize
	static ConstructorHelpers::FObjectFinder<UStaticMesh>DiscMeshAsset(TEXT("StaticMesh'/Game/Models/Editor/RotationDisc.RotationDisc'"));
	UStaticMesh* DiscMesh = DiscMeshAsset.Object;
	DiscZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotation Disc Z"));
	DiscZ->SetStaticMesh(DiscMesh);

	SetGizmoDefaultMaterials();

	//std::initializer_list<UStaticMeshComponent*> GizmoMeshComponents = { ArrowX, ArrowY, ArrowZ, PlaneXY, DiscZ };

	// Attatch all gizmo meshes to GizmoComponent, set the correct collision profile, hide and disable collision
	for (UStaticMeshComponent* Mesh : { ArrowX, ArrowY, ArrowZ, PlaneXY, DiscZ })
	{
		//Mesh->AttachToComponent(GizmoComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Mesh->SetupAttachment(GizmoComponent); // NEW
		Mesh->SetCollisionProfileName(FName("Gizmo"));
		Mesh->SetVisibility(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Set player and enable receive input
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AEditorController::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the playercontroller and set default parameters for mouse interaction
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//PC = Cast<APlayerController>(GetController());

	if (PC == nullptr) return;
	
	EnableInput(PC);

	// Set player controller to show cursor and read mouse events
	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
	PC->bAutoManageActiveCameraTarget = true;

	USplineComponent* Spline = GetNearestRailwaySplineComponent();
	if (Spline)
	{
		FVector Position = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		SetActorLocation(Position + FVector(0, 0, 500));
	}
}

// Called every frame
void AEditorController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SelectedObject != nullptr)
	{
		// Set the scale of the gizmo relative to distance between object and camera
		float Distance = FVector::Distance(this->GetActorLocation(), GizmoComponent->GetComponentLocation());
		float Scale = 1.0f + (Distance / 500.0f);
		GizmoComponent->SetWorldScale3D(FVector::OneVector * Scale);

		// Cast a ray to see if the mouse is over a gizmo
		FHitResult GizmoHit;
		PC->GetHitResultUnderCursorByChannel(GizmoTraceTypeQuery, true, GizmoHit);

		if (GizmoHit.bBlockingHit)
		{
			// If a gizmo is hit, change cursor and material of hovered gizmo
			if (!bDoOnce || GizmoHit.GetComponent() != HoveringGizmo)
			{
				PC->CurrentMouseCursor = EMouseCursor::GrabHand;

				SetGizmoDefaultMaterials();
				HoveringGizmo = GizmoHit.GetComponent();

				for (int i = 0; i < HoveringGizmo->GetNumMaterials() - 1; i++)
				{
					HoveringGizmo->SetMaterial(i, GizmoYellow);
				}
				bDoOnce = true;
			}

			// If the user is holding the mouse button, change cursor and check for which tool mode is in use
			if (bGrabbing)
			{
				PC->CurrentMouseCursor = EMouseCursor::GrabHandClosed;

				switch (ToolMode)
				{
					// If the mode is translation, check which axis to move
					case EToolMode::Translation:
					{
						FVector NewLoc = GizmoHit.ImpactPoint - OffsetOnClick;
						FVector CurrentLoc = SelectedObject->GetActorLocation();

						// Set the location to the new location along the selected axis, both X and Y in case of the XY-plane
						switch (SelectedAxis)
						{
						case EAxisList::None:
							break;
						case EAxisList::X:
							SelectedObject->SetActorLocation(FVector(NewLoc.X, CurrentLoc.Y, CurrentLoc.Z));
							//SelectedTransform.SetLocation(FVector(NewLoc.X, CurrentLoc.Y, CurrentLoc.Z));
							break;
						case EAxisList::Y:
							SelectedObject->SetActorLocation(FVector(CurrentLoc.X, NewLoc.Y, CurrentLoc.Z));
							//SelectedTransform.SetLocation(FVector(CurrentLoc.X, NewLoc.Y, CurrentLoc.Z));
							break;
						case EAxisList::Z:
							SelectedObject->SetActorLocation(FVector(CurrentLoc.X, CurrentLoc.Y, NewLoc.Z));
							//SelectedTransform.SetLocation(FVector(CurrentLoc.X, CurrentLoc.Y, NewLoc.Z));
							break;
						case EAxisList::XY:
							SelectedObject->SetActorLocation(FVector(NewLoc.X, NewLoc.Y, CurrentLoc.Z));
							//SelectedTransform.SetLocation(FVector(NewLoc.X, NewLoc.Y, CurrentLoc.Z));
							break;
						default:
							break;
						}
						break;
					}

					// If the mode is rotation, set the rotation to the delta value of the horizontal mouse position since the start of the click
					case EToolMode::Rotation:
					{
						float MouseX, MouseY;
						PC->GetMousePosition(MouseX, MouseY);

						// The value changed in the horizontal mouse position since drag start
						float MouseDelta = MouseX - MouseLocationOnClick.X;

						// The value changed in rotation since drag start
						float RotationDelta = (MouseDelta * -1) + ObjectRotationOnClick;
						
						// Rotate the object in the Z-axis
						SelectedObject->SetActorRotation(FRotator(0.0f, RotationDelta, 0.0f));
					}
						break;

					// Scaling, if implemented in the future
					//case EToolMode::Scaling:
						//break;
					default:
						break;
				}
			}
		}
		else // If no gizmo is under the cursor
		{
			// Set mouse cursor and gizmo materials back to default once
			if (bDoOnce) 
			{
				PC->CurrentMouseCursor = EMouseCursor::Default;
				SetGizmoDefaultMaterials();
				bDoOnce = false;
			}
		}

		// SEt the location of the gizmo component to that of the selected object
		GizmoComponent->SetWorldLocation(SelectedObject->GetActorLocation());
	}
}

// Called to bind functionality to input
void AEditorController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind input events to actions

	// Mouse left
	InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &AEditorController::OnMouseLeftPressed);
	InputComponent->BindAction("MouseLeftClicked", IE_Released, this, &AEditorController::OnMouseLeftReleased);

	// Mouse right
	InputComponent->BindAction("MouseRightClicked", IE_Pressed, this, &AEditorController::OnMouseRightPressed);
	InputComponent->BindAction("MouseRightClicked", IE_Released, this, &AEditorController::OnMouseRightReleased);

	// Mouse position
	InputComponent->BindAxis("MouseX", this, &AEditorController::MoveYaw);
	InputComponent->BindAxis("MouseY", this, &AEditorController::MovePitch);

	// Movement keys
	InputComponent->BindAxis("forward", this, &AEditorController::MoveForward);
	InputComponent->BindAxis("strafe", this, &AEditorController::MoveStrafe);
	InputComponent->BindAxis("vertical", this, &AEditorController::MoveVertical);
	InputComponent->BindAxis("scrollwheel", this, &AEditorController::ChangeMoveSpeed);

	// Tool mode switch
	InputComponent->BindAction("TranslationMode", IE_Pressed, this, &AEditorController::SetToolMode<EToolMode::Translation>);
	InputComponent->BindAction("RotationMode", IE_Pressed, this, &AEditorController::SetToolMode<EToolMode::Rotation>);

	// Delete button
	InputComponent->BindAction("Delete", IE_Pressed, this, &AEditorController::DeleteSelectedObject);
}

/**
 * @brief Initializes values on the event of a mouse click, casting rays to get object or gizmo data.
 *
 */
void AEditorController::OnMouseLeftPressed()
{
	if (PC == nullptr) return;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, (TEXT("LEEEEEEEEEEEEEEEEEEEEEEEEEEEEFT")));
	}
	// Check if there is a gizmo under the cursor
	FHitResult GizmoHit;
	PC->GetHitResultUnderCursorByChannel(GizmoTraceTypeQuery, true, GizmoHit);

	// If a gizmo is found, set gizmo to be visible and set the values of mouse position and object rotation on click
	if (GizmoHit.bBlockingHit)
	{
		SaveMouseLocation();

		UpdateGizmoVisibilityAndCollision();

		if (SelectedObject != nullptr)
		{
			OffsetOnClick = GizmoHit.ImpactPoint - SelectedObject->GetActorLocation();
			ObjectRotationOnClick = SelectedObject->GetActorRotation().Yaw;
			bGrabbing = true;
		}

		// Get a reference to the static mesh of each gizmo to set the selected axis to the relevant axis
		const UPrimitiveComponent* Component = GizmoHit.GetComponent();

		if (Component == ArrowX) SelectedAxis = EAxisList::X;
		else if (Component == ArrowY) SelectedAxis = EAxisList::Y;
		else if (Component == ArrowZ) SelectedAxis = EAxisList::Z;
		else if (Component == PlaneXY) SelectedAxis = EAxisList::XY;
		else if (Component == DiscZ) SelectedAxis = EAxisList::None;
	}
	
	// Check if an object is under the cursor 
	FHitResult ObjectHit;
	PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, ObjectHit);

	if (ObjectHit.bBlockingHit)
	{
		AActor* HitActor = ObjectHit.GetActor();

		UPrimitiveComponent* HitComponent = ObjectHit.GetComponent();

		// If the clicked actor is an editor object, save it as the selected object and move the gizmo to the object
		if (HitActor->ActorHasTag(FName("EditorObject")))
		{
			HitActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
			SetSelectedObject(HitActor);
			
		} 
		else if (HitComponent->ComponentHasTag(FName("EditorSpline")))
		{
			SetSelectedObject(HitComponent->GetOwner());
			// Get the generic actor component

			if (SplineActor != nullptr)
			{
				float SplineInputKey = SplineActor->SplineComponent->FindInputKeyClosestToWorldLocation(ObjectHit.Location);
				int SplinePoint = FMath::FloorToInt(SplineInputKey);
				FVector SplinePointLocation = SplineActor->SplineComponent->GetLocationAtSplinePoint(SplinePoint, ESplineCoordinateSpace::World);
				GizmoComponent->SetWorldLocation(SplinePointLocation);

				
				SelectedTransform = SplineActor->SplineComponent->GetTransformAtSplinePoint(SplinePoint, ESplineCoordinateSpace::World);
				//GizmoComponent->SetWorldLocation(SelectedTransform.GetLocation());
				GizmoComponent->SetWorldLocation(SplineActor->SplineComponent->GetLocationAtSplinePoint(SplinePoint, ESplineCoordinateSpace::World));
			}

		}
		else if (!bGrabbing)
		{
			SetSelectedObject(nullptr);
		}

	}
	else if (!bGrabbing)
	{
		SetSelectedObject(nullptr);
	}

}

/**
 * @brief Sets grabbing to false if the left mouse button is released.
 * 
 */
void AEditorController::OnMouseLeftReleased()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, (TEXT("LEEEEEEEEEEEEEFT RELEASED")));
	}
	bGrabbing = false;
	SelectedAxis = EAxisList::None;

	if (SplineActor == nullptr) return;
	SplineActor->OnConstruction(SplineActor->GetTransform());
}

/**
 * @brief Enables rotation of the camera if the right mouse button is held,
 * and saves the current mouse location.
 *
 */
void AEditorController::OnMouseRightPressed()
{
	if (PC == nullptr) return;

	FVector rot = GetActorForwardVector();
	bRotateCamera = true;

	if (PC)
	{
		PC->bShowMouseCursor = false;
		SaveMouseLocation();
	}
}

/**
 * @brief Disables rotation of the camera if the right mouse button is released,
 * and sets mouse location to its location at the time of pressing the right mouse button.
 *
 */
void AEditorController::OnMouseRightReleased()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, (TEXT("RIIIIIIIIIIIIGHT Released")));
	}
	bRotateCamera = false;

	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->SetMouseLocation(MouseLocationOnClick.X, MouseLocationOnClick.Y);
	}
}

/**
 * @brief Saves the current mouse location to MouseLocationOnClick.
 * 
 */
void AEditorController::SaveMouseLocation() 
{
	float MouseX, MouseY;
	PC->GetMousePosition(MouseX, MouseY);

	MouseLocationOnClick = FVector(MouseX, MouseY, 0);
}

/**
 * @brief Moves the player along its local Y-axis from axis input.
 * 
 * @param AxisValue The positive or negative axis value from the defined input buttons.
 */
void AEditorController::MoveForward(float AxisValue)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), AxisValue);
	
}

/**
 * @brief Moves the player along its local X-axis from axis input.
 *
 * @param AxisValue The positive or negative axis value from the defined input buttons.
 */
void AEditorController::MoveStrafe(float AxisValue)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), AxisValue);
}

/**
 * @brief Moves the player along the world Z-axis from axis input.
 *
 * @param AxisValue The positive or negative axis value from the defined input buttons.
 */
void AEditorController::MoveVertical(float AxisValue)
{
	AddMovementInput(FVector::UpVector, AxisValue);
}

/**
 * @brief Changes the users move speed based on input from the mouse wheel.
 *
 * @param AxisValue The positive or negative axis value from the mouse wheel.
 */
void AEditorController::ChangeMoveSpeed(float AxisValue)
{
	if (PawnMovement == nullptr) return;

	MoveSpeed = FMath::Clamp(MoveSpeed + (100.0f * AxisValue), 100.0f, MaxMoveSpeed);
	PawnMovement->MaxSpeed = MoveSpeed;
	
}
/**
 * @brief Rotates the player around the Z-axis based on mouse input if RotateCamera is true.
 *
 * @param AxisValue The horizontal value of the mouse input.
 */
void AEditorController::MoveYaw(float AxisValue)
{
	if (bRotateCamera)
	{
		AddControllerYawInput(AxisValue * LookSensistivity);

	}
}

/**
 * @brief Rotates the player around the X-axis based on mouse input if RotateCamera is true.
 *
 * @param AxisValue The vertical value of the mouse input.
 */
void AEditorController::MovePitch(float AxisValue)
{
	if (bRotateCamera)
	{
		AddControllerPitchInput((AxisValue * LookSensistivity) * -1);
	}
}

/**
 * @brief Sets the current mode to the given tool mode.
 * 
 */
template<const EToolMode NewToolMode>
void AEditorController::SetToolMode()
{
	ToolMode = NewToolMode;
	UpdateGizmoVisibilityAndCollision();
}


/**
 * @brief Updates the visibility and collision of all gizmo meshes based on the current tool mode.
 * 
 */
void AEditorController::UpdateGizmoVisibilityAndCollision()
{
	// If there is no selected object, disable all gizmo meshes
	if (SelectedObject == nullptr)
	{
		SetMeshEnabled({ ArrowX, ArrowY, ArrowZ, PlaneXY, DiscZ }, false);
		return;
	}

	// Check tool mode to see which gizmo meshes to enable
	switch (ToolMode)
	{
		case EToolMode::Translation:
			SetMeshEnabled({ ArrowX, ArrowY, ArrowZ, PlaneXY }, true);
			SetMeshEnabled({ DiscZ }, false);
			break;

		case EToolMode::Rotation:
			SetMeshEnabled({ ArrowX, ArrowY, ArrowZ, PlaneXY }, false);
			SetMeshEnabled({ DiscZ }, true);
			break;

		default:
			break;
	}
}

/**
 * @brief Sets the visibility and enables or disables the collision of
 * the passed list of mesh components..
 * 
 * @param Meshes An arbitrarily long list of UStaticMeshComponent
 * @param Enabled Whether or not the mesh(es) should be enabled or disabled
 */
void AEditorController::SetMeshEnabled(const std::initializer_list<UStaticMeshComponent*> Meshes, bool Enabled) 
{
	for (UStaticMeshComponent* Mesh : Meshes)
	{
		Mesh->SetVisibility(Enabled);
		Mesh->SetCollisionEnabled(Enabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

/**
 * @brief Sets the material of the referenced object to display an outline.
 * 
 * @param Object The relevant object to highlight
 * @param Value Whether the object highlight should be enabled (true) or disabled (false)
 */
void AEditorController::SetObjectHighlight(AActor* Object, bool Value)
{
	if (Object == nullptr) return;

	// Get the generic actor component
	TArray<UActorComponent*> ActorComponents = Object->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	if (ActorComponents.Num() == 0) return;

	// Cast the component to a static mesh
	for (UActorComponent* ActorComponent : ActorComponents)
	{
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(ActorComponent);
		if (StaticMesh == nullptr) continue;
		StaticMesh->SetRenderCustomDepth(Value);
	}

}

/**
 * @brief Updates the reference to the selected object and disables highlighting of the old selected object.
 * 
 * @param Object The new object to save as the selected object.
 */
void AEditorController::SetSelectedObject(AActor* Object)
{
	// If nothing was selected, disable the highlighting of the selected object if valid.
	if (Object == nullptr)
	{
		SetObjectHighlight(SelectedObject, false);
	}
	else
	{
		GizmoComponent->SetWorldLocation(Object->GetActorLocation());
	}

	// If the new selected object is different from the current, disable the old one and highlight the new object.
	if (Object != SelectedObject)
	{
		SetObjectHighlight(SelectedObject, false);
		SetObjectHighlight(Object, true);
	}

	SelectedObject = Object;

	UpdateGizmoVisibilityAndCollision();
}

/**
 * @brief Deletes the selected objects and resets the gizmo.
 * 
 */
void AEditorController::DeleteSelectedObject() 
{
	if (SelectedObject != nullptr)
	{
		SelectedObject->Destroy();
	}

	SetSelectedObject(nullptr);
}

/**
 * @brief Sets the colors of the gizmos back to default.
 * 
 */
void AEditorController::SetGizmoDefaultMaterials()
{
	ArrowX->SetMaterial(0, GizmoRed);

	ArrowY->SetMaterial(0, GizmoGreen);

	ArrowZ->SetMaterial(0, GizmoBlue);

	PlaneXY->SetMaterial(0, GizmoRed);
	PlaneXY->SetMaterial(1, GizmoGreen);

	DiscZ->SetMaterial(0, GizmoBlue);
}

USplineComponent* AEditorController::GetNearestRailwaySplineComponent()
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
	return (ClosestRailway == nullptr) ? nullptr : ClosestRailway->SplineComponent;
}

