// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMap.h"

UWorldMap::UWorldMap()
{
}

UWorldMap::~UWorldMap()
{
}

//void UTileGroup::CalcVolume(UTile* Tile, UWorldMap* Map, bool Solid)
//{
//	// Add tile
//	Tiles.Add(Tile);
//	Volume++;
//
//	// Try top tile
//	UTile* top = Map->Grid[Map->Width * Tile->Pos.Y + (Tile->Pos.X + 1)];
//
//	if (top && top->bSolid == Solid) CalcVolume(top, Map, Solid);
//}