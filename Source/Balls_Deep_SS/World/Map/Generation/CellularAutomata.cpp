// Fill out your copyright notice in the Description page of Project Settings.

#include "CellularAutomata.h"
#include "Engine.h"
PRAGMA_DISABLE_OPTIMIZATION
UCellularAutomata::UCellularAutomata()
{
}

UCellularAutomata::UCellularAutomata(TArray<int32> InBorn, TArray<int32> InSurvive) : 
	Born(InBorn), 
	Survive(InSurvive)
{
}

UCellularAutomata::~UCellularAutomata()
{
}

void UCellularAutomata::FillWithRandomNoise(UWorldMap * Map, float balance)
{
	int32 width = Map->Width;
	int32 height = Map->Height;

	for (int32 y = 0; y < height; y++)
		for (int32 x = 0; x < width; x++)
		{
			float random = FMath::RandRange(0.0f, 1.0f);
			bool cell = random > balance;

			Map->Grid[width * y + x]->bSolid = cell;//FMath::RandBool();
		}
}

//void UCellularAutomata::Smooth(UWorldMap*& Map, int32 Iterations)
//{
//	for (int32 iteration = 0; iteration < Iterations; iteration++) {
//		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current iteration: %d"), iteration));
//		SmoothOnce(Map);
//	}
//}

//void UCellularAutomata::SmoothOnce(UWorldMap*& Map)
//{
//	int32 width = Map->Width;
//	int32 height = Map->Height;
//
//	UWorldMap* MapCopy = Map->Copy();
//
//	for (int32 y = 1; y < height - 1; y++)
//		for (int32 x = 1; x < width - 1; x++)
//		{
//			int32 alive = GetAliveNeighborsCount(&(Map->Grid), width, x, y);
//
//			/*if (Map->Grid[width * y + x]) {
//				MapCopy[width * y + x] = Survive.Contains(alive);
//			}
//			else {
//				MapCopy[width * y + x] = Born.Contains(alive);
//			}*/
//			UTile* newTile = MapCopy->Grid[width * y + x];
//			UTile* oldTile = Map->Grid[width * y + x];
//
//			newTile->bSolid = // sometimes crashes
//				(!oldTile->bSolid && Survive.Contains(alive))
//				|| (oldTile->bSolid && Born.Contains(alive));
//		}
//	
//	Map = MapCopy;
//}

void UCellularAutomata::FillWithNoise(TArray<UTile*>& InMap, int32 Width, int32 Height, float balance)
{
	for (int32 y = 0; y < Height; y++)
		for (int32 x = 0; x < Width; x++)
		{
			float random = FMath::RandRange(0.0f, 1.0f);
			bool cell = random > balance;

			InMap[Width * y + x]->bSolid = cell;//FMath::RandBool();
		}
}

void UCellularAutomata::Smooth(TArray<UTile*>& InMap, int32 Width, int32 Height, TArray<int32> InBorn, TArray<int32> InSurvive)
{
	TArray<UTile*> MapCopy = CopyTileArray(InMap);

	for (int32 y = 1; y < Height - 1; y++)
		for (int32 x = 1; x < Width - 1; x++)
		{
			int32 alive = GetAliveNeighborsCount(InMap, Width, x, y);

			/*if (Map->Grid[width * y + x]) {
				MapCopy[width * y + x] = Survive.Contains(alive);
			}
			else {
				MapCopy[width * y + x] = Born.Contains(alive);
			}*/
			UTile* newTile = MapCopy[Width * y + x];
			UTile* oldTile = InMap[Width * y + x];

			newTile->bSolid = // sometimes crashes
				(!oldTile->bSolid && InSurvive.Contains(alive))
				|| (oldTile->bSolid && InBorn.Contains(alive));
		}

	InMap = MapCopy;
}

TArray<UTile*> UCellularAutomata::CopyTileArray(TArray<UTile*>& InMap)
{
	TArray<UTile*> MapCopy;

	MapCopy.SetNum(InMap.Num());

	for (auto It = InMap.CreateConstIterator(); It; ++It)
	{
		UTile* TileCopy = DuplicateObject<UTile>(*It, (*It)->GetOuter());
		MapCopy[It.GetIndex()] = TileCopy;
	}

	return MapCopy;
}

int32 UCellularAutomata::GetAliveNeighborsCount(TArray<UTile*>& Grid, int32 Width, int32 X, int32 Y)
{
	bool cells[8] =
	{
		(Grid)[Width * (Y - 1) + (X)]->bSolid,		// top
		(Grid)[Width * (Y - 1) + (X + 1)]->bSolid,	// top right
		(Grid)[Width * (Y)+(X + 1)]->bSolid,		// right
		(Grid)[Width * (Y + 1) + (X + 1)]->bSolid,	// bot right
		(Grid)[Width * (Y + 1) + (X)]->bSolid,		// bot
		(Grid)[Width * (Y + 1) + (X - 1)]->bSolid,	// bot left
		(Grid)[Width * (Y)+(X - 1)]->bSolid,		// left
		(Grid)[Width * (Y - 1) + (X - 1)]->bSolid,	// top left
	};

	int32 alive = 0;
	for (bool cell : cells)
		if (cell) alive++;

	return alive;
}
