// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenu.generated.h"

/**
 * Class that represents the pause menu.
 */
UCLASS()
class DESKSIMV2_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupButtons();
	UFUNCTION()
		void PauseGame();
	UFUNCTION()
		void OnExitPressed();
	UFUNCTION()
		void OnResumePressed();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UButton* ResumeButton; ///< Resume game button
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UButton* SettingsButton; ///< Open settings button
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UButton* ExitButton; ///< Exit game to main menu button

};
