// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggedObject.generated.h"

/**
 * 
 */
UCLASS()
class DESKSIMV2_API UDraggedObject : public UUserWidget
{
	GENERATED_BODY()

public: 
	UDraggedObject(const FObjectInitializer& ObjectInitializer);	
};
