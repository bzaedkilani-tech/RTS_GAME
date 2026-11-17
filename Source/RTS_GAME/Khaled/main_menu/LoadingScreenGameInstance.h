// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RTS_GAME_API ULoadingScreenGameInstance : public UGameInstance
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> LoadingScreenWidgetInstance;

public:
    virtual void Init() override;

    UFUNCTION()
    void BeginLoadingScreen(const FString& MapName);

    UFUNCTION()
    void EndLoadingScreen(UWorld* LoadedWorld);

public:
    UPROPERTY(EditDefaultsOnly, Category = "Loading Screen")
    TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

};