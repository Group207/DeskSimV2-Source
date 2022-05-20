// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PopUpWidget.generated.h"


UENUM()
enum EPopupType {
	Message  = 1,
	Warning  = 2,
	Critical = 3,
};

/**
 * Popup message class
 */
UCLASS()
class DESKSIMV2_API UPopUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPopUpWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
		void SetPopUpType(EPopupType Type, FString Description);

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UImage* PopupIcon; ///< Binding for image in popup widget
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UTextBlock* PopupTitle; ///< Binding for title text in popup widget
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UTextBlock* PopupDescription; ///< Binding for description text in popup widget
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* PopupOkButton; ///< Binding for OK button in popup widget
	                   
protected: 
	TMap<EPopupType, UTexture2D*> Images; ///< The image to display

};
