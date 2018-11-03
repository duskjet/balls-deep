// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapComponent.h"

#include "Tilemap.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct BALLS_DEEP_SS_API FTilemap
{
	GENERATED_BODY()

public:
	FTilemap();
	~FTilemap();

	UPROPERTY()
	UPaperTileMapComponent* Tilemap;

	UPROPERTY()
	FIntPoint Position;
};
