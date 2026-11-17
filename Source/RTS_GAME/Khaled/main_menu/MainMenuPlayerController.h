#pragma once

#include "CoreMinimal.h"
#include "RTS_GAME/Khaled/conquest_rtsPlayerController.h"
#include "MainMenuPlayerController.generated.h"

class UUserWidget;

UCLASS()
class RTS_GAME_API AMainMenuPlayerController : public Aconquest_rtsPlayerController
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

	/** Assign WBP_MainMenu in the Blueprint child / class defaults. */
	UPROPERTY(EditDefaultsOnly, Category = "MainMenu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	/** Map opened by Play. Avoids hardcoded level name strings. */
	UPROPERTY(EditDefaultsOnly, Category = "MainMenu")
	TSoftObjectPtr<UWorld> GameplayLevel;


};