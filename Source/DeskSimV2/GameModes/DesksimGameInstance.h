/*****************************************************************/
/**
 * @file   DesksimGameInstance.h
 * @brief  Gameinstance class containing some global info
 * 
 * @author Endre Heksum
 * @date   April 2022
 *********************************************************************/
// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DesksimGameInstance.generated.h"




UENUM(BlueprintType)
enum class EUserType : uint8
{
	Student,
	Teacher,
	Admin
};

/**
 * @brief Stuct containing info about the logged in user.
 * 
 * Stores all the info received by UserLogin
 */
USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()

	// Info about user
	UPROPERTY(BlueprintReadWrite, Category = "UserInfo")
	bool bIsAuthenticated = false;

	UPROPERTY(BlueprintReadWrite, Category = "UserInfo")
	EUserType UserType;

	UPROPERTY(BlueprintReadWrite, Category = "UserInfo")
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, Category = "UserInfo")
	FName Username;

	FName Operator;

	bool bIsEnabled;

	int32 SubscriptionID;
	
};

/**
 * @brief UDesksimGameInstance contains some global info.
 * 
 * Info is stored as long as game is open, and is not lost between levels.
 */
UCLASS()
class DESKSIMV2_API UDesksimGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	// Stuff here

	bool bIsLoggedIn = false;

	/// Login related info
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UserInfo")
	FUserInfo UserInfo;


	
};
