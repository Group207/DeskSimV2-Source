// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "TrainDMI.generated.h"

/**
 * 
 */
UCLASS()
class DESKSIMV2_API UTrainDMI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UTrainDMI(const FObjectInitializer& ObjectInitializer);

	void Update(float Speed);

	UFUNCTION()
		void UpdateDMI(float Speed, FText SpeedText);
	void Hide();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UImage* needle; ///< Binding for the speedometer needle
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UEditableTextBox* kmtText; ///< Binding for the speed text
};
