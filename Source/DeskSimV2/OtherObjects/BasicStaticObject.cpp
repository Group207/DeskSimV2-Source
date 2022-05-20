// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "BasicStaticObject.h"

// Sets default values
ABasicStaticObject::ABasicStaticObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = VisualMesh;

	Tags.Add("EditorObject");

}

// Called when the game starts or when spawned
void ABasicStaticObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicStaticObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

