// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransformWidgetComponent.h"
#include "EditorObject.generated.h"

UCLASS()
class DESKSIMV2_API AEditorObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEditorObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UTransformWidgetComponent* TransformWidgetComponent;
};
