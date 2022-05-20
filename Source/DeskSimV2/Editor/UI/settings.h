// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "settings.generated.h"

/**
 * The settings widget class.
 */
UCLASS()
class DESKSIMV2_API Usettings : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void ChangeResolution(FString sItem, ESelectInfo::Type seltype);
	
	UFUNCTION()
	void ChangeScreenMode(FString sItem, ESelectInfo::Type seltype);
	
	UFUNCTION()
	void CloseSettings();
	
	UFUNCTION()
	void SetupSettings();
	         

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UComboBoxString* ResolutionDropdown; ///< Binding for the Screen resolution widget
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UComboBoxString* WindowModeDropdown; ///< Binding for the window mode widget
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class USlider* SliderVolume; ///< Binding for volume slider
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* QuitButton; ///< Back to main menu button
};
