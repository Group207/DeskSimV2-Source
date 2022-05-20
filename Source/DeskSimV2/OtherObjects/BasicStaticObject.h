// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DeskSimV2/SaveGame/IsSaveableInterface.h>
#include "BasicStaticObject.generated.h"

UCLASS()
class DESKSIMV2_API ABasicStaticObject : public AActor, public IIsSaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicStaticObject();

	UPROPERTY(EditDefaultsOnly, Category = "Static Mesh")
		UStaticMeshComponent* VisualMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
