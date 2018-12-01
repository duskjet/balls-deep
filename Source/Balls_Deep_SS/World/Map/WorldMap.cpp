// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMap.h"
PRAGMA_DISABLE_OPTIMIZATION

UWorldMap::UWorldMap()
{
}

UWorldMap::~UWorldMap()
{
}

void UWorldMap::Init(int32 Width, int32 Height)
{
	this->Width = Width;
	this->Height = Height;

	Grid.SetNum(Width * Height);

	for (int32 y = 0; y < Height; y++)
		for (int32 x = 0; x < Width; x++)
		{
			UTile* Tile = NewObject<UTile>();
			Tile->Pos = FIntPoint(x, y);

			Grid[Width * y + x] = Tile;
		}
}

TArray<UTileArea*> UWorldMap::GetGroups()
{
	TArray<UTileArea*> groups;

	for (int32 y = 0; y < Height; y++)
		for (int32 x = 0; x < Width; x++)
		{
			UTile* tile = Grid[Width * y + x];

			// Create new group
			if (!tile->bSolid && !tile->HasGroup())
			{
				UTileArea* group = NewObject<UTileArea>();
				group->Init(tile, this);

				groups.Add(group);
			}
		}

	ClearTileIndexes();

	return groups;
}

UWorldMap* UWorldMap::Copy()
{
	UWorldMap* NewMap = NewObject<UWorldMap>();

	NewMap->Width = Width;
	NewMap->Height = Height;

	NewMap->Grid.SetNum(Grid.Num());

	for (int32 y = 0; y < Height; y++)
		for (int32 x = 0; x < Width; x++)
		{
			const int32 index = Width * y + x;
			UTile* TileCopy = DuplicateObject<UTile>(Grid[index], Grid[index]->GetOuter());

			NewMap->Grid[index] = TileCopy;
		}

	return NewMap;
}

void UWorldMap::ClearTileIndexes()
{
	for (UTile* tile : Grid) 
	{
		tile->Index = UTile::DEFAULT_INDEX;
	}
}