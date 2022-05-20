// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "EditorObject.h"

// Sets default values
AEditorObject::AEditorObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("EditorObject"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	

	TransformWidgetComponent = CreateDefaultSubobject<UTransformWidgetComponent>(TEXT("Transform Widget Component"));

}

// Called when the game starts or when spawned
void AEditorObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEditorObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

