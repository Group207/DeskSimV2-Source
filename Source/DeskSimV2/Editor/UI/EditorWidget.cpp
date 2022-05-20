// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#include "EditorWidget.h"


UEditorWidget::UEditorWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

/**
 * @brief Function for initializing class members.
 * 
 */
void UEditorWidget::InitializeNativeClassData()
{
	Super::InitializeNativeClassData();

}

/**
 * @brief Setting the Train button index to the active index.
 * 
 */
void UEditorWidget::TrainBtnOnClicked()
{
	if (ObjectSwitcher) {
		ObjectSwitcher->SetActiveWidgetIndex(0);
	}
}

/**
 * @brief Setting the Railway button index to the active index.
 *
 */
void UEditorWidget::RailwayBtnOnClicked()
{
	if (ObjectSwitcher) {
		ObjectSwitcher->SetActiveWidgetIndex(1);
	}

}

/**
 * @brief Setting the House button index to the active index.
 *
 */
void UEditorWidget::HouseBtnOnClicked()
{
	if (ObjectSwitcher) {
		ObjectSwitcher->SetActiveWidgetIndex(2);
	}
}

/**
 * @brief Setting the Signal button index to the active index.
 *
 */
void UEditorWidget::SignalBtnOnClicked()
{
	if (ObjectSwitcher) {
		ObjectSwitcher->SetActiveWidgetIndex(3);
	}
}

/**
 * @brief Setting the Other button index to the active index.
 *
 */
void UEditorWidget::OtherBtnOnClicked()
{
	if (ObjectSwitcher) {
		ObjectSwitcher->SetActiveWidgetIndex(4);
	}
}






