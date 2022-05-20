// Copyright 2022 Thomas Arinesalingam, John Ole Bjerke, Endre Heksum & Henrik Markengbakken Karlsen . All Rights Reserved.


#include "ObjectWidget.h"
//#include <DeskSimV2/Train.h>
#include "DrawDebugHelpers.h" // Debug
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "WidgetDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DraggedObject.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"


/**
 * @brief Gets the player controller and sets mouse editing enabled.
 * 
 * @param ObjectInitializer - Basic userwidget initializer
 */
UObjectWidget::UObjectWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{	
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC)
	{
		PC->bEnableMouseOverEvents;
		PC->bEnableClickEvents;
		PC->EnableInput(PC);
	}
}

/**
 * @brief notifies when the right mouse button is down.
 * 
 * @param InGeometry - The geometry of the event.
 * @param InMouseEvent - The actual mouse event.
 * 
 * @return NativeReply of the mouse event.
 */
FReply UObjectWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply tmp_Reply = Super::OnMouseButtonDown(InGeometry, InMouseEvent).NativeReply;

	
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
}

/**
 * @brief Creates a drag operation if a drag is detected from the widget.
 * 
 * @param InGeometry - The geometry
 * @param InMouseEvent - Mouse event
 * @param OutOperation - The drag operation
 */
void UObjectWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("DRAAAAAG DETECTED!"));

	UWidgetDragDropOperation* DragOperation = 
		Cast<UWidgetDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDragDropOperation::StaticClass()));
	
	UDraggedObject* DragWidget = CreateWidget<UDraggedObject>(PC, DraggedObjectWidgetClass, FName("DraggedObject"));
	DragOperation->DraggedWidget = DragWidget;
	
	DragOperation->ScreenOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	
	DragOperation->DefaultDragVisual = DragWidget;
	DragOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = DragOperation;	
}

/**
 * @brief Recieves a drag event that si canceled. Finds the player position and cast's a ray from there in a straight line 
 * to the ground. The ray hit rocation is stored in the Hit variable and this is used to place a new actor on the level. .
 * 
 * @param InDragDropEvent
 * @param InOperation
 */
void UObjectWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	FRotator Rotation;
	FVector Location;
	FHitResult Hit;
	PC->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * 20000);

	FCollisionQueryParams TraceParams;

	bool WasHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// If the ray did not hit anything
	if (!WasHit)
	{

		return;
	}

	FVector HitLocation = Hit.Location;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 30, 0, 5); // Debugging line
	FRotator StartRotation = FRotator(1, 0, 0);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewActor = GetWorld()->SpawnActor<AActor>(ReferencedActor, HitLocation, StartRotation, SpawnParams);


	AllActors.Add(NewActor);
}

/**
 * @brief Set's the title text of the widget.
 * 
 * @param Text - The incoming title
 */
void UObjectWidget::SetTitleText(FString Text)
{
	TitleText->SetText(FText::FromString(Text));
}

/**
 * @brief Sets the description of the widget.
 * 
 * @param Text - The incoming description
 */
void UObjectWidget::SetDescriptionText(FString Text)
{
	DescriptionText->SetText(FText::FromString(Text));
}

/**
 * @brief Sets the 2D texture of the object.
 * 
 * @param Texture - The incoming texture 
 */
void UObjectWidget::SetImageResource(UTexture2D* Texture)
{
	ImageObject->SetBrushFromTexture(Texture);
}

/**
 * @brief Sets the actor resource of the widget.
 * 
 * @param Actor - The incoming actor reference
 */
void UObjectWidget::SetActorResource(TSubclassOf<AActor> Actor)
{
	ReferencedActor = Actor;
}
