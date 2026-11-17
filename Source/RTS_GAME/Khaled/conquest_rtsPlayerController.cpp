// Copyright Epic Games, Inc. All Rights Reserved.


#include "conquest_rtsPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "RTS_GAME/RTS_GAME.h"
#include "Widgets/Input/SVirtualJoystick.h"

void Aconquest_rtsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalPlayerController())
	{
		return;
	}

	// Restore gameplay input after transitioning from the main menu.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	// Only spawn touch controls on local player controllers.
	if (SVirtualJoystick::ShouldDisplayTouchInterface())
	{
		// Spawn the mobile controls widget.
		MobileControlsWidget = CreateWidget<UUserWidget>(
			this,
			MobileControlsWidgetClass
		);

		if (MobileControlsWidget)
		{
			// Add the controls to the player screen.
			MobileControlsWidget->AddToPlayerScreen(0);
		}
		else
		{
			//UE_LOG(
			//	LogRTS_GAME,
			//	Error,
			//	TEXT("Could not spawn mobile controls widget.")
			//);
		}
	}
}

void Aconquest_rtsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}


void Aconquest_rtsPlayerController::ApplyUIInputMode()
{
	FInputModeUIOnly InputMode;

	InputMode.SetLockMouseToViewportBehavior(
		EMouseLockMode::DoNotLock
	);

	SetInputMode(InputMode);

	bShowMouseCursor = true;

	// Prevent the mobile gameplay controls from receiving input
	// while a menu is active.
	if (MobileControlsWidget)
	{
		MobileControlsWidget->SetVisibility(
			ESlateVisibility::Collapsed
		);
	}
}


UUserWidget* Aconquest_rtsPlayerController::SwitchToWidget(
	TSubclassOf<UUserWidget> NewWidgetClass
)
{
	if (!IsLocalController())
	{
		return nullptr;
	}

	// nullptr means close everything.
	if (!NewWidgetClass)
	{
		CloseActiveWidget();
		return nullptr;
	}

	// Calling the currently active widget class toggles it closed.
	if (ActiveWidget &&
		ActiveWidget->GetClass() == NewWidgetClass.Get())
	{
		CloseActiveWidget();
		return nullptr;
	}

	// Create the new instance before removing the existing widget.
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(
		this,
		NewWidgetClass
	);

	if (!NewWidget)
	{
		//UE_LOG(
		//	Logconquest_rts,
		//	Error,
		//	TEXT("UI: Failed to create widget.")
		//);

		return nullptr;
	}

	// Store the current widget's CLASS, not its instance.
	if (ActiveWidget)
	{
		WidgetHistory.Add(
			TSubclassOf<UUserWidget>(ActiveWidget->GetClass())
		);

		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	ActiveWidget = NewWidget;
	ActiveWidget->AddToViewport();

	ApplyUIInputMode();

	return ActiveWidget;
}


bool Aconquest_rtsPlayerController::GoBackToPreviousWidget()
{
	if (!IsLocalController() || !ActiveWidget)
	{
		return false;
	}

	// Discard invalid classes until we find a usable one.
	while (!WidgetHistory.IsEmpty())
	{
		TSubclassOf<UUserWidget> PreviousWidgetClass =
			WidgetHistory.Last();

		if (!PreviousWidgetClass)
		{
			WidgetHistory.Pop();
			continue;
		}

		// Always create a NEW instance.
		UUserWidget* PreviousWidget = CreateWidget<UUserWidget>(
			this,
			PreviousWidgetClass
		);

		if (!PreviousWidget)
		{
			//UE_LOG(
			//	Logconquest_rts,
			//	Error,
			//	TEXT("UI: Failed to recreate previous widget.")
			//);

			return false;
		}

		// Remove the history entry only after successful creation.
		WidgetHistory.Pop();

		// Remove the current instance.
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;

		// Add the new instance of the previous class.
		ActiveWidget = PreviousWidget;
		ActiveWidget->AddToViewport();

		ApplyUIInputMode();

		return true;
	}

	return false;
}


void Aconquest_rtsPlayerController::CloseActiveWidget()
{
	if (ActiveWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}

	// Closing completely resets the navigation stack.
	WidgetHistory.Empty();

	// Restore gameplay input.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = false;

	// Restore mobile controls.
	if (MobileControlsWidget)
	{
		MobileControlsWidget->SetVisibility(
			ESlateVisibility::Visible
		);
	}
}
