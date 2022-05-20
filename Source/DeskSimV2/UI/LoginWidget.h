// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Components/TextBlock.h>
#include <Components/EditableTextBox.h>
#include <Components/Button.h>

#include <DeskSimV2/GameModes/DesksimGameInstance.h>

#include <VaRest.h>
#include <VaRestSubsystem.h>
#include <VaRestJsonObject.h>
#include <VaRestJsonValue.h>

/*
#include <HTTP/Public/Http.h>
#include <Json/Public/Json.h>
#include <JsonUtilities/Public/JsonUtilities.h>
*/

#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"



/**
 * 
 */
UCLASS()
class DESKSIMV2_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	ULoginWidget(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

	UFUNCTION()
	void UpdateInfoText(FString Text);

	UFUNCTION()
	void OnLoginClicked();

	void CallSignIn();

	UFUNCTION()
	void SignInCallback(UVaRestRequestJSON* Request);

	void CallUserLogin(FString JWT);

	UFUNCTION()
	void UserLoginCallback(UVaRestRequestJSON* Request);

	void ReadUserData(UVaRestJsonObject* ResponseObject);

	void SwitchToMainMenu();

	UFUNCTION()
	void QuitGame();

	// ----------- UI components ------------

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	UEditableTextBox* UsernameField;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	UEditableTextBox* PasswordField;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	UTextBlock* InfoTextBox;

	// --------------------------------------

	/// VaRest Subsystem reference
	UVaRestSubsystem* RestSubSys;

	// Base url of the api
	FString ApiBaseUrl;

	// Class reference to Main menu widget
	UPROPERTY(EditDefaultsOnly, Category = "Login")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	/// Delegate used for SignIn response
	FVaRestCallDelegate SignInDelegate;

	/// Delegate used for UserLogin response
	FVaRestCallDelegate UserLoginDelegate;

};
