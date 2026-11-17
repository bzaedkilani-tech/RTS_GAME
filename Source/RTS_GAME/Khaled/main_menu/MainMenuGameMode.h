#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/** GameMode for L_MainMenu. No gameplay pawn; uses the menu PlayerController. */
UCLASS()
class RTS_GAME_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();
};