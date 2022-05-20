// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "TransformWidgetComponent.h"

// Sets default values for this component's properties
UTransformWidgetComponent::UTransformWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OriginSphere = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Origin Sphere"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	OriginSphere->SetStaticMesh(SphereMeshAsset.Object);
	OriginSphere->SetVisibility(false);


	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Models/Editor/TransformArrow.TransformArrow'"));
	UStaticMesh* ArrowMesh = MeshAsset.Object;

	ArrowX = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Transform Arrow X"));
	ArrowX->AttachTo(OriginSphere);
	ArrowX->SetStaticMesh(ArrowMesh);

	ArrowY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Transform Arrow Y"));
	ArrowY->AttachTo(OriginSphere);
	ArrowY->SetStaticMesh(ArrowMesh);
	ArrowY->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));

	ArrowZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Transform Arrow Z"));
	ArrowZ->AttachTo(OriginSphere);
	ArrowZ->SetStaticMesh(ArrowMesh);
	ArrowZ->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));

}


// Called when the game starts
void UTransformWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Parent = GetOwner();

	if (Parent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("The owner is invalid!"));
		return;
	}

	FVector ParentLocation = FVector::ZeroVector;
	ParentLocation = Parent->GetActorLocation();

	OriginSphere->SetWorldLocation(ParentLocation);
	OriginSphere->SetWorldScale3D(FVector(0.1f));
	
}


// Called every frame
void UTransformWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

