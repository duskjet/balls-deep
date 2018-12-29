// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Balls_Deep_SSGameMode.generated.h"

/**
 * The GameMode defines the game being played. It governs the game rules, scoring, what actors
 * are allowed to exist in this game type, and who may enter the game.
 *
 * This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of Balls_Deep_SSCharacter
 */
UCLASS(minimalapi)
class ABalls_Deep_SSGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABalls_Deep_SSGameMode();

	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(BlueprintReadOnly)
	class APlayerStart* DefaultStart;

private:
	/* List of Player Controllers */
	TArray<class APlayerController*> PlayerControllerList;

	FVector GetSpawnPoint(AController* Player);

	void UpdateDefaultStart();

	void DelegateTest(FVector2D pos);
};
