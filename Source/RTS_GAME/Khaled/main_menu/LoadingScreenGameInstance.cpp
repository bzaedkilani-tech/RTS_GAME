// Fill out your copyright notice in the Description page of Project Settings.


#include "./LoadingScreenGameInstance.h"
#include "MoviePlayer.h"

void ULoadingScreenGameInstance::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(
        this,
        &ULoadingScreenGameInstance::BeginLoadingScreen
    );

    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
        this,
        &ULoadingScreenGameInstance::EndLoadingScreen
    );
}

void ULoadingScreenGameInstance::BeginLoadingScreen(const FString& MapName)
{
    if (IsRunningDedicatedServer())
        return;

    UE_LOG(LogTemp, Log, TEXT("BeginLoadingScreen: %s"), *MapName);

    FLoadingScreenAttributes Attributes;

    Attributes.bAutoCompleteWhenLoadingCompletes = true;
    Attributes.bMoviesAreSkippable = false;
    Attributes.bWaitForManualStop = false;
    Attributes.bAllowInEarlyStartup = false;

    Attributes.MinimumLoadingScreenDisplayTime = 2.0f;
    Attributes.PlaybackType = EMoviePlaybackType::MT_Looped;

    // Background video
    Attributes.MoviePaths.Add(TEXT("LoadingScreenMovie"));

    // Create the UMG loading screen
    if (LoadingScreenWidgetClass)
    {
        LoadingScreenWidgetInstance = CreateWidget<UUserWidget>(
            this,
            LoadingScreenWidgetClass
        );

        if (LoadingScreenWidgetInstance)
        {
            Attributes.WidgetLoadingScreen =
                LoadingScreenWidgetInstance->TakeWidget();
        }
    }

    GetMoviePlayer()->SetupLoadingScreen(Attributes);
    GetMoviePlayer()->PlayMovie();
}

void ULoadingScreenGameInstance::EndLoadingScreen(UWorld* LoadedWorld)
{
    // Optional post-loading logic.
}