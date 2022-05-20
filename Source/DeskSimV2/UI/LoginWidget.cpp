// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "LoginWidget.h"
#include <Kismet/GameplayStatics.h>
#include <DeskSimV2/Editor/UI/EditorHUD.h>

ULoginWidget::ULoginWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ApiBaseUrl = "https://desksim.lokforerskolen.no/api/";

}

// onplay equivalent
void ULoginWidget::NativeOnInitialized()
{
	// Binds the callback functions for the requests
	SignInDelegate.BindUFunction(this, FName("SignInCallback"));
	UserLoginDelegate.BindUFunction(this, FName("USerLoginCallback"));

	// Binds Login Onclick
	if (LoginButton) LoginButton->OnClicked.AddDynamic(this, &ULoginWidget::OnLoginClicked);

	if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &ULoginWidget::QuitGame);

	// Sets the VaRestSubsystem reference
	if (GEngine)
	{
		RestSubSys = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
	}
	else
	{
		checkNoEntry()
	}
}

/**
 * @brief Updates info text box in widget.
 * 
 * @param Text - String to set
 */
void ULoginWidget::UpdateInfoText(FString Text)
{
	// Update text widget content
	InfoTextBox->SetText(FText::FromString(Text));
}

/**
 * @brief Called when the login button is clicked.
 * 
 */
void ULoginWidget::OnLoginClicked()
{
	CallSignIn();
}

/**
 * @brief Creates and sends a request to the SignIn endpoint.
 * 
 */
void ULoginWidget::CallSignIn()
{

	FText Username = UsernameField->GetText();
	FText Password = PasswordField->GetText();

	// Checks if the input is empty
	if (Username.IsEmpty())
	{
		UpdateInfoText(TEXT("Kan ikke ha tomt brukernavn"));
		return;
	}
	if (Password.IsEmpty())
	{
		UpdateInfoText(TEXT("Kan ikke ha tomt passord"));
		return;
	}

	// Creates a request object
	UVaRestJsonObject* RequestJSON = RestSubSys->ConstructVaRestJsonObject();

	RequestJSON->SetStringField("username", Username.ToString());
	RequestJSON->SetStringField("password", Password.ToString());

	FString URL = ApiBaseUrl + "authentication/SignIn/";

	RestSubSys->CallURL(URL, 
		EVaRestRequestVerb::POST, 
		EVaRestRequestContentType::json, 
		RequestJSON, 
		SignInDelegate);
}

/**
 * @brief Callback for when the Sign in responds.
 * 
 * @param Response - Response object
 */
void ULoginWidget::SignInCallback(UVaRestRequestJSON* Response)
{
	int32 ResponseCode = Response->GetResponseCode();
	
	
	switch (ResponseCode)
	{
	case 200:
		CallUserLogin(Response->GetResponseContentAsString());
		break;
	case 400:
		// Bad request
		// decode json to give error?
		UpdateInfoText(TEXT("Error: Bad request"));
		break;
	case 401:
		// Unauthorized
		UpdateInfoText(TEXT("Error: Unauthorized user"));
		break;
	case 500:
		// Internal server error
		UpdateInfoText(TEXT("Error: Internal server error"));
		break;
	default:
		// print error to screen
		UpdateInfoText(FString::Printf(TEXT("Error code: %i"), ResponseCode));
		break;
	}

}

/**
 * @brief Creates and executes a request to the UserLogin endpoint.
 * 
 * @param JWT - JSON Web Token used to retrieve the user object
 */
void ULoginWidget::CallUserLogin(FString JWT)
{
	UVaRestRequestJSON* Request = RestSubSys->ConstructVaRestRequest();

	FString URL = ApiBaseUrl + "users/UserLogin/";
	FString Token = "Bearer " + JWT;

	// Sets info and header for the request
	Request->SetURL(URL);
	Request->SetVerb(EVaRestRequestVerb::GET);
	Request->SetHeader("Authorization", Token);

	// Callback delegate
	Request->OnRequestComplete.Add(UserLoginDelegate);

	Request->ExecuteProcessRequest();

}

/**
 * @brief Callback for when UserLogin responds.
 * 
 * @param Response - Response object
 */
void ULoginWidget::UserLoginCallback(UVaRestRequestJSON* Response)
{
	UVaRestJsonObject* ResponseObject = Response->GetResponseObject();

	int32 ResponseCode = Response->GetResponseCode();

	switch (ResponseCode)
	{
	case 200: 
	{
		// Reads and stores the relevant response data
		ReadUserData(ResponseObject);
		
		SwitchToMainMenu();

		break;
	}
	case 400:
		// Bad request
		UpdateInfoText(TEXT("Error: 400 Bad request"));
		break;
	case 401:
		// Unauthorized
		UpdateInfoText(TEXT("Error: 401 Unauthorized"));
		break;
	case 500:
		// Internal server error
		UpdateInfoText(TEXT("Error: Internal server error"));
		break;
	default:
		// Unknown code
		UpdateInfoText(FString::Printf(TEXT("Error code: %i"), ResponseCode));
		break;
	}
}

/**
 * @brief Reads the UserLogin response and fills in UserInfo in the gameinstance.
 * 
 * @param JsonObj - Pointer to the JSON object
 */
void ULoginWidget::ReadUserData(UVaRestJsonObject* JsonObj)
{
	UDesksimGameInstance* GameInstance = Cast<UDesksimGameInstance>(GetGameInstance());

	GameInstance->bIsLoggedIn = true;

	// Set Userinfo in gameinstance
	GameInstance->UserInfo.bIsAuthenticated = true;
	GameInstance->UserInfo.bIsEnabled = JsonObj->GetBoolField(TEXT("isEnabled"));
	GameInstance->UserInfo.id = JsonObj->GetIntegerField(TEXT("id"));
	GameInstance->UserInfo.Username = FName(*JsonObj->GetStringField(TEXT("username")));
	GameInstance->UserInfo.SubscriptionID = JsonObj->GetIntegerField(TEXT("subscriptionId"));

	// Find info about subscription and user groups
	UVaRestJsonObject* Subscription = JsonObj->GetObjectField(TEXT("subscription"));
	
	TArray<FString> GroupNames = Subscription->GetStringArrayField(TEXT("userGroupNames"));

	// Checks which type the user is
	if (GroupNames.Contains("Administrator"))
	{
		GameInstance->UserInfo.UserType = EUserType::Admin;
	}
	else if (GroupNames.Contains("Ansatte"))
	{
		GameInstance->UserInfo.UserType = EUserType::Teacher;
	}
	else //if (GroupNames.Contains("Student"))
	{
		GameInstance->UserInfo.UserType = EUserType::Student;
	}

	// Debug messages
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Usertype: %s"), *UEnum::GetValueAsString(GameInstance->UserInfo.UserType)));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Username: %s"), *GameInstance->UserInfo.Username.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("User  ID: %i"), GameInstance->UserInfo.id));
	}
}

/**
 * @brief Removes this widget and shows main menu.
 * 
 */
void ULoginWidget::SwitchToMainMenu()
{
	RemoveFromParent();
	
	UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AEditorHUD>()->SetupMainMenuWidget();
}

/**
 * @brief Quits the game.
 * 
 */
void ULoginWidget::QuitGame()
{
	UWorld* World = GetWorld();

	UKismetSystemLibrary::QuitGame(World, UGameplayStatics::GetPlayerController(World, 0), EQuitPreference::Quit, false);
}

