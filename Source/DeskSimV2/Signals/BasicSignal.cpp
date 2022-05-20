// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "BasicSignal.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasicSignal::ABasicSignal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up the visual mesh component
	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = VisualComponent;

	// Sets up the default material
	ConstructorHelpers::FObjectFinder<UMaterial> LightMaterialAsset(TEXT("/Game/Materials/Emissive_Light_Customizable.Emissive_Light_Customizable"));

	if (LightMaterialAsset.Succeeded())
	{
		BaseLightMaterial = LightMaterialAsset.Object;
	}

	// Sets up the default light mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> LightMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (LightMeshAsset.Succeeded())
	{
		BaseLightMesh = LightMeshAsset.Object;
	}

	Tags.Add("EditorObject");

}

// Called when the game starts or when spawned
void ABasicSignal::BeginPlay()
{
	Super::BeginPlay();

	//Tags.Add(SignalTag);

	SetupLight();

	if (bSnapToNearestRailway)
	{
		SnapToNearestRailway();
	}

	UpdateSignalStatus(SignalStatus);
}

// Called when game stops of when despawned
void ABasicSignal::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	// Remove UStructs here
	// Clear lights
	RemoveLights();


	Super::EndPlay(EndPlayReason);
}


// Called every frame
void ABasicSignal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/**
 * @brief Sets the light meshes and dynamic materials for the signals.
 * 
 * Does this dynamically with sockets, allows for custom amounts of lights.
 * Clears the previous lights to prevent duplicates
 * 
 * @see RemoveLights
 */
void ABasicSignal::SetupLight()
{
	RemoveLights();

	// Create all lights in a loop
	for (int32 i = 0; i < NumLights; i++)
	{
		FString SocketName = FString::Printf(TEXT("Socket_%i"), i + 1);
		FString LightMeshName = FString::Printf(TEXT("LightMesh_%i"), i + 1);

		UStaticMeshComponent* LightMesh = NewObject<UStaticMeshComponent>(this, FName(LightMeshName));

		// Creates a new light struct
		FSignalLight NewLight(LightMesh);

		NewLight.LightMesh->SetStaticMesh(BaseLightMesh);

		NewLight.LightMesh->SetupAttachment(VisualComponent, FName(SocketName));

		// Creates the instanced dynamic light material
		NewLight.DynMaterial = UMaterialInstanceDynamic::Create(BaseLightMaterial, this);

		NewLight.DynMaterial->SetScalarParameterValue("Emissive_Strength", MaxLightLevel);
		NewLight.DynMaterial->SetVectorParameterValue("Emissive_Color", FLinearColor(1.0f, 1.0f, 1.0f));

		NewLight.LightMesh->SetMaterial(0, NewLight.DynMaterial);

		NewLight.LightMesh->RegisterComponent();


		Lights.Add(NewLight);
	}

}

/**
 * @brief Unregisters and removes the light components.
 *
 */
void ABasicSignal::RemoveLights()
{
	for (FSignalLight& SignalLight : Lights)
	{
		SignalLight.LightMesh->UnregisterComponent();
	}
	Lights.Empty();
}

/**
 * @brief Sets the light at index to be on or off.
 *
 * @param InLightIndex - Index of the light, 0 indexed
 * @param bLightOn - True for On, False for Off
 */
void ABasicSignal::SetLightOnOff(int32 InLightIndex, bool bInLightOn)
{
	// Checks if the index is out of range	
	Lights.RangeCheck(InLightIndex);

	if (bInLightOn)
	{
		Lights[InLightIndex].DynMaterial->SetScalarParameterValue("Emissive_Strength", MaxLightLevel);
	}
	else
	{
		Lights[InLightIndex].DynMaterial->SetScalarParameterValue("Emissive_Strength", 0.0f);
	}
}

/**
 * @brief Toggles one of the lights on or off.
 *
 * @param InLightIndex - Index of the light to toggle, 0 indexed
 */
void ABasicSignal::ToggleLight(int32 InLightIndex)
{
	// Checks if the index is out of range
	Lights.RangeCheck(InLightIndex);

	float DynMaterialStrength = Lights[InLightIndex].DynMaterial->K2_GetScalarParameterValue("Emissive_Strength");

	if (DynMaterialStrength == 0.0f)
	{
		SetLightOnOff(InLightIndex, true);
	}
	else
	{
		SetLightOnOff(InLightIndex, false);
	}
}

/**
 * @brief Sets the color of the light at InLightIndex.
 *
 * @param InLightIndex - The index of the light, 0 indexed
 * @param InColor - The color of the light
 */
void ABasicSignal::SetLightColor(int32 InLightIndex, FLinearColor InColor)
{
	// Checks if the index is out of range
	Lights.RangeCheck(InLightIndex);

	Lights[InLightIndex].DynMaterial->SetVectorParameterValue("Emissive_Color", InColor);
}


/**
 * @brief Sets a light to blink at the specified frequency.
 *
 * @param InLightIndex - Index of the light, 0 indexed
 * @param InSeconds - Seconds between on and off states
 */
void ABasicSignal::StartLightBlink(int32 InLightIndex, float InSeconds)
{
	// Checks if the index is out of range
	Lights.RangeCheck(InLightIndex);
	
	// Creates a timer delegate to call the function
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("ToggleLight"), InLightIndex);

	GetWorldTimerManager().SetTimer(Lights[InLightIndex].LightTimerHandle, TimerDel, InSeconds, true);
}

/**
 * @brief Stops a light from blinking.
 *
 * @param InLightIndex - Index of the light, 0 indexed
 */
void ABasicSignal::StopLightBlink(int32 InLightIndex)
{
	// Checks if the index is out of range
	Lights.RangeCheck(InLightIndex);

	GetWorldTimerManager().ClearTimer(Lights[InLightIndex].LightTimerHandle);
}


/**
* @brief Locates the USplineComponent object nearest the 
* location of a given vector in world space
* 
* @return Returns a pointer to the relevant USplineComponent if any
*/
USplineComponent* ABasicSignal::GetNearestRailwaySplineComponent()
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

/**
* @brief Snaps this object to the nearest railway, if any
*/
void ABasicSignal::SnapToNearestRailway()
{

	FVector WorldLocation = GetActorLocation();
	USplineComponent* Railway = GetNearestRailwaySplineComponent();

	if (Railway == nullptr) return;

	FTransform ClosestSplineTransform = Railway->FindTransformClosestToWorldLocation(WorldLocation, ESplineCoordinateSpace::World);

	FVector Offset = Railway->FindRightVectorClosestToWorldLocation(WorldLocation, ESplineCoordinateSpace::World) * OffsetFromRailway;

	SetActorLocationAndRotation(ClosestSplineTransform.GetLocation() + Offset, ClosestSplineTransform.GetRotation());
	
}

