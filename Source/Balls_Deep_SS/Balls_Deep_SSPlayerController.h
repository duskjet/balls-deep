// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "World/Map/Tilemap.h"
#include "Balls_Deep_SSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BALLS_DEEP_SS_API ABalls_Deep_SSPlayerController : public APlayerController
{
	GENERATED_BODY()

	ABalls_Deep_SSPlayerController();
	~ABalls_Deep_SSPlayerController();
	
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;

public:
	UFUNCTION(Server, reliable, WithValidation)
	void Server_SetTile(const FVector ClickWorldPos, ATilemap* Tilemap);

	UFUNCTION(Client, reliable)
	void ClientLoadWorldMap(UWorldMap* Map);
};
