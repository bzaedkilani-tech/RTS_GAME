
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "conquest_rtsPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

UCLASS(abstract)
class Aconquest_rtsPlayerController : public APlayerController
	/**
		Name kept from my own project, but this class is meant to be a base for all player controllers in the game.
	*/
{
	GENERATED_BODY()

public:

	/**
	 * Opens, replaces, or closes the active widget.
	 *
	 * nullptr closes the active widget and clears history.
	 * Passing the currently active class toggles it closed.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* SwitchToWidget(
		TSubclassOf<UUserWidget> NewWidgetClass = nullptr
	);

	/** Opens a fresh instance of the previous widget class. */
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GoBackToPreviousWidget();

protected:

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	UPROPERTY(EditAnywhere, Category = "Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	/** Currently active widget instance. */
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> ActiveWidget;

	/** Navigation history: stores classes, never widget instances. */
	UPROPERTY(Transient)
	TArray<TSubclassOf<UUserWidget>> WidgetHistory;

	/** Closes the active widget and clears navigation history. */
	void CloseActiveWidget();

	/** Applies UI-only input mode. */
	void ApplyUIInputMode();
};
