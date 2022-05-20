// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "Wagon.h"

// Sets default values
AWagon::AWagon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	VisibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisibleMesh->SetupAttachment(RootComponent);

	Tags.Add("EditorObject");

}

// Called when the game starts or when spawned
void AWagon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWagon::SetPosition(FVector Position)
{

	SetActorLocation(Position);
}

void AWagon::SetRotation(FVector Rotation)
{
	SetActorRotation(Rotation.Rotation());
}

