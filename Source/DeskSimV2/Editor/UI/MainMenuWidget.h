// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "MainMenuWidget.generated.h"

/**
 * The main menu widget
 */
UCLASS()
class DESKSIMV2_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public: 

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

	void SetButtons();
	
	UFUNCTION()
	void QuitGame();
	
	UFUNCTION()
	void LogOut();

	UFUNCTION()
	void ChangeLayout(bool mode);

	UFUNCTION()
	void ATCClicked();
	
	UFUNCTION()
	void ERTMSClicked();
	
	UFUNCTION()
	void INFRClicked();
	
	UFUNCTION()
	void FEILClicked();
	
	UFUNCTION()
	void SKIFClicked();
	
	UFUNCTION()
	void EKSTClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* ScenarioSwitcher; ///< The widget switcher binding from the blueprint widget

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* EditorBtn; ///< Editor mode button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* MainMenuBtn; ///< Main Menu mode button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* SettingsBtn; ///< Settings button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* QuitBtn; ///< Quit button binding

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* LogoutBtn; ///< Log out button binding

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* ATCBtn; ///< ATC button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* ERTMSBtn; ///< ERTMS button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* INFRBtn; ///< INFRASTRUKTUR button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* FEILBtn; ///< FEIL button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* SKIFBtn; ///< SKIFTING button binding
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UButton* EKSTBtn; ///< EKSTERN button binding

};
