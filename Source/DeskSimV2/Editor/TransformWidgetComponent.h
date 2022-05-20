// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "TransformWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESKSIMV2_API UTransformWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTransformWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UStaticMeshComponent* OriginSphere;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowX;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowY;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowZ;

private:
	bool bSelected = false;
	EAxis::Type SelectedAxis = EAxis::None;
};
