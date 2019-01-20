// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldMap.h"
#include "WorldMapComponent.h"
#include "TileArea.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BALLS_DEEP_SS_API UTileArea : public UObject
{
	GENERATED_BODY()

public:
	UTileArea();

	~UTileArea();

	UPROPERTY(BlueprintReadWrite)
		int32 Id;

	UPROPERTY(BlueprintReadWrite)
		bool bSolid;

	UPROPERTY(BlueprintReadWrite)
		FIntPoint StartPoint;

	UPROPERTY(BlueprintReadWrite)
		FIntPoint EndPoint;

	UPROPERTY(BlueprintReadWrite)
		int32 Volume;

	UPROPERTY(BlueprintReadWrite)
		TArray<class UTile*> Tiles;

	void Init(UTile* StartTile, class UWorldMap* Map);
	void Init(UTile* StartTile, class UWorldMapComponent* Map);

private:
	void CalcVolume(UTile*& Tile, UWorldMap*& Map, bool& Solid, int32& WaveIndex);
	void CalcVolume(UTile*& Tile, UWorldMapComponent*& Map, bool& Solid, int32& WaveIndex);
};
