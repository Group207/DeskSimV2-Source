// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PopUpWidget.h"
#include "TopBar.generated.h"

/**
 * 
 */
UCLASS()
class DESKSIMV2_API UTopBar : public UUserWidget
{
	GENERATED_BODY()


	
public:

	UTopBar(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SaveButton; ///< Binding for save button in the top bar

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ScaleButton; ///< Binding for scaling button in the top bar

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* TranslateButton; ///< Binding for translate button in the top bar

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* RotateButton; ///< Binding for rotate button in the top bar

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* MainMenuButton; ///< Binding for main menu button in the top bar

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* TrashButton; ///< Binding for trash button in the top bar

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> PopupWidgetClass;///< Pop-Up userwidget class

	UFUNCTION()
		void ChangeToMainMenu();
	UFUNCTION()
		void OkClicked();

	bool WantToQuit;
protected:
	APlayerController* PC; ///< Current Player Controller of the level
	UPopUpWidget* Popup;   ///< Saved Popup widget
};

