// Fill out your copyright notice in the Description page of Project Settings.

#include "TileArea.h"

UTileArea::UTileArea()
{
}

UTileArea::~UTileArea()
{
}

void UTileArea::Init(UTile* StartTile, UWorldMap* Map)
{
	StartPoint = StartTile->Pos;
	EndPoint = StartTile->Pos;

	int32 index = 0;
	CalcVolume(StartTile, Map, StartTile->bSolid, index);

	UE_LOG(WorldGen, Log, TEXT("[%s] Created group with Volume = (%d)"),
		   *this->GetFName().ToString(),
		   this->Volume);
}

void UTileArea::CalcVolume(UTile*& Tile, UWorldMap*& Map, bool& Solid, int32& WaveIndex)
{
	if (!Tile || Tile->bSolid != Solid) return;
	if (Tile->HasGroup()) return;
	check(WaveIndex < Map->Height * Map->Width);

	// Adjust last tile position
	if (Tile->Pos.X > EndPoint.X) EndPoint.X = Tile->Pos.X;
	if (Tile->Pos.Y > EndPoint.Y) EndPoint.Y = Tile->Pos.Y;

	// Set wave index
	Tile->Index = WaveIndex;
	WaveIndex++;

	// Add tile
	Tiles.Add(Tile);
	Volume++;

	// Try top tile
	if (Tile->Pos.Y > 0) {
		UTile* top = Map->Grid[Map->Width * (Tile->Pos.Y - 1) + Tile->Pos.X];
		CalcVolume(top, Map, Solid, WaveIndex);
	}

	// Try right tile
	if (Tile->Pos.X < Map->Width - 1) {
		UTile* right = Map->Grid[Map->Width * Tile->Pos.Y + (Tile->Pos.X + 1)];
		CalcVolume(right, Map, Solid, WaveIndex);
	}

	// Try bottom tile
	if (Tile->Pos.Y < Map->Height - 1) {
		UTile* bottom = Map->Grid[Map->Width * (Tile->Pos.Y + 1) + Tile->Pos.X];
		CalcVolume(bottom, Map, Solid, WaveIndex);
	}

	// Try left tile
	if (Tile->Pos.X > 0) {
		UTile* left = Map->Grid[Map->Width * Tile->Pos.Y + (Tile->Pos.X - 1)];
		CalcVolume(left, Map, Solid, WaveIndex);
	}
}

void UTileArea::Init(UTile* StartTile, UWorldMapComponent* Map)
{
	StartPoint = StartTile->Pos;
	EndPoint = StartTile->Pos;

	int32 index = 0;
	CalcVolume(StartTile, Map, StartTile->bSolid, index);

	UE_LOG(WorldGen, Log, TEXT("[%s] Created group with Volume = (%d)"),
		*this->GetFName().ToString(),
		this->Volume);
}

void UTileArea::CalcVolume(UTile*& Tile, UWorldMapComponent*& Map, bool& Solid, int32& WaveIndex)
{
	if (!Tile || Tile->bSolid != Solid) return;
	if (Tile->HasGroup()) return;
	check(WaveIndex < Map->Height * Map->Width);

	// Adjust last tile position
	if (Tile->Pos.X > EndPoint.X) EndPoint.X = Tile->Pos.X;
	if (Tile->Pos.Y > EndPoint.Y) EndPoint.Y = Tile->Pos.Y;

	// Set wave index
	Tile->Index = WaveIndex;
	WaveIndex++;

	// Add tile
	Tiles.Add(Tile);
	Volume++;

	// Try top tile
	if (Tile->Pos.Y > 0) {
		UTile* top = Map->Grid[Map->Width * (Tile->Pos.Y - 1) + Tile->Pos.X];
		CalcVolume(top, Map, Solid, WaveIndex);
	}

	// Try right tile
	if (Tile->Pos.X < Map->Width - 1) {
		UTile* right = Map->Grid[Map->Width * Tile->Pos.Y + (Tile->Pos.X + 1)];
		CalcVolume(right, Map, Solid, WaveIndex);
	}

	// Try bottom tile
	if (Tile->Pos.Y < Map->Height - 1) {
		UTile* bottom = Map->Grid[Map->Width * (Tile->Pos.Y + 1) + Tile->Pos.X];
		CalcVolume(bottom, Map, Solid, WaveIndex);
	}

	// Try left tile
	if (Tile->Pos.X > 0) {
		UTile* left = Map->Grid[Map->Width * Tile->Pos.Y + (Tile->Pos.X - 1)];
		CalcVolume(left, Map, Solid, WaveIndex);
	}
}
