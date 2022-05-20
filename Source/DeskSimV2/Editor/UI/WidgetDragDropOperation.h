// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class DESKSIMV2_API UWidgetDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUserWidget* DraggedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D ScreenOffset;
	
};
