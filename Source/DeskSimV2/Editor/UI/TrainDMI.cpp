// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include <DeskSimV2/Editor/UI/TrainDMI.h>
#include <DeskSimV2/Trains/Train.h>
#include "Kismet/GameplayStatics.h"

UTrainDMI::UTrainDMI(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

/**
 * @brief Updates the needle in the train DMI.
 * 
 * @param Speed - The current speed of the train.
 *
 * @see UpdateDMI();
 */
void UTrainDMI::Update(float Speed)
{
	if (Speed >= 0.0)
	{
		float RenderAngleDriving = (Speed * 0.1) + (Speed - 144);

		int Text = FMath::FloorToInt(Speed);
		FString SpeedText = FString::SanitizeFloat(Text, 0);

		UpdateDMI(RenderAngleDriving, FText::FromString(SpeedText));
	}
	else
	{

		int Text = FMath::FloorToInt(Speed);
		FString SpeedText = FString::SanitizeFloat(Text);

		UpdateDMI(Speed - 144, FText::FromString(SpeedText));
	}
}

/**
 * @brief Settign the needle in the DMI widget and the speed text.
 * 
 * @param Speed
 * @param SpeedText
 */
void UTrainDMI::UpdateDMI(float Speed, FText SpeedText)
{
	if (needle)
	{
		needle->SetRenderTransformAngle(Speed);
	}
	if (kmtText->IsValidLowLevel())
	{
		kmtText->SetText(SpeedText);
	}
}

/**
 * @briefHides the DMI from the viewport.
 * 
 */
void UTrainDMI::Hide()
{
	RemoveFromViewport();
}
