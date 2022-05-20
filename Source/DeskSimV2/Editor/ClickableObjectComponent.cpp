// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "ClickableObjectComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UClickableObjectComponent::UClickableObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UClickableObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AActor* Owner = GetOwner();

	if (Owner == nullptr) return;

	StaticMesh = Owner->GetComponentByClass(UStaticMesh::StaticClass());

	if (StaticMesh == nullptr) return;

	Owner->bBlockInput = false;
	Owner->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}


// Called every frame
void UClickableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

