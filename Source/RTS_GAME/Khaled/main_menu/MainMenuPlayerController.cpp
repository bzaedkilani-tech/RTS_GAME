// Fill out your copyright notice in the Description page of Project Settings.


#include "./MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	// Set the view to the first CameraActor in the level.
	if (AActor* Camera = UGameplayStatics::GetActorOfClass(
		this,
		ACameraActor::StaticClass()
	))
	{
		SetViewTarget(Camera);
	}

	if (MainMenuWidgetClass)
	{
		SwitchToWidget(MainMenuWidgetClass);
	}
}
