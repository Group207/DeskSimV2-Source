// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DraggedObject.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ObjectWidget.generated.h"

/**
 * The Content browser asset holding class
 */
UCLASS()
class DESKSIMV2_API UObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UObjectWidget(const FObjectInitializer& ObjectInitializer);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UImage* ImageObject; ///< Binding for image in object widget

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UTextBlock* TitleText; ///< Binding for title text in object widget

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UTextBlock* DescriptionText; ///< Binding for description text in object widget

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ScenarioButton; ///< Binding for scenario button in object widget

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<class UUserWidget> DraggedObjectWidgetClass; ///< The object widget class
	
	TSubclassOf<AActor> ReferencedActor; ///< The actor recieved from EditorHUD

	APlayerController* PC; ///< The player controller
	
	// Set functions for the native members of the object widget.
	void SetTitleText(FString Text); 
	void SetDescriptionText(FString Text);
	void SetImageResource(UTexture2D* Texture);
	void SetActorResource(TSubclassOf<AActor> Actor);

	TArray<AActor*> AllActors; ///< all the actors created in the editor.
	
};
