// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetSwitcher.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "EditorWidget.generated.h"


/**
 * The content browser widget.
 */
UCLASS()
class DESKSIMV2_API UEditorWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UEditorWidget(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeNativeClassData() override;

	// For the Content browser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSwitcher* ObjectSwitcher; ///< The widget switcher binding from the blueprint widget

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* TrainButton; ///< Train button binding

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* RailwayButton; ///< Railway button binding

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* HouseButton; ///< House button binding

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* SignalButton; ///< signal button binding

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* OtherButton; ///< Other button binding

	// Button functions
	void TrainBtnOnClicked();
	void RailwayBtnOnClicked();
	void HouseBtnOnClicked();
	void SignalBtnOnClicked();
	void OtherBtnOnClicked();
};
