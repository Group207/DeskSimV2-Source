// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "PopUpWidget.h"
#include "Components/TextBlock.h"

/**
 * @brief Loading the image sources used for the popup messages.
 * 
 * @param ObjectInitializer
 */
UPopUpWidget::UPopUpWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageMessage(TEXT("/Game/Models/Images/Editor/Popup/Message.Message"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageWarning(TEXT("/Game/Models/Images/Editor/Popup/Warning.Warning"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> ImageCritical(TEXT("/Game/Models/Images/Editor/Popup/Critical.Critical"));
	
	if (ImageMessage.Succeeded() && ImageWarning.Succeeded() && ImageCritical.Succeeded())
	{
		Images.Add(EPopupType::Message, ImageMessage.Object); Images.Add(EPopupType::Warning, ImageWarning.Object); Images.Add(EPopupType::Critical, ImageCritical.Object);
	}
}

/**
 * @brief Setting the popup text and image from parameters.
 * 
 * @param Type - The popup type (message | warning | Critical)
 * @param Description - The message to display
 */
void UPopUpWidget::SetPopUpType(EPopupType Type, FString Description)
{
	switch (Type)
	{
	case Message: PopupTitle->SetText(FText::FromString("Message")); 
		break;
	case Warning: PopupTitle->SetText(FText::FromString("Warning"));
		break;
	case Critical: PopupTitle->SetText(FText::FromString("Critical"));
		break;
	default: 
		break;
		
	}
	PopupDescription->SetText(FText::FromString(Description));
	PopupIcon->SetBrushFromTexture(*Images.Find(Type));
}
