// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldMap.generated.h"
/**
 * 
 */

UCLASS(BlueprintType)
class BALLS_DEEP_SS_API UTile : public UObject
{
	GENERATED_BODY()

public:
	UTile()
	{
		bSolid = false;
		Index = -1;
	}

	~UTile() {}

	UPROPERTY(BlueprintReadWrite)
		bool bSolid;

	UPROPERTY(BlueprintReadWrite)
		int32 Index;

	UPROPERTY(BlueprintReadOnly)
		FIntPoint Pos;

	bool HasGroup()
	{
		return Index >= 0;
	}
};

UCLASS(BlueprintType)
class BALLS_DEEP_SS_API UWorldMap : public UObject //BALLS_DEEP_SS_API
{
	GENERATED_BODY()

public:
	UWorldMap();
	~UWorldMap();

	UPROPERTY(BlueprintReadWrite, Category = Grid)
		TArray<UTile*> Grid;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
		int32 Width;

	UPROPERTY(BlueprintReadWrite, Category = Grid)
		int32 Height;
};


UCLASS(BlueprintType)
class BALLS_DEEP_SS_API UTileGroup : public UObject //BALLS_DEEP_SS_API
{
	GENERATED_BODY()

public:
	UTileGroup() {}

	~UTileGroup() {}

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
		TArray<UTile*> Tiles;

	void Init(UTile* StartTile, UWorldMap* Map) 
	{
		StartPoint = StartTile->Pos;
		CalcVolume(StartTile, Map, StartTile->bSolid);
	}

private:
	void CalcVolume(UTile* Tile, UWorldMap* Map, bool Solid, int32 WaveIndex = 0)
	{
		if (!Tile || Tile->bSolid != Solid) return;

		// Set wave index
		Tile->Index = WaveIndex; 
		WaveIndex++;

		// Add tile
		Tiles.Add(Tile);
		Volume++;

		FIntPoint pos = Tile->Pos;
		int32 max_width = Map->Width - 1;
		int32 max_height = Map->Height - 1;

		// Try top tile
		if (pos.Y > 0) {
			UTile* top = Map->Grid[Map->Width * (Tile->Pos.Y + 1) + Tile->Pos.X];
			CalcVolume(top, Map, Solid, WaveIndex);
		}

		// Try right tile
		if (pos.X < max_width) {
			UTile* right = Map->Grid[Map->Width * Tile->Pos.Y + (Tile->Pos.X + 1)];
			CalcVolume(right, Map, Solid, WaveIndex);
		}

		// Try bottom tile
		if (pos.Y < max_height) {
			UTile* bottom = Map->Grid[Map->Width * (Tile->Pos.Y - 1) + Tile->Pos.X];
			CalcVolume(bottom, Map, Solid, WaveIndex);
		}

		// Try left tile
		if (pos.X > 0) {
			UTile* left = Map->Grid[Map->Width * Tile->Pos.Y + (Tile->Pos.X - 1)];
			CalcVolume(left, Map, Solid, WaveIndex);
		}
	}

	void ScanTiles(UTile* StartTile, UWorldMap* Map)
	{
		bool solidness = StartTile->bSolid;

		this->CalcVolume(StartTile, Map, solidness);
	}
};
