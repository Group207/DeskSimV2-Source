// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ScenarioWidget.generated.h"

/**
 * The class that is used to display all of the holders for each created scenario
 */
UCLASS()
class DESKSIMV2_API UScenarioWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UTextBlock* TitleText; ///< Binding for title text in object widget
	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
		class UTextBlock* DescriptionText; ///< Binding for description text in object widget
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* ScenarioButton; ///< Binding for scenario button in object widget

	void SetMembers(FString Name, FString Description, FName MapName);
	
	UFUNCTION()
		void ChangeToLevel();

	UFUNCTION()
		void SetMenuMode(bool mode);

private:
	FName MapNameReference; // The FName of the map reference.

	bool bMenuMode; // If the menu mode is Main Menu or Editor mode
	
};
