// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DeskSimV2/SaveGame/IsSaveableInterface.h>
#include "Wagon.generated.h"


UCLASS()
class DESKSIMV2_API AWagon : public AActor, public IIsSaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWagon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleMesh;


	void SetPosition(FVector Position);
	void SetRotation(FVector Rotation);


};
